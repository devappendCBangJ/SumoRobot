// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "mbed.h"
#include "GP2A.h"

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// thread
Thread com_th;

// 스위치
DigitalIn test1(PB_8);
DigitalIn test(PB_8);

// psd센서
GP2A psdl(PA_6, 30, 150, 60, 0);
GP2A psdr(PA_7, 30, 150, 60, 0);

double psdl_val;
double psdr_val;

// AC서보 모터
PwmOut ServoL(PA_2);
PwmOut ServoR(PA_3);

float angL=90.0, incL=0.015;
float angR=90.0, incR=0.015;

// DC 모터
DigitalOut DirL(PC_7);
DigitalOut DirR(PB_6);
PwmOut PwmL(PB_4);
PwmOut PwmR(PB_5);

template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max);

// 통신
Serial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
Serial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

// 통신 - ras_com
volatile bool gotPacket = false;
volatile float ras_data[3];
// ras_data[0] : 물체 잡기 판단
    // 잡지마 : 0
    // 잡아 : 1
// ras_data[1] : 상대 크기
    // 작음 : 0
    // 큼 : 1
// ras_data[2] : 상대 위치
    // 왼쪽 : 0
    // 가운데 : 1
    // 오른쪽 : 2

// 통신 - DC_chk
volatile bool gotPacket2 = false;
volatile float pc_data[3];

// 기타
int mode = 0;

// [함수정의]
void sensor_print();

void servo_move(PwmOut &rc);
void servo_chk(PwmOut &rc);

void DC_set();
void DC_move(int _dirL, int _dirR, float _PwmL, float _PwmR);

void in_SerialRx();
void in_SerialRx_print();
void th_SerialRx();

// [main문]
int main(){
    pc.format(8, SerialBase::Even, 1);

    // 모터 세팅
    ServoL.period_ms(10);
    ServoR.period_ms(10);
    PwmL.period_us(66);
    PwmR.period_us(66);
    // ras.attach(&in_SerialRx); // interrupt 전용

    com_th.start(&th_SerialRx); // thread 전용
    while(1){
        // in_SerialRx_print(); // interrupt 전용

        // 센서값
        psdl_val = psdl.getDistance();
        psdr_val = psdr.getDistance();
        // sensor_print();

        // AC 서보모터 움직임
        // servo_chk(Servo); // Test 코드
        servo_move(ServoL);
        servo_move(ServoR);

        // DC 모터 움직임

        if(test <= 5000){
            ras_data[2] = 0;
        }
        else{
            ras_data[2] = 1;
        }

        if(ras_data[0] == 0){
            if(ras_data[2] == 0){
                DC_move(0, 1, 0.20, 0.20);
            }
            else if(ras_data[2] == 1){
                DC_move(1, 1, 0.20, 0.20);
            }
            else if(ras_data[2] == 2){
                DC_move(1, 0, 0.20, 0.20);
            } 
        }

        if(ras_data[0] == 1){
            if(ras_data[2] == 0){
                DC_move(0, 1, 0.10, 0.10);
            }
            else if(ras_data[2] == 1){
                DC_move(1, 1, 0, 0);
            }
            else if(ras_data[2] == 2){
                DC_move(1, 0, 0.10, 0.10);
            }
        }
    }
}

// [함수 정의]
// ir + psd 센서
void sensor_print(){
    printf("psdl_val : | %lf |\n", psdl_val);
    printf("psdr_val : | %lf |\n", psdr_val);
}

// AC 서보 모터
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servo_move(PwmOut &rc){
    if(ras_data[0] == 0){
        angL = 0.0;
        angR = 180.0;
    }
    else if(ras_data[0] == 1){
        angL = angL + incL;
        angR = angR - incR;
    }

    if (angL > 90.0){
        angL = 90.0;
    }
    else if (angL < 0.0){
        angL = 0.0;
    }
    if (angR < 90.0){
        angR = 90.0;
    }
    else if (angR > 180){
        angR = 180.0;
    }
    uint16_t pulseWL = map<float>(angL, 180., 0., 500., 2600.);
    uint16_t pulseWR = map<float>(angR, 180., 0., 500., 2600.);
    rc.pulsewidth_us(pulseWL);
    rc.pulsewidth_us(pulseWR);
}

void servo_chk(PwmOut &rc){
    angL += incL;
    angR += incR;
    if (angL > 180.f || angL < 0.f){
        incL = -incL;
    }
    if (angR > 180.f || angR < 0.f){
        incR = -incR;
    }
    uint16_t pulseWL = map<float>(angL, 0., 180., 500., 2600.);
    uint16_t pulseWR = map<float>(angR, 0., 180., 500., 2600.);
    rc.pulsewidth_us(pulseWL);
    rc.pulsewidth_us(pulseWR);
}

void DC_move(int _dirL, int _dirR, float _PwmL, float _PwmR){
    DirL = _dirL;
    DirR = _dirR;
    PwmL = _PwmL;
    PwmR = _PwmR;
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
            pc.printf("byteIn == ',' : %c\n", byteIn); // 
            ras_serialInBuffer[ras_buff_cnt]='\0';
            ras_data[ras_data_cnt++]=atof(ras_serialInBuffer);
            ras_buff_cnt = 0;
        }
        // '/' : 시리얼 완료
        else if(byteIn=='/'){
            pc.printf("byteIn == '/' : %c \n", byteIn); // 
            ras_serialInBuffer[ras_buff_cnt] = '\0';
            ras_data[ras_data_cnt]=atof(ras_serialInBuffer);
            ras_buff_cnt=0; ras_data_cnt=0;
            gotPacket = true;
        }
        // 이외 : 시리얼 저장
        else{
            pc.printf("byteIn == '나머지' : %c\n", byteIn); // 
            ras_serialInBuffer[ras_buff_cnt++]=byteIn;
        }
    }
}

void in_SerialRx_print(){ // interrupt 전용
    // pc.printf("pc 연결 OK \n"); // pc, mbed 통신 출력
    if(gotPacket) {
        gotPacket = false;
        pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]);
    }
}

void th_SerialRx(){ // thread 전용
    static char ras_serialInBuffer[32];
    static int ras_data_cnt = 0, ras_buff_cnt = 0;

    while(true){
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
            pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]);
        }
    }
}