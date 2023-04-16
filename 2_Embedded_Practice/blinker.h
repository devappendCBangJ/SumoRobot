#include "mbed.h"

class Blinker{
    DigitalOut _led; // 가계약
    Ticker _tick;
    void blink(){
        _led =! _led;
    }
    public:
        Blinker(PinName pin): _led(pin){
            _led = 0;
        }
        void begin(float sec){
            _tick.attach(callback(this, &Blinker::blink), sec);
        }
};