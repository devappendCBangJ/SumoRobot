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
        watch.stop();
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


// ● Encoder Sensor Project
//     2. Encoder Library
// ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
/*
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