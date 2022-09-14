// cpp 파일 : 정의

// [라이브러리]
#include "C:\Users\Hi\Mbed Programs\2_0_SRC_220305\BangLibrary\Preprocessing.h"  // 헤더파일 전처리

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// 모드
DigitalOut led1(LED1);
InterruptIn btn(BUTTON1);

Ticker mode_tic;

int mode = 0;

int tot_mode = 0;
int tic_cnt = 0;
int tic_even_cnt = 0;

// thread
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

AnalogIn psdf(PA_6);
GP2A psdb(PA_7, 10, 80, 22.5, 0.1606);

uint16_t ir_val[7];
// 0 : fl
// 1 : fr
// 2 : mr
// 3 : br
// 4 : bl
// 5 : ml
// 6 : fm
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
uint16_t black = 10000;

// AC서보 모터
PwmOut Servo(PA_8);

float ang = 90.0;
float inc = 3.5;
float small_inc = 3.5;
float big_inc = 5.5;
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
Timer brk_tmr;
Timer rotate_tmr;

// ///////////////////////////////////////////////////
// Timer control_tmr;
// ///////////////////////////////////////////////////

int turn_escape_time = 1000000; // 세부조정 필요!!!
int back_escape_time = 1000000; // 세부조정 필요!!!
int fight_back_escape_time = 500000; // 세부조정 필요!!!
int rotate_back_escape_time = 3000000; // 세부조정 필요!!!
int tilt_back_escape_time = 1500000; // 세부조정 필요!!!

// ///////////////////////////////////////////////////
// double control_time = 0;
// ///////////////////////////////////////////////////

///////////////////////////////////////////////////
extern float pitch_p;
///////////////////////////////////////////////////

// ir + psd 센서
void sensor_read(){
    ir_val[0] = irfl.read_u16();
    ir_val[1] = irfr.read_u16();
    ir_val[2] = irmr.read_u16();
    ir_val[3] = irbr.read_u16();
    ir_val[4] = irbl.read_u16();
    ir_val[5] = irml.read_u16();
    ir_val[6] = irfm.read_u16();

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
    pc.printf("ir_val : | %u | %u | %u | %u | %u | %u | %u |\n", ir_val[0], ir_val[1], ir_val[2], ir_val[3], ir_val[4], ir_val[5], ir_val[6]); // 확인용 코드
    pc.printf("ir_WhCol : | %d | %d | %d | %d | %d | %d |\n", ir_WhCol[0], ir_WhCol[1], ir_WhCol[2], ir_WhCol[3], ir_WhCol[4], ir_WhCol[5]); // 확인용 코드
    pc.printf("psdf_val : | %lf |, psdb_val : | %lf |\n", psdf_val, psdb_val); // 확인용 코드
    pc.printf("mpu6050 pitch_p : %f\n", pitch_p); // 확인용 코드
}

// AC 서보 모터
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servo_set(PwmOut &rc){
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
        if(ras_data[1] == 1) ang = ang - inc;
        else if(ras_data[1] == 2) ang = ang - inc;
        else if(ras_data[1] == 3) ang = ang - inc;
        else if(ras_data[1] == 4) ang = ang - inc;
        pre_data0 = 1;
    }
    else if(width_l <= ras_data[0] && ras_data[0] < width_r){
        // pc.printf("3"); // 확인용 코드
        ang = ang;
        pre_data0 = 2;
    }
    else if(width_r <= ras_data[0]){
        // pc.printf("4"); // 확인용 코드
        if(ras_data[1] == 1) ang = ang + inc;
        else if(ras_data[1] == 2) ang = ang + inc;
        else if(ras_data[1] == 3) ang = ang + inc;
        else if(ras_data[1] == 4) ang = ang + inc;
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

// 타이머
void normal_tmr_move(Timer* _tmr, int* _time, double _speedL, double _speedR){
    _tmr->start(); // _tmr->start(); = *_tmr.start();
    while(_tmr->read_us() < *_time){
        speedL = _speedL; speedR = _speedR;

        whl_bundle();
    }
    _tmr->reset();
    _tmr->stop();
}

void tilt_tmr_move(Timer* _tmr, int* _time, float* _sensor, double _speedL, double _speedR){
    if(*_sensor > 10.0){
        _tmr->start();
    }
    else{
        _tmr->reset();
        _tmr->stop();
    }

    if(_tmr->read_us() > *_time){
        speedL = _speedL; speedR = _speedR;
    }
}

void tilt_tmr_reset(Timer* _tmr){
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

    servo_move(Servo);
    DC_move(speedL, speedR);

    all_print();
}

// print
void all_print(){
    pc.printf("ㅡㅡㅡㅡㅡ총합ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("mode = %d \n", mode); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ통신ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("ras_data = %.3f, %.3f, %.3f\n", ras_data[0], ras_data[1], ras_data[2]); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ센서ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    sensor_print(); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ모터ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("ang = %f, inc = %f, speedL = %f, speedR = %f\n", ang, inc, speedL, speedR); // 확인용 코드
    pc.printf("=======================\n"); // 확인용 코드
}