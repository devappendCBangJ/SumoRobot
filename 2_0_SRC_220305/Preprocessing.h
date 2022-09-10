# ifndef PREPROCESSING_H
# define PREPROCESSING_H

// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "mbed.h"
#include <stdlib.h>
#include "Servo.h"
#include "GP2A.h"
// #include <stdarg.h> // va_list, va_start, va_arg, va_end가 정의된 헤더파일

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// 모드
DigitalOut led1(LED1);
InterruptIn btn(BUTTON1);

Ticker mode_tic;

int mode = 0;

int tot_mode = 0;
int tic_cnt = 0;
int tic_even_cnt = 0;

// thread
Thread com_th;

Mutex mutex;

// ir센서 + psd센서
AnalogIn irfl(PA_0);
AnalogIn irfr(PA_1);
AnalogIn irmr(PA_4);
AnalogIn irbr(PB_0);
AnalogIn irbl(PC_1);
AnalogIn irml(PC_0);
AnalogIn irfm(PB_1);

AnalogIn psdf(PA_6);
GP2A psdb(PA_7, 10, 80, 22.5, 0.1606);

uint16_t ir_val[7];
// 0 : fl
// 1 : fr
// 2 : mr
// 3 : br
// 4 : bl
// 5 : ml
// 6 : fm
bool ir_WhCol[7];
// 0 : fl + fr
// 1 : bl + br
// 2 : ml + fl
// 3 : fr + mr
// 4 : mr + br
// 5 : bl + ml

double psdf_volts;
double psdf_val;
double psdb_val;
uint16_t black = 10000;

// AC서보 모터
PwmOut Servo(PA_8);

float ang = 90.0;
float inc = 3.5;
float small_inc = 3.5;
float big_inc = 5.5;
// float small_inc = 2.5;
// float big_inc = 4.2;

float angLL = 30;
float angML = 78;
float angMM = 90;
float angMR = 102;
float angRR = 150;

// DC 모터
DigitalOut DirL(PC_7);
DigitalOut DirR(PB_6);
PwmOut PwmL(PB_4);
PwmOut PwmR(PB_5);

double speed = 0;
double speedL = 0;
double speedR = 0;

template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max);

// 통신
RawSerial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다
RawSerial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다

// 통신 - ras_com
volatile bool All_move = false;
volatile bool gotPacket = false;
volatile float ras_data[3];
// ras_data[0] : 상대 방향 + 보임 유무
    // 왼쪽 : 0/11 ~ 4/11 (1 ~ 145)
    // 가운데 : 4/11 ~ 7/11 (146 ~ 254)
    // 오른쪽 : 7/11 ~ 11/11 (255 ~ 400)
    // 없음 : 999
// ras_data[1] : 상대 크기
    // 작음 : 1
    // 보통 : 2
    // 큼 : 3
    // 매우 큼 : 4
    // 없음 : 9
// ras_data[2] : 빨간영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 빨간 영역 안 : 0(상대 가까울 때만 공격)
    // 빨간 영역 밖 : 1(무조건 공격)
// ras_data[3] : 파란영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 파란 영역 안 : 0(그대로 밀면 됨)
    // 파란 영역 밖 : 1(파란 영역 안으로 넣으면 best)
int pre_data0 = 9;
// 상대 최근 위치 왼쪽 : 1
// 상대 최근 위치 가운데 : 2
// 상대 최근 위치 오른쪽 : 3
// 상대 최근 위치 없음 : 9

int width = 400;
int width_l = width * 4.5 / 11.0; // 세부조정 필요!!!
int width_r = width * 6.5 / 11.0; // 세부조정 필요!!!

// 통신 - DC_chk
volatile bool gotPacket2 = false;
volatile float pc_data[3];

// 타이머
Timer brk_tmr;
Timer fight_back_tmr;
Timer cam_tmr;

// int turn_escape_time = 25000;
// int back_escape_time = 100000;
int turn_escape_time = 1000000; // 세부조정 필요!!!
int back_escape_time = 1000000; // 세부조정 필요!!!
int fight_back_time = 500000; // 세부조정 필요!!!

// [함수 정의]
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