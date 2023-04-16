#include "mbed.h"
#include "GP2A.h"

//uart통신 설정
Thread com_thread;
char c;
Serial ras_mcu(PC_12, PD_2, 115200);
Serial pc_mcu(USBTX, USBRX, 115200);  //센서 값 읽기용

//서보모터 구동
Thread servo_thread;
PwmOut servo(PA_9);
float max_ang = 180.0;
float min_ang = 0.0;
float mid_ang = 90.0;
float ang = 90.0;
float ang_range = 25.0;
int find_mode = 0;
int no_mode = 0;
int no_count = 0;

//모터 구동
DigitalOut dirR(PC_9);
PwmOut motorR(PB_8);
DigitalOut dirL(PB_6);
PwmOut motorL(PC_7);
float rspd = 0;
float lspd = 0;

//센서 사용 ir + psd
int mode = 0;
int back_mode = 0;
int ir_state = 0;
int psd_state = 0;
AnalogIn ir0(PA_0); //PA_0 A0
AnalogIn ir1(PA_1); //PA_1 A1
AnalogIn ir2(PC_5); //PA_4 A2 변경
AnalogIn ir3(PB_0); //PB_0 A3
AnalogIn ir4(PC_1); //PC_1 A4
AnalogIn ir5(PC_0); //PC_0 A5

GP2A PSD0(PC_3, 20, 150, 60, 0); // 앞 왼쪽 대각선
GP2A PSD1(PC_2, 20, 150, 60, 0); // 정면 20~150cm
GP2A PSD2(PC_4, 20, 150, 60, 0); // 앞 오른족 대닥선

double distance_0, distance_1, distance_2;
uint16_t ir0_value, ir1_value, ir2_value, ir3_value, ir4_value, ir5_value;
uint16_t black = 33000;

//사용함수정의
void com();
void sensor();
void velocity();
void stop();
void drive();
void turnL();
void turnR();
void back();
void go_battle();
void ir_battle();
void ir_drive();
void left_area();
void right_area();
void two_area();
void servosetting(PwmOut &motor, float deg);
void servomove();

template <class T>
T map(T x, T in_min, T in_max, T out_min, T out_max);

int main(){
    motorL.period_us(100);         //DC모터 pwm 주기 설정
    motorR.period_us(100);         //DC모터 pwm 주기 설정
    servo.period_ms(20);           //서보모터 pwm 주기 설정
    servosetting(servo, mid_ang);  //서보모터 초기설정
    com_thread.start(com);         //통신 thread시작
    
    while(true){
        sensor();

        if(mode == 0){
            if(c == 'm' || c == 'G' || c =='M' || c == 'L' || c == 'R'){
                mode = 1;
                servo_thread.start(servomove);
            }
            else turnL();
        }
        else if(mode == 1){
            //ir_state
            //0이면 회전 가능
            //1이면 회전 불가능
            if(ir0_value > black && ir1_value > black){      //정면 둘다 검은색영역
                ir_state = 0;
            }
            else if(ir0_value > black && ir1_value < black){ //전방 오른쪽만 색영역
                if(ir5_value > black && ir2_value > black){
                    ir_state = 0;
                }
                else if(ir5_value > black && ir2_value < black){
                    if(ir4_value < black && ir3_value < black){
                        ir_state = 0;
                    }
                    else ir_state = 1;
                }
                else ir_state = 1;
            }
            else if(ir0_value < black && ir1_value > black){ //전방 왼쪽만 색영역
                if(ir5_value > black && ir2_value > black){
                    ir_state = 0;
                }
                else if(ir5_value < black && ir2_value > black){
                    if(ir4_value < black && ir3_value < black){
                        ir_state = 0;
                    }
                    else ir_state = 1;
                }
                else ir_state = 1;
            }
            else if(ir0_value < black && ir1_value < black){ //전방 둘다 색영역
                if(ir5_value > black && ir2_value > black){
                    if(ir4_value > black || ir3_value > black){
                        ir_state = 0;
                    }
                    else ir_state = 1;
                }
                else ir_state = 1;
            }

            //psd_state
            //0이면 배틀
            //1이면 전진
            //2이면 좌회전
            //3이면 우회전
            if(distance_1 > 50.0){
                psd_state = 1;
            }

            if(distance_1 <= 50.0){
                ir_state = 0;
                if(ang < (mid_ang - ang_range))       psd_state = 2;
                else if(ang > (mid_ang + ang_range))  psd_state = 3;
                else                                  psd_state = 0;
            }
            else if(distance_0 < 52.5 ){
                if(ang < mid_ang - ang_range)         psd_state = 2;
                else if(ang > mid_ang + ang_range)    psd_state = 3;
                else                                  psd_state = 1;
            }
            else if(distance_2 < 52.5 ){
                if(ang < mid_ang - ang_range)         psd_state = 2;
                else if(ang > mid_ang + ang_range)    psd_state = 3;
                else                                  psd_state = 1;
            }

            //통합판단
            if(c == 'L' || c == 'R' || c == 'G' || c == 'M'){
                if(ang < (mid_ang - 20.0))      find_mode = 2;
                else if(ang > (mid_ang + 20.0)) find_mode = 3;
                else find_mode = 0;
            }
            else if(c == 'n' && (no_mode == 1 || no_mode == 2))   find_mode = 4;
            else if(c == 'n' && no_mode == 5)   find_mode = 6;
            else if(c == 'n' && no_mode == 6)   find_mode = 5;
            else if (ir_state == 0){            
                if (psd_state == 0)           find_mode = 0;
                else if (psd_state == 1)      find_mode = 1;
                else if (psd_state == 2)      find_mode = 2;
                else if (psd_state == 3)      find_mode = 3;
            }
            else if(ir_state == 1)            find_mode = 1;

            switch (find_mode) {
                case 0:
                    //ang = 90.0f;
                    servosetting(servo, ang);
                    if(c == 'L'){
                        left_area();
                    }
                    else if(c == 'R'){
                        right_area();
                    }
                    else if(c == 'M' || c == 'G'){
                        two_area();
                    }
                    ir_battle();
                    break;
                case 1:
                    ir_drive();
                    break;
                case 2:
                    turnL();
                    break;
                case 3:
                    turnR();
                    break;
                case 4:
                    stop();
                    break;
                case 5:
                    //ang = 90.0f;
                    servosetting(servo, ang);
                    turnL();
                    break;
                case 6:
                    //ang = 90.0f;
                    servosetting(servo, ang);
                    turnR();
                    break;
                default:
                    break;
            }
        }
        velocity();
    }
}

