// cpp 파일 : 정의

// [라이브러리]
#include "C:\Users\Hi\Mbed Programs\2_0_SRC_220305\BangLibrary\Preprocessing.h"  // 헤더파일 전처리
#include "C:\Users\Hi\Mbed Programs\2_0_SRC_220305\MPU9250\MPU9250.h"

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// 모드
DigitalOut led1(LED1);
InterruptIn btn(BUTTON1);

Ticker mode_tic;

int mode = 0;

int tot_mode = 0;
int tic_cnt = 0;
int tic_even_cnt = 0;

char pre_rotate_dir = 'n';
char rotate_dir = 'n';
char waiting_dir = 'l';

// 코드 위치 확인
int where = 0;

// thread
// Thread imu_th(osPriorityNormal);
Thread imu_th;

uint64_t Now_time, Work_time;

// Thread com_th;
// Mutex mutex;

// ir센서 + psd센서
AnalogIn irfl(PA_0);
AnalogIn irfr(PA_1);
AnalogIn irmr(PA_4);
AnalogIn irbr(PB_0);
AnalogIn irbl(PC_1);
AnalogIn irml(PC_0);
AnalogIn irfm(PB_1);
AnalogIn irmr2(PC_5);
AnalogIn irml2(PA_5);

AnalogIn psdf(PA_6);
GP2A psdb(PA_7, 10, 80, 22.5, 0.1606);

uint16_t ir_val[9];
// 0 : fl
// 1 : fr
// 2 : mr
// 3 : br
// 4 : bl
// 5 : ml
// 6 : fm
// 7 : mr2
// 8 : ml2
bool ir_WhCol[7];
// 0 : fl + fr
// 1 : bl + br
// 2 : ml + fl
// 3 : fr + mr
// 4 : mr + br
// 5 : bl + ml

double psdf_volts;
double psdf_val;
double psdb_val;
uint16_t black = 17500;
uint16_t tilt_black = 60000;

///////////////////////////////////////////////////
MPU9250 mpu9250;
float tilt_deg = 4.0;
float tilt_break_deg = 2.0;
extern float deltat;
extern float pitch_p;
extern float prev_y;
extern float SelfTest[6];
///////////////////////////////////////////////////

// AC서보 모터
PwmOut Servo(PA_8);

float ang = 90.0;
float inc = 3.5;
float small_inc = 3.5;
float big_inc = 6.7;
// float small_inc = 2.5;
// float big_inc = 4.2;

float angLL = 30;
float angML = 78;
float angMM = 90;
float angMR = 102;
float angRR = 150;

// DC 모터
DigitalOut DirL(PC_7);
DigitalOut DirR(PB_6);
PwmOut PwmL(PB_4);
PwmOut PwmR(PB_5);

double speed = 0;
double speedL = 0;
double speedR = 0;

// 통신
RawSerial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다
RawSerial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다
Serial blt(PC_10, PC_11, 9600);    // RawSerial 클래스에는 scanf가 정의되어있지 않다

// 통신 - ras_com
volatile bool All_move = false;
volatile bool gotPacket = false;
volatile float ras_data[3];
// ras_data[0] : 상대 방향 + 보임 유무
    // 왼쪽 : 0/11 ~ 4/11 (1 ~ 145)
    // 가운데 : 4/11 ~ 7/11 (146 ~ 254)
    // 오른쪽 : 7/11 ~ 11/11 (255 ~ 400)
    // 없음 : 999
// ras_data[1] : 상대 크기
    // 작음 : 1
    // 보통 : 2
    // 큼 : 3
    // 매우 큼 : 4
    // 매우 매우 큼 : 5
    // 매우 매우 매우 큼 : 6
    // 없음 : 9
// ras_data[2] : 빨간영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 빨간 영역 안 : 0(상대 가까울 때만 공격)
    // 빨간 영역 밖 : 1(무조건 공격)
// ras_data[3] : 파란영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 파란 영역 안 : 0(그대로 밀면 됨)
    // 파란 영역 밖 : 1(파란 영역 안으로 넣으면 best)
int pre_data0 = 9;
// 상대 최근 위치 왼쪽 : 1
// 상대 최근 위치 가운데 : 2
// 상대 최근 위치 오른쪽 : 3
// 상대 최근 위치 없음 : 9

int width = 400;
int width_l = width * 4.5 / 11.0; // 세부조정 필요!!!
int width_r = width * 6.5 / 11.0; // 세부조정 필요!!!

// 통신 - DC_chk
volatile bool gotPacket2 = false;
volatile float pc_data[3];

// 타이머
Timer control_tmr;
Timer brk_tmr;
Timer rotate_tmr;
Timer tilt_tmr;
Timer waiting_start_tmr;
Timer waiting_dir_tmr;
Timer com_check_tmr;

int turn_escape_time = 700000; // 세부조정 필요!!!
int back_escape_time = 600000; // 세부조정 필요!!!
int fight_back_escape_time = 450000; // 세부조정 필요!!!
int rotate_recog_time = 2000000; // 세부조정 필요!!!
int tilt_recog_time = 500000; // 세부조정 필요!!!
int waiting_start_time = 5000000; // 세부조정 필요!!!
int waiting_dir_time = 200000; // 세부조정 필요!!!
int com_check_time = 1500000; // 세부조정 필요!!!
double control_time = deltat * 1000000; // 세부조정 필요!!!
int imu_time = 7;

