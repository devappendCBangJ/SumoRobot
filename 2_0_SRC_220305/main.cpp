// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "mbed.h"
#include "Servo.h"
#include "GP2A.h"

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// thread
Thread com_th;

// ir센서 + psd센서
AnalogIn irfl(PA_0);
AnalogIn irfr(PA_1);
AnalogIn irmr(PA_4);
AnalogIn irbr(PB_0);
AnalogIn irbl(PC_1);
AnalogIn irml(PC_0);
GP2A psdb(PC_4, 30, 150, 60, 0);

uint16_t irfl_val, irfr_val, irmr_val, irbr_val, irbl_val, irml_val;
double psdb_val;

// AC서보 모터
PwmOut Servo(PA_8);

float ang=0., inc=0.5;

// DC 모터
DigitalOut DirL(PB_6);
DigitalOut DirR(PC_7);
PwmOut PwmL(PB_4);
PwmOut PwmR(PB_5);

template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max);

// 통신
Serial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
Serial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

// 통신 - ras_com
volatile bool gotPacket = false;
volatile float data[3];
// data[0] : 상대 방향 + 보임 유무
    // 없음 : 0
    // 왼쪽 : 1
    // 가운데 : 2
    // 오른쪽 : 3
// data[1] : 상대 크기
    // 작음 : 0
    // 보통 : 1
    // 큼 : 2
// data[2] : 빨간영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 빨간 영역 안 : 0(상대 가까울 때만 공격)
    // 빨간 영역 밖 : 1(무조건 공격)
// data[3] : 파란영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 파란 영역 안 : 0(그대로 밀면 됨)
    // 파란 영역 밖 : 1(파란 영역 안으로 넣으면 best)

// 통신 - DC_chk
volatile bool gotPacket2 = false;
volatile float data2[3];

// [함수정의]
void sensor_read();
void sensor_print();

void servo_move(PwmOut &rc);

void DC_set();
void DC_move();
void DC_chk();

void in_SerialRx();
void in_SerialRx_print();
void th_SerialRx();

// [main문]
int main(){
    Servo.period_ms(10);
    DC_set();
    // ras.attach(&in_SerialRx); // interrupt 전용
    com_th.start(&th_SerialRx); // thread 전용
    while(1){
        sensor_read();
        // in_SerialRx_print(); // interrupt 전용
        // sensor_print();
        servo_move(Servo);
        DC_move();
        DC_chk();
    }
}

// [함수 정의]
// ir + psd 센서
void sensor_read(){
    irfl_val = irfl.read_u16();
    irfr_val = irfr.read_u16();
    irmr_val = irmr.read_u16();
    irbr_val = irbr.read_u16();
    irbl_val = irbl.read_u16();
    irml_val = irml.read_u16();

    psdb_val = psdb.getDistance();
}

void sensor_print(){
    printf("ir_val : | %u | %u | %u | %u | %u | %u |\n", irfl_val, irfr_val, irmr_val, irbr_val, irbl_val, irml_val);
    printf("psd_val : | %lf |\n", psdb_val);
}

// AC 서보 모터
void servo_move(PwmOut &rc){
    wait_ms(10);
    ang+=inc;
    if (ang > 180.f || ang < 0.f){
        inc = -inc;
    }
    uint16_t pulseW = map<float>(ang, 0., 180., 500., 2600.);
    rc.pulsewidth_us(pulseW);
}

template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// DC 모터
void DC_set(){
    PwmL.period_us(66); // 주파수 : 15kHz(모터드라이버 : 최대 주파수 20kHz)
    PwmR.period_us(66); // 주파수 : 15kHz(모터드라이버 : 최대 주파수 20kHz)
}

void DC_move(){
    if(data[0] == 0){
        DirL = 1, DirR = 0;
        PwmL = 0.3f, PwmR = 0.3f;
    }
    else if(data[0] == 1){
        DirL = 0, DirR = 1;
        PwmL = 0.3f, PwmR = 0.3f; 
    }
    else if(data[0] == 2){
        DirL = 1, DirR = 1;
        PwmL = 0.0f, PwmR = 0.0f;
    }
    else if(data[0] == 3){
        DirL = 1, DirR = 0;
        PwmL = 0.3f, PwmR = 0.3f; 
    }
}

