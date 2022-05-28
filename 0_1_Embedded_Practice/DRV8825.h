#include "mbed.h"

class DRV8825{
    BusOut _bus;
    enum {CW = 0, CCW = 1} _dir;
    int32_t _steps;

    public:
        DRV8825(PinName pin1, PinName pin2):_bus(pin1, pin2){
            _dir = CW;
            _steps = 0;
        }
        int32_t getSteps(){return _steps;};
        
        void step(){
            _bus = _dir ? 0b11:0b01;    // CCW : 0b11, CW : 0b01
            wait_us(1);                 // 최소 펄스 간격
            _bus = _dir ? 0b10:0b00;    // CCW : 0b10, CW : 0b00
            _steps += _dir ? -1:1;      // CCW : -1, CW : +1
        }
        void scan(int32_t step_min, int32_t step_max){
            if(_steps > step_max) _dir = CCW;
            else if(_steps <= step_min) _dir = CW;
            step();
        }
};