void imu_read(){
    // imu_th.set_priority(osPriorityNormal);
    while (true){
        // ThisThread::sleep_for(7);
        Now_time = rtos::Kernel::get_ms_count();
        mpu9250.get_data();
        Work_time = rtos::Kernel::get_ms_count();

        ThisThread::sleep_until(rtos::Kernel::get_ms_count() + (imu_time-(Work_time - Now_time)));

        // pc.printf("%d\n", fpclassify(pitch_p) == FP_NAN); // 확인용 코드

        if(fpclassify(pitch_p) == FP_NAN){
            blt.printf("imu_thread_reset"); // 확인용 코드

            mpu9250.initMPU9250();

            mpu9250.getAres(); // Get accelerometer sensitivity +-2g 4g 8g
            mpu9250.getGres(); // Get gyro sensitivity      250  500   1000

            pitch_p = 0;
            prev_y = 0;
        }
    }
}

// ir + psd 센서
void sensor_read(){
    ///////////////////////////////////////////////////
    // if(control_tmr.read_us() >= control_time){
    //     mpu9250.get_data();
    //     // pc.printf("control_time : %d \n", control_tmr.read_us()); // 확인용 코드
    //     // pc.printf("%f | %u | %u |\n", pitch_p, ir_val[7], ir_val[8]); // 확인용 코드

    //     control_tmr.reset();
    // }
    ///////////////////////////////////////////////////

    ir_val[0] = irfl.read_u16();
    ir_val[1] = irfr.read_u16();
    ir_val[2] = irmr.read_u16();
    ir_val[3] = irbr.read_u16();
    ir_val[4] = irbl.read_u16();
    ir_val[5] = irml.read_u16();
    ir_val[6] = irfm.read_u16();
    ir_val[7] = irmr2.read_u16();
    ir_val[8] = irml2.read_u16();

    psdf_volts = psdf.read() * 3.3;
    psdf_val = 13 * pow(psdf_volts, -1.0);
    if(psdf_val > 30) psdf_val = 30;

    psdb_val = psdb.getDistance();
}

void sensor_cal(){
    if(ir_val[0] < black && ir_val[1] < black){
        ir_WhCol[0] = true;
    }
    else ir_WhCol[0] = false;
    if(ir_val[3] < black && ir_val[4] < black){
        ir_WhCol[1] = true;
    }
    else ir_WhCol[1] = false;
    if(ir_val[5] < black && ir_val[0] < black){
        ir_WhCol[2] = true;
    }
    else ir_WhCol[2] = false;
    if(ir_val[1] < black && ir_val[2] < black){
        ir_WhCol[3] = true;
    }
    else ir_WhCol[3] = false;
    if(ir_val[2] < black && ir_val[3] < black){
        ir_WhCol[4] = true;
    }
    else ir_WhCol[4] = false;
    if(ir_val[4] < black && ir_val[5] < black){
        ir_WhCol[5] = true;
    }
    else ir_WhCol[5] = false;
}

void sensor_print(){
    pc.printf("mpu6050 pitch_p : %.1f\n", pitch_p); // 확인용 코드
    pc.printf("ir_val : | %u | %u | %u | %u | %u | %u | %u | %u | %u |\n", ir_val[0], ir_val[1], ir_val[2], ir_val[3], ir_val[4], ir_val[5], ir_val[6], ir_val[7], ir_val[8]); // 확인용 코드
    pc.printf("ir_WhCol : | %d | %d | %d | %d | %d | %d |\n", ir_WhCol[0], ir_WhCol[1], ir_WhCol[2], ir_WhCol[3], ir_WhCol[4], ir_WhCol[5]); // 확인용 코드
    pc.printf("psdf_val : | %lf |, psdb_val : | %lf |\n", psdf_val, psdb_val); // 확인용 코드
}

// AC 서보 모터
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servo_set(PwmOut &rc){
    Servo.period_ms(10);
    uint16_t pulseW = map<float>(ang, 180., 0., 500., 2600.);
    // uint16_t pulseW = map<float>(ang, 180., 0., 833., 2266.);
    rc.pulsewidth_us(pulseW);
}

void servo_move(PwmOut &rc){
    if(0 <= ras_data[0] && ras_data[0] < width/2) inc = map<float>(ras_data[0], width/2, 1.0, small_inc, big_inc);
    else if(width/2 <= ras_data[0] && ras_data[0] <= width) inc = map<float>(ras_data[0], width/2, width, small_inc, big_inc);

    // 중간 동작 : 화면 상대방 안보임
    if(ras_data[0] == 999){
        // pc.printf("1"); // 확인용 코드
        inc = big_inc;

        if(pre_data0 == 1) ang = ang - inc;
        else if(pre_data0 == 2) ang = ang + inc;
        else if(pre_data0 == 3) ang = ang + inc;
        else ang = ang + inc;
    }

    // 중간 동작 : 화면 상대방 보임
    else if(ras_data[0] < width_l){
        // pc.printf("2"); // 확인용 코드
        ang = ang - inc;
        pre_data0 = 1;
    }
    else if(width_l <= ras_data[0] && ras_data[0] < width_r){
        // pc.printf("3"); // 확인용 코드
        ang = ang;
        pre_data0 = 2;
    }
    else if(width_r <= ras_data[0]){
        // pc.printf("4"); // 확인용 코드
        ang = ang + inc;
        pre_data0 = 3;
    }

    // 중간동작 : 서보모터 각도 제한
    if (ang > 180.f){
        ang = 180.0;
    }
    else if (ang < 0.0f){
        ang = 0.0;
    }
    uint16_t pulseW = map<float>(ang, 180., 0., 500., 2600.);
    // uint16_t pulseW = map<float>(ang, 180., 0., 833., 2266.);
    rc.pulsewidth_us(pulseW);

    // pc.printf("%f \n", ang); // 확인용 코드
}

