// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "mbed.h"
#include "GP2A.h"

// ir 센서
AnalogIn ir(PA_0);    // 핀 번호 선언 - AnalogIn

// psd 센서
GP2A psdf(PA_1, 30, 150, 60, 0); // 핀 번호 선언 - AnalogIn

uint16_t ir_val;
double psdf_val;

// 통신
RawSerial pc(USBTX, USBRX, 115200); // 통신 객체 선언 - RawSerial

// [함수정의]
void sensor_read(); // 센서값 읽기
void sensor_print(); // 센서값 출력

// [main문]
int main(){
    while(1){
        sensor_read(); // 센서값 읽기
        sensor_print(); // 센서값 출력
    }
}

// [함수 정의]
// ir + psd 센서
void sensor_read(){ // 센서값 읽기
    ir_val = ir.read_u16(); // IR값 읽기 - AnalogIn

    psdf_val = psdf.getDistance(); // psd값 읽기 - AnalogIn
}

void sensor_print(){ // 센서값 출력 - pc.printf
    pc.printf("ir_val : | %d |\n", ir_val);
    pc.printf("psd_val : | %lf |\n", psdf_val);
}

/*
#include "mbed.h"

Serial pc(USBTX, USBRX);
Serial HC06(PC_10, PC_11);

Timer tmr;
Timer control_tmr;

float n = 1;
int phase = 0;
double pre_speedL = 0;
double pre_speedR = 0;
double target_speedL = 0;
double target_speedR = 0;
double speedL = 0;
double speedR = 0;

bool back = true;
bool comp = false;

InterruptIn btn(BUTTON1);

// 버튼 + LED
void btn_flip(){
    n = 0;
    back = true;
}

// main() runs in its own thread in the OS
int main(){
    pc.baud(9600);
    HC06.baud(9600);

    char ch;
    pc.printf("Hello World!\n\r");
    HC06.printf("Hello World!\n\r");

    btn.fall(&btn_flip);
    
    control_tmr.start();
    tmr.start();

    while(1){
        if(control_tmr.read_ms() > 100){
            if(tmr.read_ms() < 1000){
                phase = 1;
                target_speedL = 1.0; target_speedR = 0.4;
            }
            else if(tmr.read_ms() < 2000){
                phase = 2;
                target_speedL = -0.6; target_speedR = -1.0;
            }
            else if(tmr.read_ms() < 2500){
                phase = 3;
                target_speedL = 0.5; target_speedR = 0.5;
            }
            else if(tmr.read_ms() < 2800){
                phase = 4;
                target_speedL = -0.6; target_speedR = -1.0;
            }
            else if(tmr.read_ms() < 3100){
                phase = 5;
                target_speedL = 1.0; target_speedR = 1.0;
            }
            else if(tmr.read_ms() < 3500){
                phase = 6;
                target_speedL = -1.0; target_speedR = -1.0;
            }
            else{
                phase = 7;
                tmr.reset();
            }
            // if(HC06.readable()){
            //     ch = HC06.getc();
            //     pc.printf("%c", ch);
            //     HC06.printf("%c", ch);
            // }
            // else if(pc.readable()){
            //     ch = pc.getc();
            //     pc.printf("%c", ch);
            //     HC06.printf("%c", ch);
            // }

            // // 움직임
            // if(target_speedL > 0.0 && target_speedR > 0.0){ // 목표 : 전진
            //     if(pre_speedL < 0.0 && pre_speedR < 0.0){ // 이전 : 후진
            //         if(speedL < 0.0 && speedR < 0.0){ // [목표 : 전진] + [이전 : 후진] + [현재 : 후진] -> 정지
            //             speedL = 0.01;
            //             speedR = 0.01;
            //         }
            //         else{ // [목표 : 전진] + [이전 : 후진] + [현재 : 전진] -> 천천히 적용
            //             speedL += target_speedL / 10.0;
            //             speedR += target_speedR / 10.0;

            //             if(speedL > target_speedL){
            //                 speedL = target_speedL;

            //                 comp = true;
            //                 pre_speedL = target_speedL;
            //             }
            //             if(speedR > target_speedR){
            //                 speedR = target_speedR;

            //                 comp = true;
            //                 pre_speedR = target_speedR;
            //             }
            //         }
            //     }
            //     else{ // [목표 : 전진] + [이전 : 전진] -> 즉시 적용
            //         speedL = target_speedL;
            //         speedR = target_speedR;

            //         comp = true;
            //         pre_speedL = target_speedL;
            //         pre_speedR = target_speedR;
            //     }
            // }
            // else{ // [목표 : 후진] -> 즉시 적용
            //     speedL = target_speedL;
            //     speedR = target_speedR;

            //     comp = true;
            //     pre_speedL = target_speedL;
            //     pre_speedR = target_speedR;
            // }

            // // 움직임 제한
            // if(speedL > 1.0){
            //     speedL = 1.0;
            // }
            // else if(speedR > 1.0){
            //     speedR = 1.0;
            // }

            // target_speedL = 0.0;
            // target_speedR = 0.0;

            if(back == true){
                n += 0.1;

                if(n >= 1){
                    n = 1;
                    back = false;
                }
            }

            speedL = target_speedL * n;
            speedR = target_speedR * n;

            pc.printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
            pc.printf("%d \n", phase);
            pc.printf("[t] %.2f, %.2f \n", target_speedL, target_speedR);
            pc.printf("[n] %.2f, %.2f \n", speedL, speedR);

            control_tmr.reset();

            // n++;
        }
    }
}

// // Bluetooth HC-06 may work for HC-05 (I didn't try - check https://mbed.org/users/edodm85/notebook/HC-05-bluetooth/) 
// // Use pins (RXD - PTC4, TXD - PTC3) on FRDM-KL25Z
// // Use both USBs to power the HC-06 (J9_12 - GND, J9_10 - 5V usb(VCC))
// #include "mbed.h"
 
// Serial HC06(PTC4, PTC3);
 
// int main() {
//     HC06.baud(9600);
//     HC06.printf("Press 'r'\n");
//     while (1) {
//         char c = HC06.getc();
//         if(c == 'r') {
//             HC06.printf("Hello World\n");
//         }
//     }
// }
*/