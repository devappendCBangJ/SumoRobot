// // [통신(pc, ras, mbed) Test] ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

// // Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// // 라이브러리 불러오기
// #include "mbed.h"
// #include "Servo.h"

// // 서보 객체 생성
// DigitalOut myled(LED1);
// Servo myServo(D7);
// int servo_angle;

// int cnt = 0;

// RawSerial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
// RawSerial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
// int main(){
//     while(1){
//         cnt++;
//         // pc와 통신
//         if(cnt % 1000000 == 0){
//             pc.printf("PC 연결 OK \n");
//         }

//         // 라즈베리파이와 통신
//         if(ras.readable()>0){
//             pc.printf("파이 연결 OK \n");
//             servo_angle = ras.getc();
//             // 서보 구동
//             myServo = servo_angle;           // Serial1 통신값에 의해 서보 회전
            
//             pc.printf("set to ");              // Serial포트로 메시지 작성(컴퓨터에서 확인용)
//             pc.printf("%d \n", servo_angle);          // Serial포트로 각도값 작성(컴퓨터에서 확인용)
//         }
//     }
// }

#include "mbed.h"

RawSerial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

volatile bool gotPacket = false;
volatile float data[3];

int main(){
    static char serialInBuffer[32];
    static int data_cnt = 0, buff_cnt = 0;

    while(true){
        if(pc.readable()){
            // pc.printf("pc 연결 OK\n"); // pc, mbed 통신 출력
            char byteIn = pc.getc();
            if(byteIn == ','){
                // pc.printf("byteIn == ',' : %c\n", byteIn); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
                serialInBuffer[buff_cnt]='\0';
                data[data_cnt++]=atof(serialInBuffer);
                buff_cnt = 0;
            }
            else if(byteIn=='/'){
                // pc.printf("byteIn == '\n' : %c \n", byteIn); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
                serialInBuffer[buff_cnt] = '\0';
                data[data_cnt]=atof(serialInBuffer);
                buff_cnt=0; data_cnt=0;
                gotPacket = true;
            }
            else{
                // pc.printf("byteIn == '나머지' : %c\n", byteIn); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
                serialInBuffer[buff_cnt++]=byteIn;
            }
        }

        if(gotPacket) {
            gotPacket = false;
            pc.printf("data = %.3f, %.3f, %.3f \n\r", data[0], data[1], data[2]);
        }
    }
}