void servo_chk(PwmOut &rc){
    ang+=inc;
    if (ang > 180.f || ang < 0.f){
        inc = -inc;
    }
    uint16_t pulseW = map<float>(ang, 0., 180., 500., 2600.);
    rc.pulsewidth_us(pulseW);
}

// DC 모터
void DC_set(){
    PwmL.period_us(66); // 주파수 : 15kHz(모터드라이버 : 최대 주파수 20kHz)
    PwmR.period_us(66); // 주파수 : 15kHz(모터드라이버 : 최대 주파수 20kHz)
}

void DC_move(float _PwmL, float _PwmR){
    if(_PwmL < 0) DirL = 0;
    else DirL = 1;

    if(_PwmR < 0) DirR = 0;
    else DirR = 1;

    PwmL = abs(_PwmL);
    PwmR = abs(_PwmR);
}

void DC_chk(){
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
            gotPacket2 = true;
        }
        else{
            // pc.printf("byteIn == '나머지' : %c \n", byteIn2); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
            pc_serialInBuffer2[buff_cnt2++]=byteIn2;
        }

        if(gotPacket2) {
            gotPacket2 = false;
            // pc.printf("PWM = %.3f \n\r", pc_data[0]); // 확인용 코드
        }
    }

    if (0 <= pc_data[0] && pc_data[0] <= 95){
        PwmL = 0.01 * pc_data[0];
        PwmR = 0.01 * pc_data[0];
    }

    DirR = 1;
    DirL = 1;
}

// 통신(pc, ras, mbed)
void in_SerialRx(){ // interrupt 전용
    static char ras_serialInBuffer[32];
    static int ras_data_cnt = 0, ras_buff_cnt = 0;

    if(ras.readable()){
        // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
        char byteIn = ras.getc();
        // ',' : 단어 자르기
        if(byteIn == ','){
            // pc.printf("byteIn == ',' : %c\n", byteIn); // 확인용 코드
            ras_serialInBuffer[ras_buff_cnt] = '\0';
            ras_data[ras_data_cnt++] = atof(ras_serialInBuffer);
            ras_buff_cnt = 0;
        }
        // '/' : 시리얼 완료
        else if(byteIn=='/'){
            // pc.printf("byteIn == '/' : %c \n", byteIn); // 확인용 코드
            ras_serialInBuffer[ras_buff_cnt] = '\0';
            ras_data[ras_data_cnt]=atof(ras_serialInBuffer);
            ras_buff_cnt=0; ras_data_cnt = 0;
            gotPacket = true;
        }
        // 이외 : 시리얼 저장
        else{
            // pc.printf("byteIn == '나머지' : %c\n", byteIn); // 확인용 코드
            ras_serialInBuffer[ras_buff_cnt++] = byteIn;
        }
    }
}

void in_SerialRx_main(){ // interrupt 전용
    // pc.printf("pc 연결 OK \n"); // pc, mbed 통신 출력
    if(gotPacket) {
        gotPacket = false;
        All_move = true;
        // pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]); // 확인용 코드
    }
}

void th_SerialRx(){ // thread 전용
    static char ras_serialInBuffer[32];
    static int ras_data_cnt = 0, ras_buff_cnt = 0;

    while(true){
        // mutex.lock();
        if(ras.readable()){
            // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
            char byteIn = ras.getc();
            
            // ',' : 단어 자르기
            if(byteIn == ','){
                // pc.printf("byteIn == ',' : %c\n", byteIn); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
                ras_serialInBuffer[ras_buff_cnt]='\0';
                ras_data[ras_data_cnt++]=atof(ras_serialInBuffer);
                ras_buff_cnt = 0;
            }
            // '/' : 시리얼 완료
            else if(byteIn=='/'){
                // pc.printf("byteIn == '/' : %c \n", byteIn); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
                ras_serialInBuffer[ras_buff_cnt] = '\0';
                ras_data[ras_data_cnt]=atof(ras_serialInBuffer);
                ras_buff_cnt=0; ras_data_cnt=0;
                gotPacket = true;
            }
            // 이외 : 시리얼 저장
            else{
                // pc.printf("byteIn == '나머지' : %c\n", byteIn); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
                ras_serialInBuffer[ras_buff_cnt++]=byteIn;
            }
        }

        if(gotPacket) {
            gotPacket = false;
            // pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]); // 확인용 코드
        }

        // mutex.unlock();
    }
}

