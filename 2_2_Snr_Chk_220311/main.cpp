// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "mbed.h"
#include "Servo.h"
#include "GP2A.h"

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// ir센서 + psd센서
AnalogIn irfl(PA_0);
AnalogIn irfr(PA_1);
AnalogIn irmr(PA_4);
AnalogIn irbr(PB_0);
AnalogIn irbl(PC_1);
AnalogIn irml(PC_0);
GP2A psdb(PC_4, 10, 80, 0.246, -0.297);

uint16_t irfl_val, irfr_val, irmr_val, irbr_val, irbl_val, irml_val;
double psdb_val;

// 통신
RawSerial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
RawSerial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

volatile bool gotPacket = false;
volatile float data[3];

// [함수정의]
void sensor_read();
void sensor_print();

void onSerialRx();
void onSerialRx_print();

// [main문]
int main(){
    ras.attach(&onSerialRx);
    while(1){
        sensor_read();

        onSerialRx_print(); // 통신값 출력
        sensor_print(); // 센서값 출력
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
    // printf("ir_val : | %u | \n", irbl_val);
    // printf("ir_val : | %u | %u | %u | %u | %u | %u |\n", irfl_val, irfr_val, irmr_val, irbr_val, irbl_val, irml_val);
    printf("psd_val : | %lf |\n", psdb_val);
}

// 통신(pc, ras, mbed)
void onSerialRx(){
    static char serialInBuffer[32];
    static int data_cnt = 0, buff_cnt = 0;

    if(ras.readable()){
        // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
        char byteIn = ras.getc();
        pc.printf("%c\n", byteIn); // Ras, mbed 통신 출력
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

void onSerialRx_print(){
    // pc.printf("pc 연결 OK \n"); // pc, mbed 통신 출력
    if(gotPacket) {
        gotPacket = false;
        pc.printf("data = %.3f, %.3f, %.3f \n\r", data[0], data[1], data[2]);
    }
}