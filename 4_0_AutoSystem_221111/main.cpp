#include "mbed.h"
#include "QEncoder.h"
#include "MD30C.h"
#include "PID.h"
#include <algorithm>
#include <cstdint>
// #include <ros.h>


////////////PIN_NUMBER////////////////////
#define PwmL D9 
#define DIRL D8
#define MOTORL_PINA D4
#define MOTORL_PINB D5

#define PwmR D11
#define DIRR D10
#define MOTORR_PINA D2
#define MOTORR_PINB D3


///////////SAMPLING TIME(MS) && UPDATE TIME/////////////
#define PRINT_INTERVAL 100 
#define SAMPLING_TIME_ENC 30 
#define UPDATE_TIME_ODOM 100
#define DT SAMPLING_TIME_ENC/1000.
#define WRITING_TIME_MOTOR 30


///////////////RATE CONTROL PARAMETER//////////////

#define PWM_MAX 4095            // 12 bit
#define RATE_CONTROLLER_KP 3.0 	
#define RATE_CONTROLLER_KD 0.04
#define RATE_CONTROLLER_KI 0.5
#define RATE_CONTROLLER_IMAX 100.0
#define MAX_ANGULAR_VEL 18 // PWM줘서 돌려보고 모터 속도 측정해서 세팅 

///////////////CONSTANT////////////////////
#define WHEEL_RADIUS 45.0 // 바퀴 반지름
#define C 9.0
#define ALPHA 0.7

uint8_t motorR_direction, motorL_direction;
float motorR_rpm, motorL_rpm;
float motorR_rate, motorL_rate;
float motorR_rate_ref, motorL_rate_ref;
float motorR_rate_input, motorL_rate_input;
float linear_velocity, angular_velocity;
float linear_velocity_ref, angular_velocity_ref;
float target, y;
float yaw;
uint64_t odom_prev_time;
volatile float pc_data[4];
bool gotPacket = false;
// L298LIB motor(EN, IN1, IN2);
RawSerial pc(USBTX, USBRX, 115200);

PIDBasic pid(RATE_CONTROLLER_KP, RATE_CONTROLLER_KI, RATE_CONTROLLER_KD, RATE_CONTROLLER_IMAX);// kp,ki,kd,imax
QEncoder encoR(MOTORR_PINA,MOTORR_PINB,ALPHA,SAMPLING_TIME_ENC,'R');
QEncoder encoL(MOTORL_PINA,MOTORL_PINB,ALPHA,SAMPLING_TIME_ENC,'L');

MD30C motorL(DIRL,PwmL);
MD30C motorR(DIRR,PwmR);

Thread read_encoder;
Thread control_motor;
Thread update_odom;

// Timer tmr, tmr_prt;

void enco_read();
void control_motor_vel();
void set_global_parameter();
void update_odometry();
void motor_run(float, float);
void commandVelocityCallback();

int main() {

    encoR.init();
    encoL.init();
    set_global_parameter();
    motorL.setSpeed(0);
    motorR.setSpeed(0);

    read_encoder.start(&enco_read);
    control_motor.start(&control_motor_vel);
    update_odom.start(&update_odometry);
    
    // tmr.start();
    // tmr_prt.start();

    while(1) {
        commandVelocityCallback();
    }
}

void set_global_parameter(){
    motorR_rpm = 0;
    motorL_rpm = 0;
    motorR_rate = 0;
    motorL_rate = 0;
    motorL_rate_ref=0;
    motorR_rate_ref=0;
    motorL_direction=1;
    motorR_direction=1;
    odom_prev_time = 0;
    yaw=0;
}


void enco_read(){//모터 각속도 읽기
    while(true){

        uint64_t Now_time = rtos::Kernel::get_ms_count();
        
        motorR_rate = encoR.CalRATE();//각속도 구하기
        // motorR_direction = encoR.get_direction();
        motorL_rate = encoL.CalRATE();
        // motorL_direction = encoL.get_direction();

        pc.printf("rate(R,L): %f, %f\n",motorR_rate, motorL_rate);
        pc.printf("ref(R,L): %f, %f\n",motorR_rate_ref, motorL_rate_ref);
        pc.printf("input(R,L): %f, %f\n ",motorR_rate_input, motorL_rate_input);
        // pc.printf("%f, %f\n", linear_velocity_ref, angular_velocity_ref);
        uint64_t Work_time = rtos::Kernel::get_ms_count();

        ThisThread::sleep_until(rtos::Kernel::get_ms_count() + (SAMPLING_TIME_ENC-(Work_time - Now_time)));
   }
}