// 기본 움직임
void red_in_servo_left_can_see_move(){
    // pc.printf("상대 보임 \n"); // 확인용 코드

    if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
        if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
            if(angLL < ang){ // 서보 보통 왼쪽
                back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                where = 1;
            }
            else if(ang <= angLL){ // 서보 매우 왼쪽
                back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);
                where = 2;
            }
        }
        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
            if(angLL < ang){ // 서보 보통 왼쪽
                speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                speedR = 0.60;
                where = 3;
            }
            else if(ang <= angLL){ // 서보 매우 왼쪽
                speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                speedR = 0.50;
                where = 4;
            }
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.45, -0.30);
        where = 5;
    }
    else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        turn_tmr_move(&brk_tmr, &turn_escape_time, &ir_val[6], black, &ras_data[1], -0.45, 0.45);
        where = 6;
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 ???
        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 오른쪽 전진
            speedL = 0.60; speedR = 0.10;
            where = 7;
        }
        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.45, -0.30);
            where = 8;
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 왼쪽 전진
            // speedL = 0.30; speedR = 0.60;
            speedL = 0.27; speedR = 0.60;
            where = 9;
        }
        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.45, -0.30);
            where = 10;
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 오른쪽 전진
            // speedL = 0.60; speedR = 0.30;
            speedL = 0.60; speedR = 0.27;
            where = 11;
        }
        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.45, -0.30);
            where = 12;
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.45, -0.30);
        where = 13;
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.45, -0.30);
        where = 14;
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 매우 왼쪽 전진 ???
        speedL = 0.10; speedR = 0.60; // 0.225;
        where = 15;
    }
    else if(
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 모두 검은색 : 자유롭게 공격
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 : 자유롭게 공격
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 오른쪽 뒷 바퀴 : 자유롭게 공격
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true) // 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 자유롭게 공격
    ){
        if(ang <= angLL){
            speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
            speedR = 0.50;
            where = 16;
        }
        else if(ang > angLL){
            speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
            speedR = 0.60;
            where = 17;
        }
    }
    else{ // 그 외 : 왼쪽 전진
        speedL = 0.27; speedR = 0.60;
        where = 18;
    }
}

void red_in_servo_mid_can_see_move(){
    // pc.printf("상대 보임 \n"); // 확인용 코드

    if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
        if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.70, -0.70);
            where = 21;
        }
        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 빠른 자유롭게 공격
            speedL = 0.60; speedR = 0.60;
            where = 22;
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.35, -0.35);
        where = 23;
    }
    else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞
        if(ang >= 90){ // 서보 조금이라도 오른쪽 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            turn_tmr_move(&brk_tmr, &turn_escape_time, &ir_val[6], black, &ras_data[1], 0.45, -0.45);
            where = 24;
        }
        else if(ang < 90){ // 서보 조금이라도 왼쪽 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            turn_tmr_move(&brk_tmr, &turn_escape_time, &ir_val[6], black, &ras_data[1], -0.45, 0.45);
            where = 25;
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 오른쪽 전진
            speedL = 0.60; speedR = 0.10;
            where = 26;
        }
        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.35, -0.35);
            where = 27;
        }
    }
    else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 왼쪽 전진
            speedL = 0.10; speedR = 0.60;
            where = 28;
        }
        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.35, -0.35);
            where = 29;
        }
    }
    else if(
        (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
        (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
    ){
        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 전진
            speedL = 0.35; speedR = 0.35;
            where = 30;
        }
        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.35, -0.35);
            where = 31;
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.35, -0.35);
        where = 32;
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.35, -0.35);
        where = 33;
    }
    else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 왼쪽 전진
        speedL = 0.27; speedR = 0.60;
        where = 34;
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 오른쪽 전진
        speedL = 0.60; speedR = 0.27;
        where = 35;
    }
    else if(
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 모두 검은색 : 자유롭게 공격
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 : 자유롭게 공격
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 오른쪽 뒷 바퀴 : 자유롭게 공격
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true) // 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 자유롭게 공격
    ){
        speedL = 0.60; speedR = 0.60;
        where = 36;
    }
    else{ // 그 외 : 전진
        speedL = 0.60; speedR = 0.60;
        where = 37;
    }
}

void red_in_servo_right_can_see_move(){
    // pc.printf("상대 보임 \n"); // 확인용 코드

    if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
        if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
            if(ang < angRR){ // 서보 보통 오른쪽
                back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                where = 41;
            }
            else if(angRR <= ang){ // 서보 매우 오른쪽
                back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                where = 42;
            }
        }
        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
            if(angRR <= ang){
                speedL = 0.50;
                speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.18);
                where = 43;
            }
            else if(ang < angRR){
                speedL = 0.60;
                speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                where = 44;
            }
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 왼쪽 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.30, -0.45);
        where = 45;
    }
    else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        turn_tmr_move(&brk_tmr, &turn_escape_time, &ir_val[6], black, &ras_data[1], 0.45, -0.45);
        where = 46;
    }
    else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 왼쪽 전진
            speedL = 0.10; speedR = 0.60;
            where = 47;
        }
        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.30, -0.45);
            where = 48;
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 왼쪽 전진 -> 왼쪽 전진
            // speedL = 0.30; speedR = 0.60;
            speedL = 0.27; speedR = 0.60;
            where = 49;
        }
        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.30, -0.45);
            where = 50;
        }
    }
    else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 오른쪽 전진 -> 오른쪽 전진
            // speedL = 0.60; speedR = 0.30;
            speedL = 0.60; speedR = 0.27;
            where = 51;
        }
        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.30, -0.45);
            where = 52;
        }
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.30, -0.45);
        where = 53;
    }
    else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.30, -0.45);
        where = 54;
    }
    else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 매우 오른쪽 전진
        speedL = 0.60; speedR = 0.10; // 0.225;
        where = 55;
    }
    else if(
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 모두 검은색 : 자유롭게 공격
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 : 자유롭게 공격
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 오른쪽 뒷 바퀴 : 자유롭게 공격
        (ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true) // 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 자유롭게 공격
    ){
        if(angRR <= ang){
            speedL = 0.60;
            speedR = -map<float>(ang, 180.0, angRR, 0.60, 0.18);
            where = 56;
        }
        else if(ang < angRR){
            speedL = 0.60;
            speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
            where = 57;
        }
    }
    else{ // 그 외 : 오른쪽 전진
        speedL = 0.60; speedR = 0.27;
        where = 58;
    }
}

