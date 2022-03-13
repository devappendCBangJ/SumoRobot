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
DigitalOut myled(LED1);
Servo myServo(D7);

int servo_angle;

// DC 모터
DigitalOut DirL(PB_6);
DigitalOut DirR(PC_7);
PwmOut PwmL(PB_4);
PwmOut PwmR(PB_5);

// 통신
RawSerial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
RawSerial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

volatile bool gotPacket = false;
volatile float data[3];

volatile bool gotPacket2 = false;
volatile float data2[3];
int good = 1;

// [함수정의]
void sensor_read();
void sensor_print();

void in_SerialRx();
void in_SerialRx_print();
void th_SerialRx();

void DC_set();
void DC_go();

// [main문]
int main(){
    // ras.attach(&in_SerialRx); // interrupt 전용
    // com_th.start(&th_SerialRx); // thread 전용
    DC_set();
    while(1){
        sensor_read();
        // in_SerialRx_print(); // interrupt 전용
        // sensor_print();

        // DC_chk();
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

// 통신(pc, ras, mbed)
void in_SerialRx(){ // interrupt 전용
    static char serialInBuffer[32];
    static int data_cnt = 0, buff_cnt = 0;

    if(ras.readable()){
        // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
        char byteIn = ras.getc();
        if(byteIn == ','){
            pc.printf("byteIn == ',' : %c\n", byteIn); // 
            serialInBuffer[buff_cnt]='\0';
            data[data_cnt++]=atof(serialInBuffer);
            buff_cnt = 0;
        }
        else if(byteIn=='\n'){
            pc.printf("byteIn == '\n' : %c \n", byteIn); // 
            serialInBuffer[buff_cnt] = '\0';
            data[data_cnt]=atof(serialInBuffer);
            buff_cnt=0; data_cnt=0;
            gotPacket = true;
        }
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
            if(byteIn == ','){
                pc.printf("byteIn == ',' : %c\n", byteIn); // 
                serialInBuffer[buff_cnt]='\0';
                data[data_cnt++]=atof(serialInBuffer);
                buff_cnt = 0;
            }
            else if(byteIn=='/'){
                pc.printf("byteIn == '\n' : %c \n", byteIn); // 
                serialInBuffer[buff_cnt] = '\0';
                data[data_cnt]=atof(serialInBuffer);
                buff_cnt=0; data_cnt=0;
                gotPacket = true;
            }
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

// DC 모터
void DC_set(){
    PwmL.period_us(66); // 주파수 : 15kHz(모터드라이버 : 최대 주파수 20kHz)
    PwmR.period_us(66); // 주파수 : 15kHz(모터드라이버 : 최대 주파수 20kHz)
}

void DC_chk(){
    static char serialInBuffer2[32];
    static int data_cnt2 = 0, buff_cnt2 = 0;

    if(pc.readable()){
        // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
        char byteIn2 = pc.getc();
        if(byteIn2=='\n'){
            pc.printf("byteIn == '\n' : %c \n", byteIn2);
            serialInBuffer2[buff_cnt2] = '\0';
            data2[data_cnt2]=atof(serialInBuffer2);
            buff_cnt2=0; data_cnt2=0;
            gotPacket2 = true;
        }
        else{
            pc.printf("byteIn == '나머지' : %c\n", byteIn2);
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