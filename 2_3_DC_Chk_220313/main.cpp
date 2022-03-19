
// [라이브러리]
#include "mbed.h"

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// 통신
// Serial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
Serial pc(USBTX, USBRX, 1200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

// DC 모터
DigitalOut DirL(PB_6);
DigitalOut DirR(PC_7);
PwmOut PwmL(PB_4);
PwmOut PwmR(PB_5);

// 통신
volatile bool gotPacket2 = false;
volatile float data2[3];

// [함수정의]
void DC_set();
void DC_go();

// [main문]
int main(){
    //DC_set();
    while(1){
        printf("%c", pc.getc());
        //DC_go();
    }
}

// [함수 정의]
// DC 모터
void DC_set(){
    PwmL.period_us(66);
    PwmR.period_us(66);
}

void DC_go(){
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