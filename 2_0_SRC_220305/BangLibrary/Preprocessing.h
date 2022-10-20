# ifndef PREPROCESSING_H
# define PREPROCESSING_H
// 헤더파일 : 선언

// [라이브러리]
#include "mbed.h"
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include "Servo.h" // Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html
#include "GP2A.h"
// #include <stdarg.h> // va_list, va_start, va_arg, va_end가 정의된 헤더파일

// [함수 선언]
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max);

void imu_read();
void sensor_read();
void sensor_cal();
void sensor_print();

void servo_set(PwmOut &rc);
void servo_move(PwmOut &rc);
void servo_chk(PwmOut &rc);

void DC_set();
void DC_move(float _PwmL, float _PwmR);
void DC_ratio_inc();
void DC_chk();

void in_SerialRx();
void in_SerialRx_main();
void th_SerialRx();

// template<class T> void fight_back_tmr_move(T* _while_brk_sensor, const char* _inequality, T _sensor_val, double _speedL, double _speedR);
// void fight_back_tmr_move(bool* _while_brk_sensor, const char* _inequality, bool _sensor_val, double _speedL, double _speedR);
// void fight_back_tmr_move(uint16_t * _while_brk_sensor, const char* _inequality, uint16_t  _sensor_val, double _speedL, double _speedR);
// void fight_back_tmr_move(double* _while_brk_sensor, const char* _inequality, double _sensor_val, double _speedL, double _speedR);

void init_move();
void wait_move();
void no_see_move();

void red_in_servo_left_can_see_move();
void red_in_servo_mid_can_see_move();
void red_in_servo_right_can_see_move();
void red_out_servo_all_can_see_move();

void color_escape_tmr_move();
void fight_back_tmr_move(Timer* _tmr, int* _time, int* _check_time, double _speedL, double _speedR);
void turn_90_tmr_move(Timer* _tmr, int* _time, uint16_t* _while_brk_sensor, uint16_t _sensor_val, volatile float* _com_data, double _speedL, double _speedR);
void turn_180_tmr_move(Timer* _tmr, int* _time, uint16_t* _while_brk_sensor, uint16_t _sensor_val, volatile float* _com_data, double _speedL, double _speedR);
void rotate_tmr_judgment();
void tilt_tmr_judgment();
void tilt_tmr_move();
void tmr_reset(Timer* _tmr);

void btn_flip();
void led_flash();

void whl_bundle();

void all_print();

extern double speedL;
extern double speedR;
extern float ratio;
template<class T> void back_tmr_move(Timer* _tmr, int* _time, T* _while_brk_sensor, const char* _inequality, T _sensor_val, double _speedL, double _speedR){
    _tmr->start(); // _tmr->start(); = *_tmr.start();
    if(_inequality[0] == '='){
        while(*_while_brk_sensor == _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(_tmr->read_us() > *_time){
                break;
            }
        }

        if(_speedL <= -0.95 || _speedR <= -0.95){
            ratio = 0;
        }
        _tmr->reset();
        _tmr->stop();
    }
    else if(_inequality[0] == '>'){
        while(*_while_brk_sensor > _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(_tmr->read_us() > *_time){
                break;
            }
        }

        if(_speedL <= -0.95 || _speedR <= -0.95){
            ratio = 0;
        }
        _tmr->reset();
        _tmr->stop();
    }
    else if(_inequality[0] == '<'){
        while(*_while_brk_sensor < _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(_tmr->read_us() > *_time){
                break;
            }
        }

        if(_speedL <= -0.95 || _speedR <= -0.95){
            ratio = 0;
        }
        _tmr->reset();
        _tmr->stop();
    }
}

# endif