void com(){
    while (true){
        if(ras_mcu.readable()){
            c = ras_mcu.getc();
        }
    }
}

void servosetting(PwmOut &motor, float deg){
    float pulseW=map<float>(deg, min_ang, max_ang, 500., 2500.);
    motor.pulsewidth_us(pulseW);
}

void servomove(){
    while (true) {
        if(c == 'm'){
            no_mode = 0;
            no_count = 0;
            servosetting(servo, ang);
        }
        else if(c == 'r'){
            no_mode = 0;
            no_count = 0;
            ang = ang + 0.00075f;
            if(ang >= max_ang) ang = max_ang;
            servosetting(servo, ang);
        }
        else if(c == 'l'){
            no_mode = 0;
            no_count = 0;
            ang = ang - 0.00075f;
            if(ang <= min_ang) ang = min_ang;
            servosetting(servo, ang);
        }
        else if(c == 'n'){
            if(no_mode == 0){
                if(ang < 90.0)        no_mode = 1;
                else if(ang >= 90.0)  no_mode = 2;
            }

            switch(no_mode) {
                case 1: //왼쪽에서 놓침
                    ang = ang - 0.00075f;
                    if(ang <= min_ang)      no_count = 1;
                    if(no_count == 1)       no_mode = 3;
                    servosetting(servo, ang);
                    break;
                case 2: //오른쪽에서 놓침
                    ang = ang + 0.00075f;
                    if(ang >= max_ang)      no_count = 1;
                    if(no_count == 1)       no_mode = 4;
                    servosetting(servo, ang);
                    break;
                case 3:
                    ang = 90.0f;
                    servosetting(servo, ang);
                    no_mode = 5;
                    break;
                case 4:
                    ang = 90.0f;
                    servosetting(servo, ang);
                    no_mode = 6;
                    break;
                default:
                    break;
            }
        }
    }
}

template <class T>
    T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void sensor(){
    ir0_value = ir0.read_u16();
    ir1_value = ir1.read_u16();
    ir2_value = ir2.read_u16();
    ir3_value = ir3.read_u16();
    ir4_value = ir4.read_u16();
    ir5_value = ir5.read_u16();

    // pc_mcu.printf("ir0 = %d\n",   ir0_value);
    // pc_mcu.printf("ir1 = %d\n",   ir1_value);
    // pc_mcu.printf("ir2 = %d\n",   ir2_value);
    // pc_mcu.printf("ir3 = %d\n",   ir3_value);
    // pc_mcu.printf("ir4 = %d\n",   ir4_value);
    // pc_mcu.printf("ir5 = %d\n\n", ir5_value);

    distance_0 = PSD0.getDistance();
    distance_1 = PSD1.getDistance();
    distance_2 = PSD2.getDistance();

    // pc_mcu.printf("d0 = %f\n", distance_0);
    // pc_mcu.printf("d1 = %f\n", distance_1);
    // pc_mcu.printf("d2 = %f\n\n", distance_2);
}