void control_motor_vel(){//모터 각속도 쓰기
    while(true){

        motorR_rate_ref = (linear_velocity_ref + C / 2.f * angular_velocity_ref) / (WHEEL_RADIUS);
        motorL_rate_ref = (linear_velocity_ref - C / 2.f * angular_velocity_ref) / (WHEEL_RADIUS);

        uint64_t Now_time = rtos::Kernel::get_ms_count();

        motorR_rate_input = pid.computePID(motorR_rate_ref,motorR_rate,WRITING_TIME_MOTOR);
        motorL_rate_input = pid.computePID(motorL_rate_ref,motorL_rate,WRITING_TIME_MOTOR);
        motor_run(motorR_rate_input/MAX_ANGULAR_VEL, motorL_rate_input/MAX_ANGULAR_VEL);
        ///////debug x,y,z 파트 추가하면 됨////////////
        uint64_t Work_time = rtos::Kernel::get_ms_count();

        ThisThread::sleep_until(rtos::Kernel::get_ms_count() + (SAMPLING_TIME_ENC-(Work_time - Now_time)));
    }
}

void update_odometry(){
    ///쿼터니언에서 특정 축에 대한 회전 각도는 cos(회전각/2)이다. 회전 각과 좌표 축 합으로 나타내는게 쿼터니언....q0=회전각, q1,q2,q3=i,j,k축 좌표(x,y,z)
    ////
    while(1){
    uint64_t Now_time = rtos::Kernel::get_ms_count();

    // uint64_t odom_prev_time;
    float dt, dx, dy;
	float qw, qx, qy, qz;

    dt = (float)(rtos::Kernel::get_ms_count() - odom_prev_time) * 0.001f;

    odom_prev_time = rtos::Kernel::get_ms_count();

    linear_velocity = WHEEL_RADIUS * (motorR_rate + motorL_rate) / 2.0f;
    angular_velocity = (WHEEL_RADIUS / 2*C) * (motorR_rate - motorL_rate);
    yaw += angular_velocity * dt;

    dx = cos(yaw) * linear_velocity * dt;
    dy = sin(yaw) * linear_velocity * dt;

    // [qw, qx, qy, qz 추정]???
    // qw 추정 = cos(abs(추정 yaw))/2
	qw = cos(abs(yaw) / 2.0f);
    // qx 추정 = 0???
	qx = 0.0f;
    // qy 추정 = 0
	qy = 0.0f;
    // qz 추정 = sign(추정 yaw) * sin(abs(추정 yaw))/2
	qz = (yaw>0?1:-1) * sin(abs(yaw) / 2.0f);
    //  변위  
	//  odom.pose.pose.position.x += dx;
	// 	odom.pose.pose.position.y += dy;
	// 	odom.pose.pose.position.z = 0.0;

    pc.printf("position : (%f, %f)\n",dx,dy);
    // // // 쿼터니언
	// // 	odom.pose.pose.orientation.w = qw;
	// // 	odom.pose.pose.orientation.x = qx;
	// // 	odom.pose.pose.orientation.y = qy;
	// // 	odom.pose.pose.orientation.z = qz;
    pc.printf("orientation : (%f, %f)\n",qw, qz);
    pc.printf("linear, angvel : %f, %f",linear_velocity,angular_velocity);

    uint64_t Work_time = rtos::Kernel::get_ms_count();

    ThisThread::sleep_until(rtos::Kernel::get_ms_count() + (UPDATE_TIME_ODOM-(Work_time - Now_time)));
    }
}

void motor_run(float VelR, float VelL){
        motorR.run(VelR);
        motorL.run(VelL);
}

void commandVelocityCallback(){
    static char pc_serialInBuffer2[32];
    static int buff_cnt2 = 0;
        
    if(pc.readable()) {
        // pc.printf("%d \n", pc.readable()); // 확인용 코드
        // pc.printf("pc 연결 OK \n"); // pc, mbed 통신 출력
        char byteIn2 = pc.getc();
        if(byteIn2=='\n'){
            // pc.printf("byteIn == '띄워' : %c \n", byteIn2); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
            pc_serialInBuffer2[buff_cnt2] = '\0';
            pc_data[0]=atof(pc_serialInBuffer2);
            buff_cnt2=0;
            gotPacket = true;
        }
        else{
            // pc.printf("byteIn == '나머지' : %c \n", byteIn2); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
            pc_serialInBuffer2[buff_cnt2++]=byteIn2;
        }

        if(gotPacket) {
            gotPacket = false;
            linear_velocity_ref = pc_data[0];
            angular_velocity_ref = pc_data[0];
            // pc.printf("PWM = %.3f \n\r", pc_data[0]); // 확인용 코드
        }
    }

}