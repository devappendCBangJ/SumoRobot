#include "mbed.h"

Timer tmr;

RawSerial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

int main(){
    while(1){
        tmr.start();
        while(tmr.read_us() < 500000){
            pc.printf("%d \n", tmr.read_us()); // 확인용 코드
        }
        tmr.reset();
        tmr.stop();
    }
}