void velocity(){
    motorR = rspd;
    motorL = lspd;
}

void stop(){
    dirR = 0;
    dirL = 1;
    rspd = 0.0f;
    lspd = 0.0f;
}

void drive(){
    dirR = 1;
    dirL = 1;
    rspd = 0.30f;
    lspd = 0.30f;
}

void turnR(){
    dirR = 0;
    dirL = 1;
    rspd = 0.18f;
    lspd = 0.18f;
}

void turnL(){
    dirR = 1;
    dirL = 0;
    rspd = 0.18f;
    lspd = 0.18f;
}

void back(){
    dirR = 0;
    dirL = 0;
    rspd = 0.18f;
    lspd = 0.18f;
}

void go_battle(){
    dirR = 1;
    dirL = 1;
    rspd = 0.70f;
    lspd = 0.70f;
}

void ir_battle(){
    if(back_mode == 1){
        back();
        if((ir0_value > 40000 && ir1_value > black) && (ir5_value > black && ir2_value > black)){
            back_mode = 0;
        }
    }
    else if(ir0_value > black || ir1_value > black){
        back_mode = 0;
        dirR = 1;
        dirL = 1;
        rspd = 1.00f;
        lspd = 1.00f;
    }
    else if(ir0_value < black && ir1_value < black){
        dirR = 1;
        dirL = 1;
        // if(ir2_value < black && ir5_value > black && ir3_value > black && ir4_value > black){
        //     rspd = 0.95f;
        //     lspd = lspd + 0.01f;
        //     if(lspd >= 1.00f) lspd = 1.00f;
        // }
        // else if (ir2_value > black && ir5_value < black && ir3_value > black && ir4_value > black){
        //     rspd = rspd + 0.01f;
        //     lspd = 0.95f;
        //     if(rspd >= 1.00f) rspd = 1.00f;
        // }
        // else if(ir2_value < black && ir5_value < black && ir3_value > black && ir4_value > black){
        //     rspd = 1.00f;
        //     lspd = 1.00f;
        // }
        // else if((ir2_value < black && ir5_value < black) && (ir3_value < black || ir4_value < black)){
        //     back_mode = 1;
        // }
        if(ir5_value > black && ir2_value < black){
            rspd = 0.95f;
            lspd = 1.00f;
        }
        else if(ir5_value < black && ir2_value > black){
            rspd = 1.00f;
            lspd = 0.95f;
        }
        else if(ir5_value < black && ir2_value < black){
            back_mode = 1;
        }
    }
}

void left_area(){
    dirL = 1;
    dirR = 1;
    lspd = 0.95f;
    rspd = rspd + 0.01f;
    if(rspd >= 1.00f) rspd = 1.00f;
}

void right_area(){
    dirL = 1;
    dirR = 1;
    lspd = lspd + 0.01f;
    rspd = 0.95f;
    if(lspd >= 1.00f) lspd = 1.00f;
}

void two_area(){    //go_battle과 동일한 pwm이라서 추후에 pwm수정
    dirL = 1;
    dirR = 1;
    rspd = 1.00;
    lspd = 1.00f;
}

void ir_drive(){
    //원 둘레 주행 O
    if(ir0_value > black && ir1_value > black){
        dirR = 1;
        dirL = 1;
        if((ir5_value > black && ir4_value > black) && (ir2_value < black || ir3_value < black)){
            rspd = 0.15;
            lspd = 0.40; 
        }
        else if((ir5_value < black || ir4_value < black) && (ir2_value > black && ir3_value > black)){
            rspd = 0.40;
            lspd = 0.15; 
        }
        else{
            rspd = 0.40;
            lspd = 0.40;
        }
    }
    else if(ir0_value < black && ir1_value < black){
        turnR();
    }
    else if(ir0_value > black && ir1_value < black){ //오른쪽
        dirR = 1;
        dirL = 1;
        if((ir5_value > black && ir4_value > black) && (ir2_value < black || ir3_value < black)){
            rspd = 0.15;
            lspd = 0.40; 
        }
        else if((ir5_value < black || ir4_value < black) && (ir2_value > black && ir3_value > black)){
            rspd = 0.40;
            lspd = 0.15; 
        }
        else{
            rspd = 0.40;
            lspd = 0.40;
        }
    }
    else if(ir0_value < black && ir1_value > black){ //왼쪽
        dirR = 1;
        dirL = 1;
        if((ir5_value > black && ir4_value > black) && (ir2_value < black || ir3_value < black)){
            rspd = 0.15;
            lspd = 0.40; 
        }
        else if((ir5_value < black || ir4_value < black) && (ir2_value > black && ir3_value > black)){
            rspd = 0.40;
            lspd = 0.15; 
        }
        else{
            rspd = 0.40;
            lspd = 0.40;
        }
    }
}