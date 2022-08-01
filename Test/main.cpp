#include "mbed.h"

InterruptIn btn(BUTTON1);
DigitalOut led1(LED1);

RawSerial pc(USBTX, USBRX, 9600);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

Ticker tic;
int led_cnt = 0;
int tic_cnt = 0;
int tic_even_cnt = 0;

void flip();
void led_flash();

// main() runs in its own thread in the OS
int main(){
    btn.fall(&flip);
    tic.attach(&led_flash, 0.10);
    while(true){
        pc.printf("| led : %d | led_cnt : %d | tic_cnt : %d | tic_even_cnt : %d | \n", led1.read(), led_cnt, tic_cnt, tic_even_cnt);
    }
}

void flip(){
    led_cnt++;
}

void led_flash(){
    tic_cnt++;
    if(tic_cnt % 2 == 0){
        tic_even_cnt++;
        if(led_cnt > (tic_even_cnt % 10)) led1 = 1;
    }
    else led1 = 0;
}