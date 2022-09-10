# ifndef PREPROCESSING_H
# define PREPROCESSING_H

// [라이브러리]
#include "mbed.h"
#include <stdlib.h>
#include "Servo.h"
#include "GP2A.h"
// #include <stdarg.h> // va_list, va_start, va_arg, va_end가 정의된 헤더파일

// [함수 선언]
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max);

void sensor_read();
void sensor_cal();
void sensor_print();

void servo_set(PwmOut &rc);
void servo_move(PwmOut &rc);
void servo_chk(PwmOut &rc);

void DC_set();
void DC_move(float _PwmL, float _PwmR);
void DC_chk();

void in_SerialRx();
void in_SerialRx_main();
void th_SerialRx();

void btn_flip();
void led_flash();

void whl_bundle();

void all_print();

void normal_tmr_move(bool* _while_brk_sensor, const char* _inequality, bool _sensor_val, double _speedL, double _speedR);
void normal_tmr_move(uint16_t * _while_brk_sensor, const char* _inequality, uint16_t  _sensor_val, double _speedL, double _speedR);
void normal_tmr_move(double* _while_brk_sensor, const char* _inequality, double _sensor_val, double _speedL, double _speedR);

# endif