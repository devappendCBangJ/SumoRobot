// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "mbed.h"
#include "Servo.h"
#include "GP2A.h"

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// ir 센서
AnalogIn irfl(PA_0);    // 핀 번호 선언 - AnalogIn
AnalogIn irfr(PA_1);
AnalogIn irmr(PA_4);
AnalogIn irbr(PB_0);
AnalogIn irbl(PC_1);
AnalogIn irml(PC_0);

// psd 센서
GP2A psdb(PC_4, 10, 80, 0.246, -0.297); // 핀 번호 선언 - AnalogIn

uint16_t irfl_val, irfr_val, irmr_val, irbr_val, irbl_val, irml_val;
double psdb_val;

// 통신
RawSerial ras(PA_9, PA_10, 115200);
RawSerial pc(USBTX, USBRX, 115200); // 통신 객체 선언 - RawSerial

volatile bool gotPacket = false;
volatile float data[3];

// [함수정의]
void sensor_read(); // 센서값 읽기
void sensor_print(); // 센서값 출력

void onSerialRx(); // 통신값 읽기
void onSerialRx_print(); // 통신값 출력

// [main문]
int main(){
    ras.attach(&onSerialRx); // 통신값 읽기 - Interrupt
    while(1){
        sensor_read(); // 센서값 읽기

        onSerialRx_print(); // 통신값 출력
        sensor_print(); // 센서값 출력
    }
}

// [함수 정의]
// ir + psd 센서
void sensor_read(){ // 센서값 읽기
    irfl_val = irfl.read_u16(); // IR값 읽기 - AnalogIn
    irfr_val = irfr.read_u16();
    irmr_val = irmr.read_u16();
    irbr_val = irbr.read_u16();
    irbl_val = irbl.read_u16();
    irml_val = irml.read_u16();

    psdb_val = psdb.getDistance(); // psd값 읽기 - AnalogIn
}

void sensor_print(){ // 센서값 출력 - pc.printf
pc.printf("ir_val : | %d |\n", irfl_val);
    pc.printf("psd_val : | %lf |\n", psdb_val);
}

// 통신(pc, ras, mbed)
void onSerialRx(){ // 통신값 읽기
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

void onSerialRx_print(){ // 통신값 출력
    // pc.printf("pc 연결 OK \n"); // pc, mbed 통신 출력
    if(gotPacket) {
        gotPacket = false;
        pc.printf("data = %.3f, %.3f, %.3f \n\r", data[0], data[1], data[2]);
    }
}