void DC_chk(){
    static char serialInBuffer2[32];
    static int buff_cnt2 = 0;
        
    if(pc.readable()) {
        pc.printf("%d \n", pc.readable());
        // pc.printf("Ras 연결 OK \n"); // Ras, mbed 통신 출력
        char byteIn2 = pc.getc();
        if(byteIn2=='\n'){
            pc.printf("byteIn == '띄워' : %c \n", byteIn2);
            serialInBuffer2[buff_cnt2] = '\0';
            data2[0]=atof(serialInBuffer2);
            buff_cnt2=0;
            gotPacket2 = true;
        }
        else{
            pc.printf("byteIn == '나머지' : %c \n", byteIn2);
            serialInBuffer2[buff_cnt2++]=byteIn2;
        }

        if(gotPacket2) {
            gotPacket2 = false;
            pc.printf("PWM = %.3f \n\r", data2[0]);
        }
    }

    if (0 <= data2[0] <= 95){
        PwmL = 0.01 * data2[0];
        PwmR = 0.01 * data2[0];
    }

    DirR = 1;
    DirL = 1;
    // PwmL = 0.3f;
    // PwmR = 0.3f;
}

// 통신(pc, ras, mbed)
void in_SerialRx(){ // interrupt 전용
    static char serialInBuffer[32];
    static int data_cnt = 0, buff_cnt = 0;

    if(ras.readable()){
        // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
        char byteIn = ras.getc();
        // ',' : 단어 자르기
        if(byteIn == ','){
            pc.printf("byteIn == ',' : %c\n", byteIn); // 
            serialInBuffer[buff_cnt]='\0';
            data[data_cnt++]=atof(serialInBuffer);
            buff_cnt = 0;
        }
        // '/' : 시리얼 완료
        else if(byteIn=='/'){
            pc.printf("byteIn == '/' : %c \n", byteIn); // 
            serialInBuffer[buff_cnt] = '\0';
            data[data_cnt]=atof(serialInBuffer);
            buff_cnt=0; data_cnt=0;
            gotPacket = true;
        }
        // 이외 : 시리얼 저장
        else{
            pc.printf("byteIn == '나머지' : %c\n", byteIn); // 
            serialInBuffer[buff_cnt++]=byteIn;
        }
    }
}

void in_SerialRx_print(){ // interrupt 전용
    // pc.printf("pc 연결 OK \n"); // pc, mbed 통신 출력
    if(gotPacket) {
        gotPacket = false;
        pc.printf("data = %.3f, %.3f, %.3f \n\r", data[0], data[1], data[2]);
    }
}

void th_SerialRx(){ // thread 전용
    static char serialInBuffer[32];
    static int data_cnt = 0, buff_cnt = 0;

    while(true){
        if(ras.readable()){
            // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
            char byteIn = ras.getc();
            // ',' : 단어 자르기
            if(byteIn == ','){
                pc.printf("byteIn == ',' : %c\n", byteIn); // 
                serialInBuffer[buff_cnt]='\0';
                data[data_cnt++]=atof(serialInBuffer);
                buff_cnt = 0;
            }
            // '/' : 시리얼 완료
            else if(byteIn=='/'){
                pc.printf("byteIn == '/' : %c \n", byteIn); // 
                serialInBuffer[buff_cnt] = '\0';
                data[data_cnt]=atof(serialInBuffer);
                buff_cnt=0; data_cnt=0;
                gotPacket = true;
            }
            // 이외 : 시리얼 저장
            else{
                pc.printf("byteIn == '나머지' : %c\n", byteIn); // 
                serialInBuffer[buff_cnt++]=byteIn;
            }
        }

        if(gotPacket) {
            gotPacket = false;
            pc.printf("data = %.3f, %.3f, %.3f \n\r", data[0], data[1], data[2]);
        }
    }
}