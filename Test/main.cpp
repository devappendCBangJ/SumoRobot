#include "mbed.h"
#include "Adafruit_SSD1306.h"

// 상태
enum State {
    STATE_SHRINK, 
    STATE_STRETCH, 
    STATE_UP, 
    STATE_DOWN
};
State _state = STATE_SHRINK;

// 서보모터
PwmOut rcServo(D6);
void turn(PwmOut &rc, float deg);
template <class T> T map (T x, T in_min, T in_max, T out_min, T out_max);

// oled
I2C i2c(I2C_SDA, I2C_SCL);
Adafruit_SSD1306_I2c myOled(i2c, D4, 0x78, 64, 128);

// led, speaker, ang
Ticker tic1;
DigitalOut led(LED1);
int cnt = 0;

float ang=0.;
void tic_chg();

Ticker tic2;
DigitalOut speaker(D8);
void spk_chg();

// 버튼
InterruptIn btn(BUTTON1);

void irp(){
    switch(_state){
        case STATE_SHRINK:
            myOled.fillRect(0, 0, 128, 32, 0);
            _state = STATE_UP;
            break;
        case STATE_UP:
            myOled.fillRect(0, 0, 128, 32, 0);
            ang = 0;
            led = 0;
            _state = STATE_SHRINK;
            break;
        case STATE_STRETCH:
            myOled.fillRect(0, 0, 128, 32, 0);
            _state = STATE_DOWN;
            break;
        case STATE_DOWN:
            myOled.fillRect(0, 0, 128, 32, 0);
            ang = 0;
            led = 0;
            _state = STATE_SHRINK;
            break;
    }
}

int main(){
    // 서보모터 초기 설정
    rcServo.period_ms(10);
    turn(rcServo, 0);

    // i2c(oled 통신) 초기 설정
    i2c.frequency(400000);

    // 상태에 따른 서보모터 움직임
    while(1){
        switch(_state){
            case STATE_SHRINK:
                myOled.printf("SHRINK\r");
                break;
            case STATE_UP:
                myOled.printf("UP\r");
                break;
            case STATE_STRETCH:
                myOled.printf("STRETCH\r");
                break;
            case STATE_DOWN:
                myOled.printf("DOWN\r");
                break;
        }

        // 버튼 interrupt
        btn.fall(&irp);
        // tictok
        tic1.attach(&tic_chg, 0.02);
        tic2.attach(&spk_chg, 1.0/523.0/2.0); // 높은 레
        // 서보 모터 움직임
        turn(rcServo, ang);
        // oled 표시
        myOled.display();
        // printf("state read : %u\n", _state);
    }
}

// 서보 모터
template <class T> T map (T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void turn(PwmOut &rc, float deg){
    uint16_t pulseW = map<float>(deg, 0., 180., 600., 2400.);
    rc.pulsewidth_us(pulseW);
}

// led, ang
// void led_chg(){
//     led = !led;
// }
void tic_chg(){
    cnt++;
    switch(_state){
        case STATE_SHRINK:
            break;
        case STATE_UP:
            if(cnt % 5 == 0) led = !led;

            ang++;
            if(ang >= 180){
                ang = 180;
                led = 1;
                _state = STATE_STRETCH;
            }
            break;
        case STATE_STRETCH:
            break;
        case STATE_DOWN:
            if(cnt % 5 == 0) led = !led;

            ang--;
            if(ang <= 0){
                ang = 0;
                led = 0;
                _state = STATE_SHRINK;
            }
            break;
    }
}

void spk_chg(){
    switch(_state){
        case STATE_UP:
            speaker = !speaker;
            break;
        case STATE_DOWN:
            speaker = !speaker;
            break;
    }
}