void red_out_servo_all_can_see_move(){
    // pc.printf("상대 보임 \n"); // 확인용 코드

    if(ang <= angML){ // 서보 왼쪽
        if(angLL < ang){ // 서보 보통 왼쪽
            if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                    back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                    where = 61;

                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        normal_tmr_move(&brk_tmr, &fight_back_escape_time, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                        where = 62;
                    }
                }
                else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                    speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                    speedR = 0.60;
                    where = 63;
                }
            }
            else if(
                (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
            ){
                back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                where = 64;

                if(ras_data[1] == 5 || ras_data[1] == 6){
                    normal_tmr_move(&brk_tmr, &fight_back_escape_time, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                    where = 65;
                }
            }
            else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                    speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                    speedR = 0.60;
                    where = 66;
                }
                else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                    back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                    where = 67;

                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        normal_tmr_move(&brk_tmr, &fight_back_escape_time, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                        where = 68;
                    }
                }
            }
            else if(
                (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
                (ir_val[0] < black && ir_val[1] > black && ir_val[2] > black && ir_val[3] > black && ir_val[4] < black && ir_val[5] > black && ir_val[6] > black) || // ir 왼쪽 앞 + ir 왼쪽 뒤
                (ir_val[0] > black && ir_val[1] > black && ir_val[2] > black && ir_val[3] > black && ir_val[4] > black && ir_val[5] < black && ir_val[6] > black) // ir 왼쪽 가운데
            ){
                if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        rotate_dir = 'l';
                    }

                    if(rotate_tmr.read_us() < rotate_recog_time){
                        speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                        speedR = 0.60;
                        where = 69;
                    }
                    else if(rotate_tmr.read_us() > rotate_recog_time){
                        // if(rotate_dir == pre_rotate_dir){
                            // speedL = 0.60; speedR = 0.60;
                            // speedL = 0.60; speedR = 0.40;
                            // speedL = 0.60; speedR = 0.30;
                            where = 69;
                        // }
                        // else{
                        //     tmr_reset(&rotate_tmr);
                        // }
                    }
                }
                else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                    back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                    where = 70;

                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        normal_tmr_move(&brk_tmr, &fight_back_escape_time, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                        where = 71;
                    }
                }
            }
            else{ // 그 외 : 자유롭게 공격
                speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                speedR = 0.60;
                where = 72;
            }
        }
        else if(ang <= angLL){ // 서보 매우 왼쪽
            if(ras_data[1] == 6){ // 화면 원통 매우 매우 매우 큼
                speedL = -1.0; speedR = 1.0;
                where = 73;
            }
            else if(ras_data[1] != 6){ // 화면 원통 매우 매우 매우 크지 않음
                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);
                        where = 74;

                        if(ras_data[1] == 5 || ras_data[1] == 6){
                            normal_tmr_move(&brk_tmr, &fight_back_escape_time, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);
                            where = 75;
                        }
                    }
                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                        speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                        speedR = 0.50;
                        where = 76;
                    }
                } 
                else if(
                    (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                ){
                    back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);
                    where = 77;

                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        normal_tmr_move(&brk_tmr, &fight_back_escape_time, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);
                        where = 78;
                    }
                }
                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                        speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                        speedR = 0.50;
                        where = 79;
                    }
                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);
                        where = 80;

                        if(ras_data[1] == 5 || ras_data[1] == 6){
                            normal_tmr_move(&brk_tmr, &fight_back_escape_time, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);
                            where = 81;
                        }
                    }
                }
                else{ // 그 외 : 자유롭게 공격
                    speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                    speedR = 0.50;
                    where = 82;
                }
            }
        }
    }
    else if(angML < ang && ang < angMR){ // 서보 중간
        if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
            if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -0.50);
                where = 91;

                if(ras_data[1] == 5 || ras_data[1] == 6){
                    normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -0.50);
                    where = 92;
                }
            }
            else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                speedL = 0.60; speedR = 0.60;
                where = 93;
            }
        }
        else if(
            (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
            (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
            (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
            (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
        ){
            back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -0.50);
            where = 94;

            if(ras_data[1] == 5 || ras_data[1] == 6){
                normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -0.50);
                where = 95;
            }
        }
        else if(
            (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴
            (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) // 오른쪽 앞 바퀴
        ){
            if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                speedL = 0.60; speedR = 0.60;
                where = 96;
            }
            else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -0.50);
                where = 97;

                if(ras_data[1] == 5 || ras_data[1] == 6){
                    normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -0.50);
                    where = 98;
                }
            }
        }
        else{ // 그 외 : 자유롭게 공격
            speedL = 0.60; speedR = 0.60;
            where = 99;
        }
    }
    else if(angMR <= ang){ // 서보 오른쪽
        if(ang < angRR){ // 서보 보통 오른쪽
            if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                    back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                    where = 101;

                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                        where = 102;
                    }
                }
                else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                    speedL = 0.60;
                    speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                    where = 103;
                }
            }
            else if(
                (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
            ){
                back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                where = 104;

                if(ras_data[1] == 5 || ras_data[1] == 6){
                    normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                    where = 105;
                }
            }
            else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                    speedL = 0.60;
                    speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                    where = 106;
                }
                else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                    back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                    where = 107;

                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                        where = 108;
                    }
                }
            }
            else if(
                (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
                (ir_val[0] > black && ir_val[1] < black && ir_val[2] > black && ir_val[3] < black && ir_val[4] > black && ir_val[5] > black && ir_val[6] > black) || // ir 오른쪽 앞 + ir 오른쪽 뒤
                (ir_val[0] > black && ir_val[1] > black && ir_val[2] < black && ir_val[3] > black && ir_val[4] > black && ir_val[5] > black && ir_val[6] > black) // ir 오른쪽 가운데
            ){
                if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        rotate_dir = 'r';
                    }

                    if(rotate_tmr.read_us() < rotate_recog_time){
                        speedL = 0.60;
                        speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                        where = 109;
                    }
                    else if(rotate_tmr.read_us() > rotate_recog_time){
                        // if(rotate_dir == pre_rotate_dir){
                            // speedL = 0.60; speedR = 0.50;
                            // speedL = 0.40; speedR = 0.60;
                            speedL = 0.30; speedR = 0.60;
                            where = 110;
                        // }
                        // else{
                        //     tmr_reset(&rotate_tmr);
                        // }
                    }
                }
                else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                    back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                    where = 111;

                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        normal_tmr_move(&brk_tmr, &fight_back_escape_time, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                        where = 112;
                    }
                }
            }
            else{ // 그 외 : 자유롭게 공격
                speedL = 0.60;
                speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                where = 113;
            }
        }
        else if(angRR <= ang){ // 서보 매우 오른쪽
            if(ras_data[1] == 6){ // 화면 원통 매우 매우 매우 큼
                speedL = 1.0; speedR = -1.0;
                where = 114;
            }
            else if(ras_data[1] != 6){ // 화면 원통 매우 매우 매우 크지 않음
                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                        where = 115;

                        if(ras_data[1] == 5 || ras_data[1] == 6){
                            normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                            where = 116;
                        }
                    }
                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                        speedL = 0.50;
                        speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                        where = 117;
                    }
                }
                else if(
                    (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                ){
                    back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                    where = 118;

                    if(ras_data[1] == 5 || ras_data[1] == 6){
                        normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                        where = 119;
                    }
                }
                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                        speedL = 0.50;
                        speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                        where = 120;
                    }
                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                        where = 130;

                        if(ras_data[1] == 5 || ras_data[1] == 6){
                            normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                            where = 131;
                        }
                    }
                }
                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                        speedL = 0.50;
                        speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                        where = 132;
                    }
                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                        where = 133;

                        if(ras_data[1] == 5 || ras_data[1] == 6){
                            normal_tmr_move(&brk_tmr, &fight_back_escape_time, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                            where = 134;
                        }
                    }
                }
                else{ // 그 외 : 자유롭게 공격
                    speedL = 0.50;
                    speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                    where = 135;
                }
            }
        }
    }
}

