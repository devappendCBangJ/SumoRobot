#include "mbed.h"

class Sonar {
    DigitalOut trigger;
    InterruptIn echo;

    Timer timer;
    Timeout timeout;
    Ticker ticker;

    int32_t pulse_begin;
    int32_t pulse_end;
    float all_time;

    public:
        Sonar(PinName trigger_pin, PinName echo_pin):trigger(trigger_pin), echo(trigger_pin){
                trigger = 0;
                all_time = -1;
                echo.rise(callback(this, &Sonar::echo_rise));   // echo핀 high로 바뀌면 타이머 시작 
                echo.fall(callback(this, &Sonar::echo_fall));   // echo핀 low로 바뀌면 타이머 종료
            }
            
            void start(void){           // 시작 : 100ms마다 trigger 초음파 발사
                ticker.attach(callback(this, &Sonar::periodic_start), 0.1f);
            }
            void stop(void){            // 종료 : trigger 초음파 발사 종료
                ticker.detach();
            }

            // [100ms마다 trigger 초음파 발사 + 10us후 trigger 초음파 발사 종료] 반복
            void periodic_start(void){
                trigger = 1;
                timeout.attach_us(callback(this, &Sonar::trigger_off), 10);
            }
            void trigger_off(void){
                trigger = 0;
            }

            // 체공시간 측정 + 계산
            void echo_rise(void){
                timer.reset();
                timer.start();
                pulse_begin = timer.read_us();
            }
            void echo_fall(void){
                pulse_end = timer.read_us();
                timer.stop();
                all_time = pulse_end - pulse_begin;
            }
            float read(void){
                return all_time / 58.0f;    // 시간 x 음속 / 2
            }
};