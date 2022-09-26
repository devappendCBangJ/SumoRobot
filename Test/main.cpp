#include "mbed.h"

Serial pc(USBTX, USBRX);
Serial HC06(PC_10, PC_11);

int n = 0;

// main() runs in its own thread in the OS
int main(){
    pc.baud(9600);
    HC06.baud(9600);

    char ch;
    pc.printf("Hello World!\n\r");
    HC06.printf("Hello World!\n\r");

    while(1){
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
        HC06.printf("%d \n", n);

        n++;
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
            