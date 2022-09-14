// // ■ DigitalOut Class
    // 1. 사용방법
        // 1) 생성자 : DigitalOut

// DigitalOut myled(LED1);

// int main(){
//     while(1){
//         myled = 1;
//         wait(0.2);
//         myled = 0;
//         wait(1.0);
//     }
// }

// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

// // ■ DigitalIn Class
    // 1. 사용방법
        // 1) 생성자 : 

// DigitalOut led(LED1);

// DigitalIn but(BUTTON1);
// int main() {
//     while(1) {
//         led = !but;
//     }
// }

// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ● PwmOut
//     2. PwmOut 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

PwmOut pwm(D7);

int main(){
    int count = 0;
    pwm.period_us(25);

    while(1){
        pwm = count / 100.;
        count++;
        count %= 101;
        wait(0.1);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

PwmOut rcServo(D6);

void turn(PwmOut &rc, float deg);
template <class T> T map (T x, T in_min, T in_max, T out_min, T out_max);

int main(){
    float ang=0., inc=0.5;
    rcServo.period_ms(10);
    turn(rcServo, 0);
    
    while(1){
        turn(rcServo, ang);
        wait_ms(10);
        ang+=inc;
        if (ang > 180.f || ang < 0.f){
            inc = -inc;
        }
    }
}

void turn(PwmOut &rc, float deg){
    uint16_t pulseW = map<float>(deg, 0., 180., 600., 2400.);
    rc.pulsewidth_us(pulseW);
}

template <class T> T map (T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ● AnalogIn
//     2. AnalogIn 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

AnalogIn POT(A0);
int main(){
    while(1) {
        float voltage = POT * 3.3f;
        uint16_t value = POT.read_u16();
        printf("POT Value = %u, Voltage = %f \n", value, voltage);
        wait(0.5);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
GP2A psd1(A0, 20, 150, 60, 0);

int main(){
    while(1) {
        printf("Value = %lf, Voltage = %lf \n", psd1.getDistance(), psd1.getVoltage());
        wait(0.5);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
GP2A psd2(A0, 7, 80, 0.23625, -0.297);

int main(){
    while(1) {
        printf("Value = %lf, Voltage = %lf \n", psd2.getDistance(), psd2.getVoltage());
        wait(0.5);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

PwmOut rcServo(D6);
AnalogIn POT(A0);

void turn(PwmOut &rc, float deg);
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max);

int main(){
    float ang = 0., inc = 0.1;
    rcServo.period_ms(10);
    turn(rcServo, 0);

    while(1){
        inc = 2.0f * POT - 1.0f;
        ang += inc;
        if (ang > 180.0f) ang = 180.0f;
        else if (ang < 0.0f) ang = 0.0f;
        turn(rcServo, ang);
        wait_ms(10);
    }
}

void turn(PwmOut &rc, float deg){
    uint16_t pulseW = map<float>(deg, 0., 180., 600., 2400.);
    rc.pulsewidth_us(pulseW);
}

template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ● InterruptIn
//     2. InterruptIn 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
DigitalOut led1(LED1);
DigitalOut led2(D7);
InterruptIn btn(BUTTON1);

void flip(){
    led2 =! led2;
}

int main(){
    btn.fall(&flip);
    while(true){
        led1 =! led1;
        wait_ms(100);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
class Counter{
    InterruptIn _interrupt;
    volatile uint16_t _count;
    volatile bool _updated;
    void isr(){
        _count++;
        _updated = 1;
    }
    public:
        Counter(PinName pin): _interrupt(pin){ // ♣♣♣
            _count = 0; _updated = 0;
            _interrupt.fall(callback(this, &Counter::isr));
        }
        uint16_t read(){
            _updated = 0;
            return _count;
        }
        bool isUpdated(){
            return _updated;
        }
        operator int(){ // 오버로딩 연산자 ♣♣♣
            return read();
        }
};

Counter cnt(BUTTON1);
int main(){
    while(1){
        if(cnt.isUpdated()){
            // printf("count = %3d\n", cnt.read()); // 클래스 멤버함수 ♣♣♣
            printf("count = %3d\n", (int)cnt); // 오버로딩 연산자. int 변환 필수 ♣♣♣
            wait(0.1);
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/* // 이거 왜 제대로 안되냐??? 플로팅 현상 때문??? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"

class CounterUD{
    InterruptIn _upBot, _downBot;
    volatile int _count;
    volatile bool _updated;
    void plus(){
        _count++;
        _updated = true;
    }
    void minus(){
        _count--;
        _updated = true;
    }
    public:
        CounterUD(PinName p1, PinName p2): _upBot(p2, PullUp), _downBot(p1, PullUp){
            _count = 0; _updated = false;
            _upBot.fall(callback(this, &CounterUD::plus));
            _downBot.fall(callback(this, &CounterUD::minus));
        }
        int read(){
            _updated = false;
            return _count;
        }
        bool isUpdated(){
            return _updated;
        }
};

CounterUD cnt(D2, D3);

int main(){
    while(1){
        if(cnt.isUpdated()){
            printf("count = %3d \n", cnt.read());
            wait(0.005);
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ● Timer
//     2. Ticker 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
Ticker tic;
DigitalOut led1(LED1);
DigitalOut led2(D7);

void toggle(){
    led1 =! led1;
}

int main(){
    tic.attach(&toggle, 0.1);
    while(true){
        led2 =! led2;
        wait(1.0);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
#include "blinker.h"

// // 라이브러리화 by header file & class ♣♣♣
// class Blinker{
//     DigitalOut _led; // 가계약
//     Ticker _tick;
//     void blink(){
//         _led =! _led;
//     }
//     public:
//         Blinker(PinName pin): _led(pin){
//             _led = 0;
//         }
//         void begin(float sec){
//             _tick.attach(callback(this, &Blinker::blink), sec);
//         }
// };

Blinker blink(LED1);
DigitalOut led2(D7);

int main(){
    blink.begin(0.1);
    while(true){
        led2 =! led2;
        wait(0.1);
    }
}
*/
//     2. Timer 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

Timer watch;

int main(){
    while(1){
        watch.reset();
        watch.start();
        printf("Print 15bytes!\n");
        // watch.stop();
        printf("It takes about %5u us.\n", watch.read_us());
        wait(1);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
#define SAMPLE_INTERVAL 10000

Timer tmr;
DigitalOut led(LED1);

int main(){
    tmr.start();
    while(1){
        if(tmr.read_us() > SAMPLE_INTERVAL){
            tmr.reset();
            led=!led;
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//     2. Timeout 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

Timeout tmo;
DigitalOut led1(LED1);
DigitalOut led2(D7, 1);

void flip(){
    led2 =! led2;
}

int main(){
    tmo.attach(&flip, 10.0);
    while(1){
        led1 =! led1;
        wait(0.2);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/* // ♣♣♣♣♣ 이해하기 어렵다
#include "mbed.h"

class Debounce{
    InterruptIn _btn;
    Timeout _tmo;
    int _intval;
    int _state, _ready;

    void btnISR(){
        if(_ready){
            _ready = 0;
            _tmo.attach_us(callback(this, &Debounce::decide), _intval); // ♣♣♣
        }
    }
    void decide(){
        _state = 0;
        _ready = 1;
        _tmo.detach(); // ♣♣♣
    }

    public:
        Debounce(PinName pin, int intv = 100000) : _btn(pin){
            _btn.fall(callback(this, &Debounce::btnISR)); // ♣♣♣
            _state = 1;
            _ready = 1;
            _intval = intv;
        }
        int read(){
            int state = _state;
            _state = 1;
            return state;
        }
        operator int(){
            return read(); // ♣♣♣
        }
};

int main(){
    Debounce dbc(BUTTON1, 100000);
    int cnt = 0;
    while(1){
        if(!dbc){
            cnt++;
            printf("count = %d\n", cnt);
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ● UART
//     2. Serial 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         1) Serial_Basic
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

Serial pc(USBTX, USBRX); // mbed-os5에서는 RawSerial 사용. RawSerial 클래스에는 scanf가 정의되어있지 않다.
int main(){
    int n, m;

    while(1){
        pc.printf("Type two integers : ");
        pc.scanf("%d%d", &n, &m);
        pc.printf("\n You typed %d and %d\n", n, m);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//             2) Seiral loopback
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

Serial pc(USBTX, USBRX); // mbed-os5에서는 RawSerial 사용

int main(){
    printf("Loop back program start \n");
    while(1){
        if (pc.readable()){
            pc.putc(pc.getc());
        }
        // wait(0.001); // wait이 있으면 통신이 날아가는 도중에 막혀서 데이터 손실되므로, 통신에는 사용x
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//             3) Seiral loopback + Callback
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

RawSerial pc(USBTX, USBRX); // mbed-os5에서는 RawSerial 사용
DigitalOut led(LED1);
int d0;

// Callback Def
void serialEvent(){
    if(pc.readable()){
        pc.putc(pc.getc());
    }
}

int main(){
    printf("Loop back program start \n");
    pc.attach(&serialEvent);
    while(1){
        led =! led;
        wait(0.1);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//             4) Serial Communication + Callback
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

InterruptIn btn(BUTTON1);
DigitalOut led(LED1);
RawSerial pc(USBTX, USBRX); // mbed-os5에서는 RawSerial 사용

volatile char c = '\0';
volatile bool pressed = false;

// Callback Def
void onCharReceived(){
    c = pc.getc();
}
void onPressed(){
    pressed = true;
}

int main(){
    pc.attach(&onCharReceived);
    btn.fall(&onPressed);

    while(true){
        if(c == '1'){
            c = '\0';   // if문 1번만 동작시키기 위해 바로 값 바꿔줌 ♣♣♣
            led = 1;
        }
        else if(c == '0'){
            c = '\0';   // if문 1번만 동작시키기 위해 바로 값 바꿔줌 ♣♣♣
            led = 0;
        }
        if (pressed){
            pc.putc('b');
            pressed = false;   // if문 1번만 동작시키기 위해 바로 값 바꿔줌 ♣♣♣
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//             5) Serial Protocol(,를 기준으로 잘라서, 3개 string 배열 만들기) + Callback // 이거 왜 안되냐?? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
#include <string>

DigitalOut led1(LED1);
RawSerial pc(USBTX, USBRX); // mbed-os5에서는 RawSerial 사용

volatile bool gotPacket = false;
volatile float data[3];

void onSerialRx(){
    static char serialInBuffer[32];
    static int serialCount = 0;

    while(pc.readable()){
        char byteIn = pc.getc();
        // 통신 데이터 마지막인 경우
        if(byteIn == '\n'){
            serialInBuffer[serialCount] = 0;
            float d0, d1, d2;
            if (sscanf(serialInBuffer, "%f, %f, %f", &d0, &d1, &d2) == 3){
                data[0] = d0;
                data[1] = d1;
                data[2] = d2;
                gotPacket = true;
            }
            serialCount = 0;
        }
        // 통신 데이터 마지막이 아닌 경우
        else {
            serialInBuffer[serialCount] = byteIn;
            if(serialCount < 32)
                serialCount++;
        }
    }
}

int main() {
    pc.attach(&onSerialRx);
    while(true) {
        if(gotPacket) {
            gotPacket = false;
            pc.printf("data = %.3f, %.3f, %.3f \n\r", data[0], data[1], data[2]);
        }
        led1 =! led1;
        wait(0.2);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//             6) Serial Protocol(,를 기준으로 잘라서, 3개 string 배열 만들기) + Callback // 이거 왜 되냐??? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"

DigitalOut led1(LED1);
RawSerial pc(USBTX, USBRX); // mbed-os5에서는 RawSerial 사용

volatile bool gotPacket = false;
volatile float data[3];

void onSerialRx(){
    static char serialInBuffer[32];
    static int data_cnt = 0, buff_cnt = 0;

    if(pc.readable()){
        char byteIn = pc.getc();
        if(byteIn == ','){
            serialInBuffer[buff_cnt]='\0';
            data[data_cnt++]=atof(serialInBuffer);
            buff_cnt = 0;
        }
        else if(byteIn=='\n'){
            serialInBuffer[buff_cnt] = '\0';
            data[data_cnt]=atof(serialInBuffer);
            buff_cnt=0; data_cnt=0;
            gotPacket = true;
        }
        else{
            serialInBuffer[buff_cnt++]=byteIn;
        }
    }
}

int main() {
    pc.attach(&onSerialRx);
    while(true) {
        if(gotPacket) {
            gotPacket = false;
            pc.printf("data = %.3f, %.3f, %.3f \n\r", data[0], data[1], data[2]);
        }
        led1 =! led1;
        wait(0.2);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● I2C
//     2. I2C 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         1) 미지 주소 장치 검색
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*  // mbed-os5 버전 : i2c에 데이터 배열 설정하지 않으면 오류 발생 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"

I2C i2c(I2C_SDA, I2C_SCL);

int main(){
    uint8_t count = 0;
    while(1){
        printf("\nScanning I2C devices....\n");
        count = 0;
        char data[1];
        for(int adr = 0; adr < 256; adr++){
            // if(i2c.read(adr,0,0) == 0){
            if(i2c.read(adr,data,1,0) == 0){
                count++;
                printf("found @ 0x%02X \n", adr);
            }
            wait(0.005);
        }
        printf("\n Total %2d deviceds are found\n", count);
        wait(1.0);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         2) RTC 모듈 : 시간 출력
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*  // mbed-os5 버전 : i2c에 데이터 배열 설정하지 않으면 오류 발생 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
// 레지스터 주소 선언
#define REG_SEC (0x02) // 레지스터 datasheet 참조 ♣♣♣
#define REG_MIN (0x03)
#define REG_HOUR (0x04)
#define PCF8563_ADDR (0xA2)

I2C i2c(I2C_SDA, I2C_SCL);

void writeByte(uint8_t address, uint8_t regAddress, uint8_t data);
char readByte(uint8_t address, uint8_t regAddress);
uint8_t BCDToDec(uint8_t val);
uint8_t DecToBCD(uint8_t val);

int main(){
    while(1){
        // sec, min, hour 받기
        char s = readByte(PCF8563_ADDR, REG_SEC);
        char m = readByte(PCF8563_ADDR, REG_MIN);
        char h = readByte(PCF8563_ADDR, REG_HOUR);

        // sec, min, hour 출력
        printf("Time = %02u:%02u:%02u\n", BCDToDec(h & 0x3F), BCDToDec(m & 0x7F), BCDToDec(s & 0x7F)); // 0x3F : 6비트 마스크, 0x7F : 7비트 마스크, %02u : 2자리 차지 + 10의 자리수 없는 경우 0으로 채움 + unsigned형 ♣♣♣
        wait(1.0);
    }
}

// 쓰기 to 레지스터
void writeByte(uint8_t address, uint8_t regAddress, uint8_t data){
    char data_write[2];
    data_write[0] = regAddress;
    data_write[1] = data;
    i2c.write(address, data_write, 2, 0);
}

// 읽기 from 레지스터
char readByte(uint8_t address, uint8_t regAddress){
    char data[1];
    char data_write[1];
    data_write[0] = regAddress;
    i2c.write(address, data_write, 1, 1);
    i2c.read(address, data, 1, 0);
    return data[0];
}

// 8비트 BCD <-> 10진수 변환
uint8_t BCDToDec(uint8_t val){
    return ((val/16)*10 + (val%16));
}

uint8_t DecToBCD(uint8_t val){
    return ((val/10)*16 + (val%10));
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         3) OLED 모듈 : 문자 출력
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*  // 한 프로젝트에 main문이 2개 있으면 컴파일 에러 발생 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
#include "Adafruit_SSD1306.h"

I2C i2c(I2C_SDA, I2C_SCL);
Adafruit_SSD1306_I2c myOled(i2c, D4, 0x78, 64, 128);

int main(){
    uint16_t q = 0;
    i2c.frequency(400000);

    // 문자 출력
    myOled.begin();
    myOled.printf("%ux%u\n Hello World\r\n", myOled.width(), myOled.height());
    myOled.printf("First sample\r\n");
    myOled.display();
    while(1){
        myOled.printf("%u\r", q++);
        myOled.display();
        printf("q = %u \n", q);
        wait(1.0);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         3) OLED 모듈 : 그래픽 출력
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
#include "Adafruit_SSD1306.h"

I2C i2c(I2C_SDA, I2C_SCL);
Adafruit_SSD1306_I2c myOled(i2c, D4, 0x78, 64, 128);

int main(){
    int16_t x = 0;
    float inc = 0.0f, force = 1.0f;
    i2c.frequency(400000);

    // 문자 출력
    myOled.begin();
    myOled.printf("Hello World\r\n");
    myOled.display();
    while(1){
        // 박스 출력
        myOled.fillRect(0, 19, 128, 12, 0);
        myOled.drawRect(x, 20, 10, 10, 1);
        myOled.display();

        // 박스 좌표
        inc+=0.1f*force;
        x+=inc;
        if(118 < x || x < 0){
            force = -force;
            inc = 0.0f;
        }
        wait(0.01);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         3) OLED + RTC 모듈 : 시계 만들기
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
#include "Adafruit_SSD1306.h"
#define REG_SEC (0x02)
#define REG_MIN (0x03)
#define REG_HOUR (0x04)
#define PCF8563_ADDR (0xA2)

I2C i2c(I2C_SDA, I2C_SCL);
DigitalOut myled(LED1);
Adafruit_SSD1306_I2c Oled(i2c, D4, 0x78, 32, 128);

void writeByte(uint8_t address, uint8_t regAddress, uint8_t data);
char readByte(uint8_t address, uint8_t regAddress);
uint8_t BCDToDec(uint8_t val);
uint8_t DecToBCD(uint8_t val);

int main(){
    // 텍스트 출력
    Oled.begin();
    Oled.setTextSize(1);
    Oled.printf("15/MAY/2020\r\n\n");

    // 시간 출력
    Oled.setTextSize(2);
    while(1){
        // sec, min, hour 받기
        char s = readByte(PCF8563_ADDR, REG_SEC);
        char m = readByte(PCF8563_ADDR, REG_MIN);
        char h = readByte(PCF8563_ADDR, REG_HOUR);

        // sec, min, hour 출력
        Oled.printf("%02u:%02u:%02u\r", BCDToDec(h&0x3F), BCDToDec(m&0x7F), BCDToDec(s&0x7F));
        Oled.display();
    }
}

// 쓰기 to 레지스터
void writeByte(uint8_t address, uint8_t regAddress, uint8_t data){
    char data_write[2];
    data_write[0] = regAddress;
    data_write[1] = data;
    i2c.write(address, data_write, 2, 0);
}

// 읽기 from 레지스터
char readByte(uint8_t address, uint8_t regAddress){
    char data[1];
    char data_write[1];
    data_write[0] = regAddress;
    i2c.write(address, data_write, 1, 1);
    i2c.read(address, data, 1, 0);
    return data[0];
}

// 8비트 BCD <-> 10진수 변환
uint8_t BCDToDec(uint8_t val){
    return ((val/16)*10 + (val%16));
}

uint8_t DecToBCD(uint8_t val){
    return ((val/10)*16 + (val%10));
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● SPI
//     2. SPI 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         1) 1바이트 읽기
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*  // 아직 안돌려봄 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
#define WHO_AM_I 0x75                   // register address
#define SPI_READ 0x80                   // register address 최상위 비트 1로 만들기
SPI spi(D11, D12, D13);                 // mosi, miso, sclk
// SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK) // same pin
DigitalOut ss(D2);

int main(){
    ss = 1;
    spi.format(8, 3);
    spi.frequency(15000000);
    while(true){
        ss = 0;
        spi.write(WHO_AM_I | SPI_READ);
        int whoami = spi.write(0);
        printf("WHOAMI register = 0x%X\n", whoami);
        ss = 1;
        wait(0.1);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         1) 1바이트 읽기 class화
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*  // 아직 안돌려봄 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
#define WHO_AM_I 0x75
#define SPI_READ 0x80

class Dummy{
    SPI _spi;
    DigitalOut _ss;
    public:
        Dummy(PinName mosi, PinName miso, PinName clk, PinName ss):_spi(mosi, miso, clk), _ss(ss){
            _ss = 1;
            _spi.format(8, 3);
            _spi.frequency(15000000);
        }
    uint8_t readByte(uint8_t address){
        _ss = 0;
        _spi.write(address | SPI_READ);
        int data = _spi.write(0);
        _ss = 1;
        return data;
    }
};

Dummy dum(D11, D12, D13, D2);
Serial pc(USBTX, USBRX, 115200);

int main(){
    while(true){
        pc.printf("WHOAMI register = 0x%X\n", dum.readByte(WHO_AM_I));
        wait(0.1);
    }
}
*/


// ● Bus
//     2. BusIn, BusOut 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         1) RGB LED 순서대로 켜기
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
#define LED_PINS 3

BusOut leds(D4, D5, D6);
int main(){
    leds = 0x07; // 전부 on
    while(true){
        for(int i = 0; i < LED_PINS; i++){
            leds[i] = 0; // off
            wait(0.1);
            leds[i] = 1; // on
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         2) RGB LED 무작위로 켜기
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
#include "mbed.h"
#include <cstdlib>
#define LED_PINS 3

BusOut leds(D4, D5, D6);
int main(){
    leds = 0x07; // 전부 on
    while(true){
        for(int i = 0; i < LED_PINS; i++){
            leds = rand(); // 선택적 off
            wait(1);
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         3) RGB LED 패턴대로 켜기
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/* // 이거 왜 안되지??? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
#define LED_PINS 3
#define Ndata 8

Ticker tic;
BusOut leds(D4, D5, D6);
uint16_t data[] = {0b000, 0b001, 0b010, 0b100, 0b011, 0b101, 0b110, 0b111};
volatile int cnt = 0;
void colorGen(){
    leds = data[cnt];
    cnt++;
    cnt %= Ndata;
}

int main(){
    leds = 0x07; // 전부on
    tic.attach(&colorGen, 1.0);
    while(true){}
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         4) RGB LED 푸시버튼으로 켜기
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/* // 이거 왜 안되지??? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"

BusIn pbs(D14, D15);
BusOut leds(D4, D5, D6);
uint16_t data[] = {0b001, 0b010, 0b100};

int main(){
    pbs.mode(PullUp);
    while(true){
        leds = data[pbs];
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         5) 16진수 순차적으로 FND에 출력
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*      // 이거 안해봄 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"

BusOut fnd(D2, D3, D4, D5, D6, D7, D8, D9);
uint16_t num[] = {0x88, 0xBE, 0xC4, 0x94, 0xB2, 0x81, 0xBC, 0x80, 0xB0, 0x84, 0x83, 0xC7, 0x86, 0xC1, 0xE1};    // 애노드 타입 1 ~ F ♣

int main(){
    fnd = 0xFF;                 // 애노드 타입인데 전부1이므로 전부 끈 상태로 시작 ♣
    uint16_t cnt = 0;
    while(true){
        fnd = num[cnt++%16];
        wait(0.5);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         6) 4개 푸시버튼을 2진수로 생각하여 해당하는 16진수 FND에 출력
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*      // 이거 안해봄 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
BusIn pbs(D10, D11, D12, D13);
BusOut fnd(D2, D3, D4, D5, D6, D7, D8, D9);
uint16_t num[] = {0x88, 0xBE, 0xC4, 0x94, 0xB2, 0x81, 0xBC, 0x80, 0xB0, 0x84, 0x83, 0xC7, 0x86, 0xC1, 0xE1};    // 애노드 타입 1 ~ F ♣

int main(){
    pbs.mode(PullUp);           // Pullup ♣
    fnd = num[0];
    while(true){
        fnd = num[pbs^0x0F];    // 0인 마스크만 1, 나머지는 0으로 변환하여 16진수 만듦 ♣
        wait(0.1);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//         7) 외곽 회전 FND / 4개 푸시버튼 : 주기 조절
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*      // 이거 안해봄 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
BusIn pbs(D10, D11, D12, D13);
BusOut fnd(D2, D3, D4, D5, D6, D7, D8, D9);
uint16_t seg[] = {~0x01, ~0x40, ~0x20, ~0x10, ~0x04, ~0x02};    // 애노드 타입이므로 1개만 0으로, 나머지는 1로 해야 1개만 켜짐 ♣
Timer tmr;

int main(){
    pbs.mode(PullUp);               // Pullup ♣
    fnd = 0xFF;                     // 애노드 타입인데 전부1이므로 전부 끈 상태로 시작 ♣
    tmr.start();
    uint16_t n_cnt = 0, cnt = 0, intv = 1;

    while(true){
        if(tmr.read_us()>1000){     // 1ms마다 타이머 초기화 + 버튼에 따른 재설정 + FND 작동 ♣
            tmr.reset();
            switch(pbs^0x0f){
                case 1:
                    intv = 10;
                    break;
                case 2:
                    intv = 50;
                    break;
                case 4:
                    intv = 100;
                    break;
                case 8:
                    intv = 500;
                    break;
            }
            if(cnt % intv == 0){
                fnd = seg[n_cnt++%6];
            }
            cnt++;
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● RTOS
//     2. Thread 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/* 
#include "mbed.h"
DigitalOut led1(LED1), led2(D7);
Thread thread;
void led1_thread(){
    while (true){
        led1 =! led1;
        wait_us(100000);
    }
}
int main(){
    thread.start(&led1_thread);
    while (true){
        led2 =! led2;
        wait_us(500000);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/* 
#include "mbed.h"
Thread thread;
DigitalOut led1(LED1), led2(D7);
volatile bool running = true;
void blink(DigitalOut *led){
    while(running){
        *led =! *led;
        wait_us(20000);
    }
}
int main(){
    thread.start(callback(&blink, &led1));
    led2 =1;
    wait_us(5000000);
    running = false;
    thread.join();
    led2 = 0;
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● RTOS
//     2. Mutex 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
# include "mbed.h"
Mutex mutex;
Thread t2, t3;
int cnt = 0;
DigitalOut led(LED1);

void notify(const char* name, int ct){
    printf("%s : %3d\n\r", name, ct);
}
void thread_plus(const char *args){
    while(true){
        if(mutex.trylock_for(1)){ // true or false 반환
            led =! led;
            notify((const char*)args, ++cnt);
            mutex.unlock();
            wait_us(490000);
        }
    }
}
void thread_minus(const char *args){
    mutex.lock();
    notify((const char*)args, --cnt);
    mutex.unlock();
    wait_us(1000000);
}

int main(){
    t2.start(callback(&thread_plus, (const char*)"Th 2"));
    t3.start(callback(&thread_minus, (const char*)"Th 3"));

    while(true){
        thread_minus((const char *)"Th 1");
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● RTOS
//     2. Queue 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
# include "mbed.h"
typedef struct { // 구조체 생성
    float voltage, current;
    uint32_t counter;
} message_t;
MemoryPool<message_t, 16> mpool; // MemoryPool 생성
Queue<message_t, 16> queue; // Queue 생성
Thread thread;

void generator(void){
    uint32_t i = 0;
    while(true){
        i++;
        message_t *message = mpool.alloc(); // MemoryPool 주소 할당 to 구조체 변수 생성
        message -> voltage = (i * 0.1) * 33; // 구조체 변수 주소의 항목
        message -> current = (i * 0.1) * 11;
        message -> counter = i;
        queue.put(message); // Enqueue
        wait_us(1000000);
    }
}

int main(void){
    thread.start(&generator);
    while(true){
        osEvent evt = queue.get(); // queue 상태 반환
        if (evt.status == osEventMessage){
            message_t *message = (message_t*)evt.value.p;
            printf("\nVoltage : %.2f V\n", message -> voltage); // 구조체 변수 주소의 항목 
            printf("Current : %.2f A\n", message -> current);
            printf("Number of cycles : %u\n", message -> counter);
            mpool.free(message); // MemoryPool 주소 반환
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/* // 이거 왜 안되냐?? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
Serial pc(USBTX, USBTX);

MemoryPool<uint32_t,16> mpool; // MemoryPool 생성
Queue<uint32_t,16> queue; // Queue 생성

Thread thread;
Mutex uart_mutex;

DigitalOut led(LED1);

void send_thread(void){
    uint32_t i = 0;
    while(true){
        uint32_t *value = mpool.alloc(); // MemoryPool 주소 할당 to 구조체 변수 생성
        *value = i;
        queue.put(value); // Enqueue
        uart_mutex.lock();
        pc.printf("Thread : %d\n", *value);
        uart_mutex.unlock();
        wait_us(400000);
        i++;
    }
}

int main(){
    thread.start(&send_thread);
    while(true){
        osEvent evt = queue.get(0); // queue 상태 반환
        if(evt.status == osEventMessage){
            led =! led;
            uint32_t *value = (uint32_t*)evt.value.p;
            uart_mutex.lock();
            pc.printf("Main : %d\n", *value);
            uart_mutex.unlock();
            mpool.free(value); // MemoryPool 주소 반환
        }
        uart_mutex.lock();
        pc.printf("Main : wating... \n");
        uart_mutex.unlock();
        wait_us(200000);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/* // 이거 왜 안되냐?? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
Serial pc(USBTX, USBTX);

MemoryPool<uint32_t,16> mpool; // MemoryPool 생성
Queue<uint32_t,16> queue; // Queue 생성

Thread thread;
Mutex uart_mutex;

DigitalOut led(LED1);

void send_thread(void){
    uint32_t i = 0;
    while(true){
        uint32_t *value = mpool.alloc(); // MemoryPool 주소 할당 to 구조체 변수 생성
        *value = i;
        queue.put(value); // Enqueue
        uart_mutex.lock();
        pc.printf("Thread : %d\n", *value);
        uart_mutex.unlock();
        wait_us(400000);
        i++;
    }
}

int main(){
    thread.start(&send_thread);
    uint32_t cnt = 0;
    while(true){
        osEvent evt = queue.get(0); // queue 상태 반환
        if(evt.status == osEventMessage){
            led =! led;
            uint32_t *value = (uint32_t*)evt.value.p;
            uart_mutex.lock();
            pc.printf("Main : %d\n", *value);
            uart_mutex.unlock();
            mpool.free(value); // MemoryPool 주소 반환
        }
        if(cnt%20 == 0){
            uart_mutex.lock();
            pc.printf("Main : wating... \n");
            uart_mutex.unlock();
            wait_us(200000);
        }
        wait_us(200000);
        cnt++;
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● 초음파 센서
//     2. 초음파 센서 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//          1) 초음파 센서값 Oled 표시
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*      // 이거 안해봄 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
#include "Adafruit_SSD1306.h"
#include "sonar.h"

I2C i2c(I2C_SDA, I2C_SCL);
Serial pc(SERIAL_TX, SERIAL_RX);

DigitalOut myled(LED1);
Adafruit_SSD1306_I2c Oled(i2c, D4, 0x78, 32, 128);
Sonar sonar(D5, D6);

int main(){
    Oled.begin();
    Oled.setTextSize(1);
    Oled.printf("Distance\r\n\n");
    Oled.setTextSize(2);
    sonar.start();

    while(1){
        Oled.printf("%7.2fcm\r", sonar.read());
        Oled.display();
        pc.printf("%7.2fcm \r\n", sonar.read());
        wait(0.1f);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● CircularBuffer
//     2. CircularBuffer 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//          1) 1초마다 push + 사용자가 입력한 문자 push + 0.5초마다 pop
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*      // 이거 안해봄 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
#define BUF_SIZE 10

Serial pc(USBTX, USBRX);
DigitalOut led(LED1);
CircularBuffer<char, BUF_SIZE> buff;
Ticker tickman;

void dataArrived(){
    if(pc.readable()){
        char c = pc.getc();
        if(!buff.full()) buff.push(c);
    }
}
void pusher(){
    if(!buff.full()) buff.push('!');
}

int main(){
    tickman.attach(&pusher, 1.0);
    pc.attach(&dataArrived);
    char c;
    while(true){
        if(!buff.empty()){
            buff.pop(c);
            pc.printf("size : %u, got : '%c'\n", buff.size(), c);
        }
        led = buff.full()? 1:0;
        wait(0.5);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● StepMotor
//     2. StepMotor 실습
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//          1) 0 ~ 180도 왕복 StepMotor
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*      // 이거 안해봄 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
#include "DRV8825.h"

#define MOTOR_STEPS_PER_REV 400                                     // 1회전 당 모터 step 수 : 400
#define MICRO_STEP_DIV 32                                           // microstep 개수 : 32
#define MICRO_STEP_PER_REV (MOTOR_STEPS_PER_REV * MICRO_STEP_DIV)   // 1회전 당 모터 microstep 수 : 400 x 32 = 12800

#define DESIRED_STEPS_PER_SEC 50                                    // 초당 step 수 : 50
#define DESIRED_MICRO_STEPS_PER_SEC (32 * DESIRED_STEPS_PER_SEC)    // 초당 microstep 수 : 50 x 32 = 1600

#define SAMPLE_TIME (1000000L / DESIRED_MICRO_STEPS_PER_SEC)        // 1 microstep 주기 = 625us

#define MAX_STEP (MICRO_STEP_PER_REV / 2)                           // 0.5회전 당 모터 microstep 수 : 400 x 32 / 2 = 6400

Serial pc(USBTX, USBRX, 115200);

Timer tmr;
InterruptIn bot(BUTTON1);
DigitalOut led(LED1);
DRV8825 stepper(D3, D2);
volatile bool f_run = false;

void botPress(){
    f_run =! f_run;
}

int main(){
    bot.fall(&botPress);
    tmr.start();
    while(1){
        // 1microstep 샘플링 시간 < 타이머 시간 : step모터 회전 + 각도 출력
        if(tmr.read_us() > SAMPLE_TIME){
            tmr.reset();
            if(f_run){
                stepper.scan(0, MAX_STEP);
                int32_t steps = stepper.getSteps();
                // 1step 돌때마다 step 수 출력
                if(steps % MICRO_STEP_DIV == 0){
                    led != led;
                    pc.printf("step = %d\n", steps/MICRO_STEP_DIV);
                }
            }
        }
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● Encoder Sensor Project
//     2. Encoder Library
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*  // 이거 왜 안됨??? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include <mbed.h>
DigitalOut led1(LED1);
// extern DigitalOut led1; // 외부 변수 사용
class QEncoder{
    public:
        QEncoder(PinName pinA, PinName pinB) : _pinA(pinA), _pinB(pinB), _bi(pinA, pinB){}

        void init(){
            _pinA.rise(callback(this, &QEncoder::decode)); // 모든 핀의 edge에서 interrupt를 이용하여 decode함수 사용
            _pinA.fall(callback(this, &QEncoder::decode));
            _pinB.rise(callback(this, &QEncoder::decode));
            _pinB.fall(callback(this, &QEncoder::decode));
            _previousState = _bi; // 0 ~ 3까지 4가지 종류의 상태 중 하나
            _count = 0;
            _errorCount = 0;
        }

        int32_t getCount(){
            return _count;
        }

        void setCount(int32_t EncoderVal){
            _pinA.disable_irq(); // 엔코더값이 지속적으로 변화함. 이 때 값을 넣으려고 하면 오류 발생할 수 있으므로 
                                    // interrupt를 disable 시킨 후, 값 넣고 다시 재개(엔코더 끄고 값 넣고 킴)
            _pinB.disable_irq();
            _count = EncoderVal;
            _pinA.enable_irq();
            _pinB.enable_irq();
        }

        int32_t getErrorCount(){
            return _errorCount;
        }

    private: // 밖에서 건들 수 없도록 설정
        InterruptIn _pinA; // 예약만 해둔 상태
        InterruptIn _pinB;
        BusIn _bi;
        uint8_t _previousState;
        volatile int32_t _count;
        volatile int32_t _errorCount;

        void decode(){
            uint8_t newState = _bi;
            switch((_previousState << 2) | newState){ // 과거와 현재 상태를 한 변수에 넣음
                case 0b0001:
                case 0b0111:
                case 0b1110:
                case 0b1000:
                    _count --; // 역방향
                    break;
                case 0b0010:
                case 0b1011:
                case 0b1101:
                case 0b0100:
                    _count ++; // 정방향
                    break;
                default:
                    _errorCount++;
                    break;
            }
            _previousState = newState;
            led1 =! led1;
        }
};

QEncoder enco(D2, D3);
int main(){
    enco.init();
    enco.setCount(0);
    while(1){
        printf("count = %d, %d \n", enco.getCount(), enco.getErrorCount());
        wait(0.01);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
// ● DC Motor Drive
//     2. DC Motor Drive
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//          1) CW CCW 테스트
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*      // 이거 안해봄 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
#include "mbed.h"
PwmOut PWM(D10);
DigitalOut IN1(D8), IN2(D9);

int main(){
    IN1 = 1; IN2 = 0;
    while(1){
        IN1 = 1; IN2 = 0;   // CW
        PWM = 1.0;
        wait_ms(2000);
        IN1 = 0; IN2 = 1;   // CCW
        PWM = 0.5;
        wait_ms(2000);
        PWM = 0;
        wait_ms(2000);
    }
}
*/
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//          2) 라이브러리 생성
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