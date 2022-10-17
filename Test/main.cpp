#include "mbed.h"

Serial pc(USBTX, USBRX);
Serial HC06(PC_10, PC_11);

Timer tmr;
Timer control_tmr;

int n = 0;
double pre_speedL = 0;
double pre_speedR = 0;
double temp_speedL = 0;
double temp_speedR = 0;
double speedL = 0;
double speedR = 0;

// main() runs in its own thread in the OS
int main(){
    pc.baud(9600);
    HC06.baud(9600);

    char ch;
    pc.printf("Hello World!\n\r");
    HC06.printf("Hello World!\n\r");
    
    control_tmr.start();
    tmr.start();

    while(1){
        if(control_tmr.read_ms() > 100){
            if(tmr.read_ms() < 10000){
                temp_speedL = 1.0; temp_speedR = 0.4;
            }
            else if(tmr.read_ms() < 20000){
                temp_speedL = -0.6; temp_speedR = -1.0;
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


            if(temp_speedL > 0.0 && temp_speedR > 0.0){
                if(pre_speedL < 0.0 && pre_speedR < 0.0){
                    speedL = 0.0;
                    speedR = 0.0;
                }
                else if(pre_speedL < temp_speedL || pre_speedR < temp_speedR){
                    speedL = temp_speedL;
                    speedR = temp_speedR;
                }
            }
            else{
                speedL = temp_speedL;
                speedR = temp_speedR;
            }

            speedL += temp_speedL / 10.0;
            speedR += temp_speedR / 10.0;


            if(speedL > 1.0){
                speedL = 1.0;
            }
            else if(speedR > 1.0){
                speedR = 1.0;
            }

            pre_speedL = speedL;
            pre_speedR = speedR;

            pc.printf("%.2f, %.2f \n", speedL, speedR);

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
            