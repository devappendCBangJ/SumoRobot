#include "mbed.h"

AnalogIn psdf(PA_6);

RawSerial pc(USBTX, USBRX, 9600);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

// main() runs in its own thread in the OS
int main()
{
    while (true) {
        float volts = psdf.read() * 3.3;
        pc.printf("volts : %f \n", volts);
        float distance = 13 * pow(volts, -1.0);
        if(distance < 30){
            pc.printf("distance : %f \n", distance);
        }
        wait(0.1);
    }

    btn.fall(&flip);
    while(true){
        led1 =! led1;
        wait_ms(100);
    }
}

void flip(){
    led2 =! led2;
}