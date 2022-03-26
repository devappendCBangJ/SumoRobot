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

uint16_t ir_val[6];
// 0 : fl
// 1 : fr
// 2 : mr
// 3 : br
// 4 : bl
// 5 : ml
bool ir_res[6];
// 0 : fl + fr
// 1 : bl + br
// 2 : ml + fl
// 3 : fr + mr
// 4 : mr + br
// 5 : bl + ml
double psdb_val;
int black = 40000;

// AC서보 모터
PwmOut Servo(PA_8);

float ang=0., inc=1.0;

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
volatile float ras_data[3];
// ras_data[0] : 상대 방향 + 보임 유무
    // 없음 : 0
    // 왼쪽 : 1
    // 가운데 : 2
    // 오른쪽 : 3
// ras_data[1] : 상대 크기
    // 작음 : 0
    // 보통 : 1
    // 큼 : 2
// ras_data[2] : 빨간영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 빨간 영역 안 : 0(상대 가까울 때만 공격)
    // 빨간 영역 밖 : 1(무조건 공격)
// ras_data[3] : 파란영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 파란 영역 안 : 0(그대로 밀면 됨)
    // 파란 영역 밖 : 1(파란 영역 안으로 넣으면 best)
int pre_data0 = 0;
// 상대 최근 위치 없음 : 0
// 상대 최근 위치 왼쪽 : 1
// 상대 최근 위치 가운데 : 2
// 상대 최근 위치 오른쪽 : 3

// 통신 - DC_chk
volatile bool gotPacket2 = false;
volatile float pc_data[3];

// 기타
int mode = 0;

// [함수정의]
void sensor_read();
void sensor_cal();
void sensor_print();

void servo_move(PwmOut &rc);
void servo_chk(PwmOut &rc);

void DC_set();
void DC_move(int _dirL, int _dirR, float _PwmL, float _PwmR);
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
        // in_SerialRx_print(); // interrupt 전용

        sensor_read();
        sensor_cal();
        sensor_print();

        // servo_chk(Servo); // Test 코드
        // DC_chk(); // Test 코드

        // 초기 동작 : 상대 탐색
        if(mode == 0){
            if(ras_data[0] == 0){
                DC_move(1, 0, 0.3, 0.3);
            }
            else if(ras_data[0] == 1){
                DC_move(0, 1, 0.3, 0.3);
            }
            else if(ras_data[0] == 2){
                DC_move(1, 1, 0.0, 0.0);
                mode = 1;
            }
            else if(ras_data[0] == 3){
                DC_move(1, 0, 0.3, 0.3);
            }
        }
        
        // 중간 동작 : 상대 탐색 + 원 회전 + 공격
        else if(mode == 1){
            servo_move(Servo);

            if(ras_data[0] == 0){
                if(pre_data0 == 1) DC_move(0, 1, 0.3, 0.3);
                else if(pre_data0 == 2) DC_move(1, 0, 0.3, 0.3);
                else if(pre_data0 == 3) DC_move(1, 0, 0.3, 0.3);
                else DC_move(1, 0, 0.3, 0.3);
            }
            if(ras_data[0] != 0){
                if(ang < 80) DC_move(1, 1, 0.3, 0.5);
                else if(80 <= ang && ang <= 100) DC_move(1, 1, 0.3, 0.3);
                else if(100 < ang) DC_move(1, 1, 0.5, 0.3);
            }
        }
    }
}

// [함수 정의]
// ir + psd 센서
void sensor_read(){
    ir_val[0] = irfl.read_u16();
    ir_val[1] = irfr.read_u16();
    ir_val[2] = irmr.read_u16();
    ir_val[3] = irbr.read_u16();
    ir_val[4] = irbl.read_u16();
    ir_val[5] = irml.read_u16();

    psdb_val = psdb.getDistance();
}

void sensor_cal(){
    if(ir_val[0] < black && ir_val[1] < black){
        ir_res[0] = true;
    }
    else ir_res[0] = false;
    if(ir_val[3] < black && ir_val[4] < black){
        ir_res[1] = true;
    }
    else ir_res[1] = false;
    if(ir_val[5] < black && ir_val[1] < black){
        ir_res[2] = true;
    }
    else ir_res[2] = false;
    if(ir_val[1] < black && ir_val[2] < black){
        ir_res[3] = true;
    }
    else ir_res[3] = false;
    if(ir_val[2] < black && ir_val[3] < black){
        ir_res[4] = true;
    }
    else ir_res[4] = false;
    if(ir_val[3] < black && ir_val[4] < black){
        ir_res[5] = true;
    }
    else ir_res[5] = false;
}

void sensor_print(){
    printf("ir_val : | %u | %u | %u | %u | %u | %u |\n", ir_val[0], ir_val[1], ir_val[2], ir_val[3], ir_val[4], ir_val[5]);
    printf("ir_res : | %d | %d | %d | %d | %d | %d |", ir_res[0], ir_res[1], ir_res[2], ir_res[3], ir_res[4], ir_res[5]);
    printf("psd_val : | %lf |\n", psdb_val);
}

// AC 서보 모터
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servo_move(PwmOut &rc){
    if(ras_data[0] == 0){
        if(pre_data0 == 1) ang -= inc;
        else if(pre_data0 == 2) ang += ang;
        else if(pre_data0 == 3) ang += inc;
        else ang += inc;
    }
    else if(ras_data[0] == 1){
        ang-=inc;
        pre_data0 = 1;
    }
    else if(ras_data[0] == 2){
        ang = ang;
        pre_data0 = 2;
    }
    else if(ras_data[0] == 3){
        ang+=inc; 
        pre_data0 = 3;
    }

    if (ang > 180.f){
        ang = 180.0;
    }
    else if (ang < 0.0f){
        ang = 0.0;
    }
    uint16_t pulseW = map<float>(ang, 0., 180., 500., 2600.);
    rc.pulsewidth_us(pulseW);
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

void DC_move(int _dirL, int _dirR, float _PwmL, float _PwmR){
    DirL = _dirL, DirR = _dirR;
    PwmL = _PwmL, PwmR = _PwmR;
}

void DC_chk(){
    static char pc_serialInBuffer2[32];
    static int buff_cnt2 = 0;
        
    if(pc.readable()) {
        pc.printf("%d \n", pc.readable());
        // pc.printf("Ras 연결 OK \n"); // Ras, mbed 통신 출력
        char byteIn2 = pc.getc();
        if(byteIn2=='\n'){
            pc.printf("byteIn == '띄워' : %c \n", byteIn2);
            pc_serialInBuffer2[buff_cnt2] = '\0';
            pc_data[0]=atof(pc_serialInBuffer2);
            buff_cnt2=0;
            gotPacket2 = true;
        }
        else{
            pc.printf("byteIn == '나머지' : %c \n", byteIn2);
            pc_serialInBuffer2[buff_cnt2++]=byteIn2;
        }

        if(gotPacket2) {
            gotPacket2 = false;
            pc.printf("PWM = %.3f \n\r", pc_data[0]);
        }
    }

    if (0 <= pc_data[0] && pc_data[0] <= 95){
        PwmL = 0.01 * pc_data[0];
        PwmR = 0.01 * pc_data[0];
    }

    DirR = 1;
    DirL = 1;
    // PwmL = 0.3f;
    // PwmR = 0.3f;
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

        if(gotPacket) {
            gotPacket = false;
            pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]);
        }
    }
}