// 타이머 움직임
void normal_tmr_move(Timer* _tmr, int* _time, double _speedL, double _speedR){
    _tmr->start(); // _tmr->start(); = *_tmr.start(); // 타이머 시작
    while(_tmr->read_us() < *_time){ // 타이머 일정 시간 이하 : 특정 움직임 유지
        speedL = _speedL; speedR = _speedR;

        whl_bundle();
    }
    _tmr->reset(); // 타이머 리셋
    _tmr->stop();
}

void turn_tmr_move(Timer* _tmr, int* _time, uint16_t* _while_brk_sensor, uint16_t _sensor_val, volatile float* _com_data, double _speedL, double _speedR){
    while(*_while_brk_sensor < _sensor_val){
        speedL = _speedL; speedR = _speedR;

        whl_bundle();
        if(_tmr->read_us() > *_time){
            break;
        }
        if(int(*_com_data) == 5 || int(*_com_data) == 6){
            break;
        }
        if(
            (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) || // 모든 바퀴
            (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴
            // (ir_WhCol[0] == true && ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + ir 오른쪽 앞 O
            // (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 오른쪽 앞 바퀴 + ir 왼쪽 앞 O
            (ir_WhCol[0] == true && ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + ir 오른쪽 앞 O
            (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 + ir 왼쪽 앞 O
            (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴
            (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
        ){
            break;
        }
    }
    _tmr->reset();
    _tmr->stop();
}

void rotate_tmr_judgment(){
    if(
        (rotate_dir == 'l' && rotate_dir == pre_rotate_dir) || // 원 회전 상황 O and 이전 회전 방향과 현재 회전 방향 같음 : 타이머 시작
        (rotate_dir == 'r' && rotate_dir == pre_rotate_dir)
    ){
        rotate_tmr.start();
    }
    else{ // 원 회전 상황 X or 이전 회전 방향과 현재 회전 방향 다름 : 타이머 리셋
        tmr_reset(&rotate_tmr);
    }
}

void tilt_tmr_judgment(){
    if(pitch_p > tilt_deg){ // IMU 일정 각도 이상 : 타이머 시작
        tilt_tmr.start();
    }
    else{ // IMU 일정 각도 이하 : 타이머 리셋
        tmr_reset(&tilt_tmr);
    }
}

void tilt_tmr_move(){
    blt.printf("| %u | %u | %u | %u |\n", ir_val[7]/1000, ir_val[3]/1000, ir_val[4]/1000, ir_val[8]/1000);
    if(ang <= angML){ // 서보 왼쪽
        if(angLL < ang){ // 서보 보통 왼쪽
            // 항상 : 자유롭게 공격
            if(
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 전부 검정 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] > tilt_black) || // ir 왼쪽 뒤 색 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 뒤 색 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 왼쪽 뒤 색 + 오른쪽 뒤 색 : 자유로운 공격
            ){
                red_out_servo_all_can_see_move();
            }
            // 상대 좌측 : 자유롭게 공격
            else if(
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 : 자유로운 공격
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 : 자유로운 공격
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 + 왼쪽 뒤 색 : 자유로운 공격
            ){
                red_out_servo_all_can_see_move();
            }
            // 항상 : 빠른 오른쪽 후진
            else if(
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
            ){
                back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -map<float>(ang, angML, angLL, 0.60, 0.85) * 1.1760, -0.50 * 1.1760);
            }
            // 상대 좌측 : 빠른 오른쪽 후진
            else if(
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
            ){
                back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -map<float>(ang, angML, angLL, 0.60, 0.85) * 1.1760, -0.50 * 1.1760);
            }
            // 존재하지 않는 경우 : 자유롭게 공격
            else{
                red_out_servo_all_can_see_move();
            }
        }
        else if(ang <= angLL){ // 서보 매우 왼쪽
            // 항상 : 자유롭게 공격
            if(
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 전부 검정 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] > tilt_black) || // ir 왼쪽 뒤 색 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 뒤 색 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 왼쪽 뒤 색 + 오른쪽 뒤 색 : 자유로운 공격
            ){
                red_out_servo_all_can_see_move();
            }
            // 상대 좌측 : 자유롭게 공격
            else if(
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 : 자유로운 공격
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 : 자유로운 공격
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 + 왼쪽 뒤 색 : 자유로운 공격
            ){
                red_out_servo_all_can_see_move();
            }
            // 항상 : 빠른 오른쪽 후진
            else if(
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
            ){
                back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -map<float>(ang, angLL, 0.0, 0.85, 0.95) * 1.0525, -0.50 * 1.0525);
            }
            // 상대 좌측 : 빠른 오른쪽 후진
            else if(
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 오른쪽 후진
            ){
                back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -map<float>(ang, angLL, 0.0, 0.85, 0.95) * 1.0525, -0.50 * 1.0525);
            }
            // 존재하지 않는 경우 : 자유롭게 공격
            else{
                red_out_servo_all_can_see_move();
            }
        }
    }
    else if(angML < ang && ang < angMR){ // 서보 중간
        // 항상 : 자유롭게 공격
        if(
            (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 전부 검정 : 자유로운 공격
            (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] > tilt_black) || // ir 왼쪽 뒤 색 : 자유로운 공격
            (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 뒤 색 : 자유로운 공격
            (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 왼쪽 뒤 색 + 오른쪽 뒤 색 : 자유로운 공격
        ){
            red_out_servo_all_can_see_move();
        }
        // 상대 전방 : 자유롭게 공격
        else if(
            (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 : 자유로운 공격
            (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 : 자유로운 공격
            (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 왼쪽 가운데 색 : 자유로운 공격
            (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 왼쪽 뒤 색 + 왼쪽 가운데 색 : 자유로운 공격
        ){
            red_out_servo_all_can_see_move();
        }
        // 항상 : 빠른 후진
        else if(
            (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 가운데 색 : 빠른 후진
            (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 후진
            (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 후진
            (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 후진
        ){
            back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -1.0, -1.0);
        }
        // 상대 전방 : 빠른 후진
        else if(
            (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 후진
            (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 + 왼쪽 뒤 색 : 빠른 후진
        ){
            back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -1.0, -1.0);
        }
        // 존재하지 않는 경우 : 자유롭게 공격
        else{
            red_out_servo_all_can_see_move();
        }
    }
    else if(angMR <= ang){ // 서보 오른쪽
        if(ang < angRR){ // 서보 보통 오른쪽
            // 항상 : 자유롭게 공격
            if(
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 전부 검정 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] > tilt_black) || // ir 왼쪽 뒤 색 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 뒤 색 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 왼쪽 뒤 색 + 오른쪽 뒤 색 : 자유로운 공격
            ){
                red_out_servo_all_can_see_move();
            }
            // 상대 우측 : 자유롭게 공격
            else if(
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 : 자유로운 공격
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 : 자유로운 공격
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 + 왼쪽 뒤 색 : 자유로운 공격
            ){
                red_out_servo_all_can_see_move();
            }
            // 항상 : 빠른 왼쪽 후진
            else if(
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
            ){
                back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -0.50 * 1.1760, -map<float>(ang, angMR, angRR, 0.60, 0.85) * 1.1760);
            }
            // 상대 우측 : 빠른 왼쪽 후진
            else if(
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
            ){
                back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -0.50 * 1.1760, -map<float>(ang, angMR, angRR, 0.60, 0.85) * 1.1760);
            }
            // 존재하지 않는 경우 : 자유롭게 공격
            else{
                red_out_servo_all_can_see_move();
            }
        }
        else if(angRR <= ang){ // 서보 매우 오른쪽
            // 항상 : 자유롭게 공격
            if(
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 전부 검정 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] > tilt_black) || // ir 왼쪽 뒤 색 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 뒤 색 : 자유로운 공격
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 왼쪽 뒤 색 + 오른쪽 뒤 색 : 자유로운 공격
            ){
                red_out_servo_all_can_see_move();
            }
            // 상대 우측 : 자유롭게 공격
            else if(
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 : 자유로운 공격
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] > tilt_black) || // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 : 자유로운 공격
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] > tilt_black) // ir 오른쪽 가운데 색 + 오른쪽 뒤 색 + 왼쪽 뒤 색 : 자유로운 공격
            ){
                red_out_servo_all_can_see_move();
            }
            // 항상 : 빠른 왼쪽 후진
            else if(
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] < tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 오른쪽 가운데 + 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
            ){
                back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -0.50 * 1.0525, -map<float>(ang, angRR, 180.0, 0.85, 0.95) * 1.0525);
            }
            // 상대 우측 : 빠른 왼쪽 후진
            else if(
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] > black && ir_val[8] < tilt_black) || // ir 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] > tilt_black && ir_val[3] > black && ir_val[4] < black && ir_val[8] < tilt_black) || // ir 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
                (ir_val[7] > tilt_black && ir_val[3] < black && ir_val[4] < black && ir_val[8] < tilt_black) // ir 오른쪽 뒤 색 + 왼쪽 뒤 색 + 왼쪽 가운데 색 : 빠른 왼쪽 후진
            ){
                back_tmr_move<float>(&brk_tmr, &back_escape_time, &pitch_p, ">", tilt_break_deg, -0.50 * 1.0525, -map<float>(ang, angRR, 180.0, 0.85, 0.95) * 1.0525);
            }
            // 존재하지 않는 경우 : 자유롭게 공격
            else{
                red_out_servo_all_can_see_move();
            }
        }
    }
}

