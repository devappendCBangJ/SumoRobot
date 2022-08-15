#include "mbed.h"

AnalogIn ir_new(PA_0);
AnalogIn ir_old(PA_1);

RawSerial pc(USBTX, USBRX, 9600);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

// main() runs in its own thread in the OS
int main(){
    while(true){
        int ir_new_val = ir_new.read_u16();
        int ir_old_val = ir_old.read_u16();
        pc.printf("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\n");
        pc.printf("ir_new_val : %d \n", ir_new_val);
        pc.printf("ir_old_val : %d \n", ir_old_val);
    }
}