void tmr_reset(Timer* _tmr){ // 타이머 리셋
    _tmr->reset();
    _tmr->stop();
}

// 버튼 + LED
void btn_flip(){
    tot_mode++;
}

void led_flash(){
    tic_cnt++;
    if(tic_cnt % 2 == 0){
        tic_even_cnt++;
        if(tot_mode > (tic_even_cnt % 10)){
            led1 = 1;
        }
    }
    else led1 = 0;
}

// while문 필수 함수
void whl_bundle(){
    in_SerialRx_main(); // interrupt 전용

    sensor_read();
    sensor_cal();
    // sensor_print(); // 확인용 코드

    if(All_move == true){ // 통신 받음
        servo_move(Servo);
        DC_move(speedL, speedR);

        // blt.printf("b%d\n", brk_tmr.read_ms()); // 확인용 코드

        blt.printf("w%d\n", where); // 확인용 코드
        // if(where == 46 || where == 24 || where == 6) blt.printf("---%d---\n", where);
        // blt.printf("i%d\n", ir_val[6]);
        
        // blt.printf("r%d\n", rotate_tmr.read_ms()); // 확인용 코드
        // blt.printf("%d, %.1f, %.2f, %.2f, %d, %d\n", mode, pitch_p, speedL, speedR, rotate_tmr.read_us(), tilt_tmr.read_us()); // 확인용 코드
        // blt.printf("%.2f, %.2f, %d, %.1f\n", speedL, speedR, rotate_tmr.read_us(), pitch_p); // 확인용 코드
        // blt.printf("%.2f\n", pitch_p); // 확인용 코드
        // blt.printf("%d\n", tilt_tmr.read_us()); // 확인용 코드
        // blt.printf("%d\n", brk_tmr.read_us()); // 확인용 코드
        // blt.printf("ir_val : | %u | %u | %u | %u | %u | %u | %u |\n", ir_val[0]/1000, ir_val[1]/1000, ir_val[2]/1000, ir_val[3]/1000, ir_val[4]/1000, ir_val[5]/1000, ir_val[6]/1000); // 확인용 코드
    }
    All_move = false;
    // all_print(); // 확인용 코드
}

// print
void all_print(){
    pc.printf("ㅡㅡㅡㅡㅡ전략 모드ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("mode = %d \n", tot_mode); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ모드ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("mode = %d \n", mode); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ통신ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("ras_data = %.3f, %.3f, %.3f\n", ras_data[0], ras_data[1], ras_data[2]); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ센서ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    sensor_print(); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ모터ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("ang = %f, inc = %f, speedL = %f, speedR = %f\n", ang, inc, speedL, speedR); // 확인용 코드
    pc.printf("=======================\n"); // 확인용 코드
}