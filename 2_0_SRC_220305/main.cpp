// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "mbed.h"
#include <stdlib.h>
#include "Servo.h"
#include "GP2A.h"

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// thread
Thread com_th;

// ir센서 + psd센서
AnalogIn irfl(PA_0);
AnalogIn irfr(PA_1);
AnalogIn irmr(PA_4);
AnalogIn irbr(PB_0);
AnalogIn irbl(PC_1);
AnalogIn irml(PC_0);
AnalogIn irfm(PC_2);

GP2A psdb(PB_1, 10, 80, 22.5, 0.1606);

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
double psdb_val;
int black = 40000;

// AC서보 모터
PwmOut Servo(PA_8);

float ang = 90.0, inc = 3.5;
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

float speed = 0;
float speedL = 0;
float speedR = 0;

template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max);

// 통신
RawSerial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
RawSerial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.

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

// 기타
Timer tmr;
Mutex mutex;

int mode = 0;
// int turn_escape_time = 25000;
// int back_escape_time = 100000;
int turn_escape_time = 1000000; // 세부조정 필요!!!
int back_escape_time = 1000000; // 세부조정 필요!!!

// [함수정의]
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

void whl_bundle();

void all_print();

// [main문]
int main(){
    pc.format(8, SerialBase::Even, 1);

    Servo.period_ms(10);
    DC_set();
    servo_set(Servo);
    ras.attach(&in_SerialRx); // interrupt 전용

    // com_th.start(&th_SerialRx); // thread 전용
    while(1){
        in_SerialRx_main(); // interrupt 전용

        sensor_read();
        sensor_cal();
        // sensor_print(); // 확인용 코드

        if(All_move == true){
            // servo_chk(Servo); // Test 코드
            // DC_chk(); // Test 코드

            // mutex.lock();

            // 초기 동작 : 상대 탐색
            if(mode == 0){
                if(ras_data[0] == 999){ // 상대 안보임
                    speedL = 0.30; speedR = -0.30;
                }
                else if(ras_data[0] < width_l){ // 화면 왼쪽 보임
                    speedL = -0.30; speedR = 0.30;
                }
                else if(width_l <= ras_data[0] && ras_data[0] < width_r){ // 화면 가운데 보임
                    speedL = 0.0; speedR = 0.0;
                    mode = 1;
                    // pc.printf("mode = 1"); // 확인용 코드
                }
                else if(width_r <= ras_data[0]){ // 화면 오른쪽 보임
                    speedL = 0.30; speedR = -0.30;
                }
            }
            
            // 중간 동작 : 상대 탐색 + 원 회전 + 공격
            else if(mode == 1){
                // 서보모터 움직임
                servo_move(Servo);
                
                // pc.printf("mode = 1 \n"); // 확인용 코드

                if(ras_data[2] == 0){ // 상대방이 빨간색 끝좌표 안쪽에 위치
                    // pc.printf("상대방이 빨간색 안에 위치,   "); // 확인용 코드

                    // // 서보 왼쪽 최대 + 화면 매우 왼쪽 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치
                    // if(ang == 0 && ras_data[0] < width_l){ // 서보 왼쪽 최대 + 화면 매우 왼쪽 상대방 보임 // 상대 로봇이 클때의 경우도 and 조건으로 추가해야함!!!
                    //     tmr.start();
                    //     while(ir_val[6] < black){
                    //         speedL = -0.40; speedR = 0.40;

                    //         whl_bundle();
                    //         if(tmr.read_us() > turn_escape_time){
                    //             tmr.reset();
                    //             tmr.stop();
                    //             break;
                    //         }
                    //         // if(ras_data[1] == 4) break;
                    //     }
                    // }

                    // // 서보 오른쪽 최대 + 화면 매우 오른쪽 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치
                    // else if(ang == 180 && width_r < ras_data[0]){ // 서보 오른쪽 최대 + 화면 매우 오른쪽 상대방 보임 // 상대 로봇이 클때의 경우도 and 조건으로 추가해야함!!!
                    //     tmr.start();
                    //     while(ir_val[6] < black){
                    //         speedL = -0.40; speedR = 0.40;

                    //         whl_bundle();
                    //         if(tmr.read_us() > turn_escape_time){
                    //             tmr.reset();
                    //             tmr.stop();
                    //             break;
                    //         }
                    //         // if(ras_data[1] == 4) break;
                    //     }
                    // }

                    // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치 (ir 사용o. 전면 카메라 사용x)
                    if(ang <= angML){ // 서보 왼쪽
                        if(ras_data[1] == 9){ // 화면 원통 안보임
                            // pc.printf("상대 안보임 \n"); // 확인용 코드

                            if(pre_data0 == 1){
                                speedL = -0.40; speedR = 0.40;
                            }
                            else if(pre_data0 == 2){
                                speedL = 0.40; speedR = -0.40;
                            }
                            else if(pre_data0 == 3){
                                speedL = 0.40; speedR = -0.40;
                            }
                            else{
                                speedL = 0.40; speedR = -0.40;
                            }
                        }
                        else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                            // pc.printf("상대 보임 \n"); // 확인용 코드

                            if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    tmr.start();
                                    while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                        speedL = -0.65; speedR = -0.50;

                                        whl_bundle();
                                        if(tmr.read_us() > back_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                    }
                                }
                                else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                    if(ang <= angLL){
                                        speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                        speedR = 0.50;
                                    }
                                    else if(ang > angLL){
                                        speedL = map<float>(ang, angML, angLL, 0.25, 0.15);
                                        speedR = 0.50;
                                    }
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                    speedL = -0.40; speedR = -0.30;

                                    whl_bundle();
                                    if(tmr.read_us() > back_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 : 매우 오른쪽 전진
                                speedL = 0.30; speedR = 0.08;
                            }
                            else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_val[6] < black){
                                    speedL = -0.30; speedR = 0.30;

                                    whl_bundle();
                                    if(tmr.read_us() > turn_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                    // if(ras_data[1] == 4) break;
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 : 조금 왼쪽 전진
                                speedL = 0.15; speedR = 0.30;
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_val[6] < black){
                                    speedL = 0.30; speedR = -0.225;

                                    whl_bundle();
                                    if(tmr.read_us() > turn_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                    // if(ras_data[1] == 4) break;
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_val[6] < black){
                                    speedL = -0.225; speedR = 0.30;

                                    whl_bundle();
                                    if(tmr.read_us() > turn_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                    // if(ras_data[1] == 4) break;
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 매우 왼쪽 전진
                                speedL = 0.08; speedR = 0.30; // 0.225;
                            }
                            else if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                if(ang <= angLL){
                                    speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                    speedR = 0.5;
                                }
                                else if(ang > angLL){
                                    speedL = map<float>(ang, angML, angLL, 0.25, 0.15);
                                    speedR = 0.5;
                                }
                            }
                            else{ // 그 외 : 왼쪽 전진
                                speedL = 0.135; speedR = 0.30;
                            }
                        }
                        if(ras_data[1] == 4){ // 화면 매우 큼
                            if(abs(speedL) <= 0.55 && abs(speedR) <= 0.55){
                                speedL = speedL * (1.50);
                                speedR = speedR * (1.50);
                            }
                        }
                    }

                    else if(angML < ang && ang < angMR){ // 서보 중간
                        if(ras_data[1] == 9){ // 화면 원통 안보임
                            // pc.printf("상대 안보임 \n"); // 확인용 코드

                            if(pre_data0 == 1){
                                speedL = -0.40; speedR = 0.40;
                            }
                            else if(pre_data0 == 2){
                                speedL = 0.40; speedR = -0.40;
                            }
                            else if(pre_data0 == 3){
                                speedL = 0.40; speedR = -0.40;
                            }
                            else{
                                speedL = 0.40; speedR = -0.40;
                            }
                        }
                        else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                            // pc.printf("상대 보임 \n"); // 확인용 코드

                            if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    tmr.start();
                                    while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                        speedL = -0.65; speedR = -0.65;

                                        whl_bundle();
                                        if(tmr.read_us() > back_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                    }
                                }
                                else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 빠른 자유롭게 공격
                                    ///////////////////////////////////////////////////////
                                    // speedL = 0.50; speedR = 0.50;
                                    ///////////////////////////////////////////////////////
                                    speedL = 0.50; speedR = 0.50;
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                    speedL = -0.35; speedR = -0.35;

                                    whl_bundle();
                                    if(tmr.read_us() > back_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 : 매우 오른쪽 전진
                                speedL = 0.30; speedR = 0.08;
                            }
                            else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴 : 매우 왼쪽 전진
                                speedL = 0.08; speedR = 0.30;
                            }
                            else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞
                                if(ang >= 90){ // 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    tmr.start();
                                    while(ir_val[6] < black){
                                        speedL = 0.30; speedR = -0.30;

                                        whl_bundle();
                                        if(tmr.read_us() > turn_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                        // if(ras_data[1] == 4) break;
                                    }
                                }
                                else if(ang < 90){ // 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    tmr.start();
                                    while(ir_val[6] < black){
                                        speedL = -0.30; speedR = 0.30;

                                        whl_bundle();
                                        if(tmr.read_us() > turn_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                        // if(ras_data[1] == 4) break;
                                    }
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_val[6] < black){
                                    speedL = 0.30; speedR = -0.225;

                                    whl_bundle();
                                    if(tmr.read_us() > turn_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                    // if(ras_data[1] == 4) break;
                                }
                            }
                            else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 왼쪽 전진
                                speedL = 0.135; speedR = 0.30;
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_val[6] < black){
                                    speedL = -0.225; speedR = 0.30;

                                    whl_bundle();
                                    if(tmr.read_us() > turn_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                    // if(ras_data[1] == 4) break;
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 오른쪽 전진
                                speedL = 0.30; speedR = 0.135;
                            }
                            else if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                ///////////////////////////////////////////////////////
                                // speedL = 0.50; speedR = 0.50;
                                ///////////////////////////////////////////////////////
                                speedL = 0.30; speedR = 0.30;
                            }
                            else{ // 그 외 : 전진
                                speedL = 0.30; speedR = 0.30;
                            }
                        }
                        if(ras_data[1] == 4){ // 화면 원통 매우 큼
                            ///////////////////////////////////////////////////////
                            if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                speedL = 0.50; speedR = 0.50;
                            }
                            ///////////////////////////////////////////////////////

                            if(abs(speedL) <= 0.55 && abs(speedR) <= 0.55){
                                speedL = speedL * (1.50);
                                speedR = speedR * (1.50);
                            }
                        }
                    }
                    
                    else if(angMR <= ang){ // 서보 오른쪽
                        if(ras_data[1] == 9){ // 화면 원통 안보임
                            // pc.printf("상대 안보임 \n"); // 확인용 코드

                            if(pre_data0 == 1){
                                speedL = -0.40; speedR = 0.40;
                            }
                            else if(pre_data0 == 2){
                                speedL = 0.40; speedR = -0.40;
                            }
                            else if(pre_data0 == 3){
                                speedL = 0.40; speedR = -0.40;
                            }
                            else{
                                speedL = 0.40; speedR = -0.40;
                            }
                        }
                        else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                            // pc.printf("상대 보임 \n"); // 확인용 코드

                            if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 왼쪽 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    tmr.start();
                                    while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                        speedL = -0.50; speedR = -0.65;

                                        whl_bundle();
                                        if(tmr.read_us() > back_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                    }
                                }
                                else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                    if(angRR <= ang){
                                        speedL = 0.5;
                                        speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                    }
                                    else if(ang < angRR){
                                        speedL = 0.5;
                                        speedR = map<float>(ang, angRR, angMR, 0.15, 0.25);
                                    }
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 왼쪽 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                    speedL = -0.30; speedR = -0.40;

                                    whl_bundle();
                                    if(tmr.read_us() > back_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                }
                            }
                            else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴 : 매우 왼쪽 전진
                                speedL = 0.08; speedR = 0.30;
                            }
                            else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_val[6] < black){
                                    speedL = 0.30; speedR = -0.30;

                                    whl_bundle();
                                    if(tmr.read_us() > turn_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                    // if(ras_data[1] == 4) break;
                                }
                            }
                            else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 조금 오른쪽 전진
                                speedL = 0.30; speedR = 0.15;
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_val[6] < black){
                                    speedL = 0.30; speedR = -0.225;

                                    whl_bundle();
                                    if(tmr.read_us() > turn_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                    // if(ras_data[1] == 4) break;
                                }
                            }
                            else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                tmr.start();
                                while(ir_val[6] < black){
                                    speedL = -0.225; speedR = 0.30;

                                    whl_bundle();
                                    if(tmr.read_us() > turn_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                    // if(ras_data[1] == 4) break;
                                }
                            }
                            else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 매우 오른쪽 전진
                                speedL = 0.30; speedR = 0.08; // 0.225;
                            }
                            else if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                if(angRR <= ang){
                                    speedL = 0.5;
                                    speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                }
                                else if(ang < angRR){
                                    speedL = 0.5;
                                    speedR = map<float>(ang, angRR, angMR, 0.15, 0.25);
                                }
                            }
                            else{ // 그 외 : 오른쪽 전진
                                speedL = 0.30; speedR = 0.135;
                            }
                        }
                        if(ras_data[1] == 4){ // 화면 원통 매우 큼
                            if(abs(speedL) <= 0.55 && abs(speedR) <= 0.55){
                                speedL = speedL * (1.50);
                                speedR = speedR * (1.50);
                            }
                        }
                    }
                }

                // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 바깥에 위치
                // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 2개만 출력 + 상대방이 빨간색 끝좌표 안에 위치
                // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 빨간색 안보임
                else if(ras_data[2] == 1){ // 상대방이 빨간색 끝좌표 바깥에 위치
                    if(ras_data[1] == 9){ // 화면 원통 안보임
                        // pc.printf("상대 안보임 \n"); // 확인용 코드

                        if(pre_data0 == 1){
                            speedL = -0.40; speedR = 0.40;
                        }
                        else if(pre_data0 == 2){
                            speedL = 0.40; speedR = -0.40;
                        }
                        else if(pre_data0 == 3){
                            speedL = 0.40; speedR = -0.40;
                        }
                        else{
                            speedL = 0.40; speedR = -0.40;
                        }
                    }
                    else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                        // pc.printf("상대 보임 \n"); // 확인용 코드

                        if(ang <= angML){ // 서보 왼쪽
                            // speed = map<float>(ang, 75.0, 0.0, 0.20, 0.35);
                            // DC_move(0, 1, speed, speed);

                            if(angLL < ang){ // 서보 보통 왼쪽
                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        tmr.start();
                                        while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                            speedL = -map<float>(ang, angML, angLL, 0.60, 0.65);
                                            speedR = -0.5;

                                            whl_bundle();
                                            if(tmr.read_us() > back_escape_time){
                                                tmr.reset();
                                                tmr.stop();
                                                break;
                                            }
                                        }
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        speedL = map<float>(ang, angML, angLL, 0.25, 0.15);
                                        speedR = 0.5;
                                    }
                                }
                                else if(
                                    (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true) // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                ){
                                    tmr.start();
                                    while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                        speedL = -map<float>(ang, angML, angLL, 0.60, 0.65);
                                        speedR = -0.5;

                                        whl_bundle();
                                        if(tmr.read_us() > back_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                    }
                                }
                                else{ // 그 외 : 자유롭게 공격
                                    speedL = map<float>(ang, angML, angLL, 0.25, 0.15);
                                    speedR = 0.5;
                                }
                            }
                            else if(ang <= angLL){ // 서보 매우 왼쪽
                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        tmr.start();
                                        while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                            speedL = -map<float>(ang, angLL, 0.0, 0.65, 0.70);
                                            speedR = -0.5;

                                            whl_bundle();
                                            if(tmr.read_us() > back_escape_time){
                                                tmr.reset();
                                                tmr.stop();
                                                break;
                                            }
                                        }
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                        speedR = 0.5;
                                    }
                                } 
                                else if(
                                    (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true) // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                ){
                                    tmr.start();
                                    while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                        speedL = -map<float>(ang, angLL, 0.0, 0.65, 0.70);
                                        speedR = -0.5;

                                        whl_bundle();
                                        if(tmr.read_us() > back_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                    }
                                }
                                else{ // 그 외 : 자유롭게 공격
                                    speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                    speedR = 0.5;
                                }
                            }
                        }
                        else if(angML < ang && ang < angMR){ // 서보 중간
                            if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    tmr.start();
                                    while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                        speedL = -0.65;
                                        speedR = -0.65;

                                        whl_bundle();
                                        if(tmr.read_us() > back_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                    }
                                }
                                else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                    speedL = 0.5;
                                    speedR = 0.5;
                                }
                            }
                            else if(
                                (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                                (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                            ){
                                tmr.start();
                                while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                    speedL = -0.65;
                                    speedR = -0.65;

                                    whl_bundle();
                                    if(tmr.read_us() > back_escape_time){
                                        tmr.reset();
                                        tmr.stop();
                                        break;
                                    }
                                }
                            }
                            else{ // 그 외 : 자유롭게 공격
                                speedL = 0.5;
                                speedR = 0.5;
                            }
                        }
                        else if(angMR <= ang){ // 서보 오른쪽
                            // speed = map<float>(ang, 180.0, 105.0, 0.35, 0.20);
                            // DC_move(1, 0, speed, speed);

                            if(ang < angRR){ // 서보 보통 오른쪽
                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        tmr.start();
                                        while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                            speedL = -0.5;
                                            speedR = -map<float>(ang, angMR, angRR, 0.60, 0.65);

                                            whl_bundle();
                                            if(tmr.read_us() > back_escape_time){
                                                tmr.reset();
                                                tmr.stop();
                                                break;
                                            }
                                        }
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        speedL = 0.5;
                                        speedR = map<float>(ang, angRR, angMR, 0.15, 0.25);
                                    }
                                }
                                else if(
                                    (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 + 왼쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                ){
                                    tmr.start();
                                    while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                        speedL = -0.5;
                                        speedR = -map<float>(ang, angMR, angRR, 0.60, 0.65);

                                        whl_bundle();
                                        if(tmr.read_us() > back_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                    }
                                }
                                else{ // 그 외 : 자유롭게 공격
                                    speedL = 0.5;
                                    speedR = map<float>(ang, angRR, angMR, 0.15, 0.25);
                                }
                            }
                            else if(angRR <= ang){ // 서보 매우 오른쪽
                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        tmr.start();
                                        while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                            speedL = -0.5;
                                            speedR = -map<float>(ang, angRR, 180.0, 0.65, 0.70);

                                            whl_bundle();
                                            if(tmr.read_us() > back_escape_time){
                                                tmr.reset();
                                                tmr.stop();
                                                break;
                                            }
                                        }
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        speedL = 0.5;
                                        speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                    }
                                }
                                else if(
                                    (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 + 왼쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                ){
                                    tmr.start();
                                    while(ir_WhCol[2] == true || ir_WhCol[3] == true){
                                        speedL = -0.5;
                                        speedR = -map<float>(ang, angRR, 180.0, 0.65, 0.70);

                                        whl_bundle();
                                        if(tmr.read_us() > back_escape_time){
                                            tmr.reset();
                                            tmr.stop();
                                            break;
                                        }
                                    }
                                }
                                else{ // 그 외 : 자유롭게 공격
                                    speedL = 0.5;
                                    speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                }
                            }
                        }
                    }
                    if(ras_data[1] == 4){
                        if(abs(speedL) <= 0.55 && abs(speedR) <= 0.55){
                            speedL = speedL * (1.50);
                            speedR = speedR * (1.50);
                        }
                    }
                }
            }
            // mutex.unlock();
            if(abs(speedL) < 0.40 && abs(speedR) < 0.40){
                speedL = speedL * 1.50;
                speedR = speedR * 1.50;
            }
            
            DC_move(speedL, speedR);

            all_print();

            All_move = false;
        }

        // speedL = 0.18; speedR = 0.40;
        // DC_move(speedL, speedR);
    }
}

// [함수 정의]
// ir + psd 센서
void sensor_read(){
    ir_val[0] = irfl.read_u16();
    ir_val[1] = irfr.read_u16();
    ir_val[2] = irmr.read_u16();
    ir_val[3] = irbr.read_u16();
    ir_val[4] = irbl.read_u16();
    ir_val[5] = irml.read_u16();
    ir_val[6] = irfm.read_u16();

    psdb_val = psdb.getDistance();
}

void sensor_cal(){
    if(ir_val[0] < black && ir_val[1] < black){
        ir_WhCol[0] = true;
    }
    else ir_WhCol[0] = false;
    if(ir_val[3] < black && ir_val[4] < black){
        ir_WhCol[1] = true;
    }
    else ir_WhCol[1] = false;
    if(ir_val[5] < black && ir_val[0] < black){
        ir_WhCol[2] = true;
    }
    else ir_WhCol[2] = false;
    if(ir_val[1] < black && ir_val[2] < black){
        ir_WhCol[3] = true;
    }
    else ir_WhCol[3] = false;
    if(ir_val[2] < black && ir_val[3] < black){
        ir_WhCol[4] = true;
    }
    else ir_WhCol[4] = false;
    if(ir_val[4] < black && ir_val[5] < black){
        ir_WhCol[5] = true;
    }
    else ir_WhCol[5] = false;
}

void sensor_print(){
    pc.printf("ir_val : | %u | %u | %u | %u | %u | %u | %u |\n", ir_val[0], ir_val[1], ir_val[2], ir_val[3], ir_val[4], ir_val[5], ir_val[6]); // 확인용 코드
    pc.printf("ir_WhCol : | %d | %d | %d | %d | %d | %d |\n", ir_WhCol[0], ir_WhCol[1], ir_WhCol[2], ir_WhCol[3], ir_WhCol[4], ir_WhCol[5]); // 확인용 코드
    pc.printf("psd_val : | %lf |\n", psdb_val); // 확인용 코드
}

// AC 서보 모터
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servo_set(PwmOut &rc){
    uint16_t pulseW = map<float>(ang, 180., 0., 500., 2600.);
    rc.pulsewidth_us(pulseW);
}

void servo_move(PwmOut &rc){
    if(0 <= ras_data[0] && ras_data[0] < 200) inc = map<float>(ras_data[0], 200.0, 1.0, 3.5, 5.5);
    else if(ras_data[0] <=200 && ras_data[0] <= 400) inc = map<float>(ras_data[0], 200.0, 400.0, 3.5, 5.5);

    // 중간 동작 : 화면 상대방 안보임
    if(ras_data[0] == 999){
        // pc.printf("1"); // 확인용 코드
        inc = 5.5;

        if(pre_data0 == 1) ang = ang - inc;
        else if(pre_data0 == 2) ang = ang + inc;
        else if(pre_data0 == 3) ang = ang + inc;
        else ang = ang + inc;
    }

    // 중간 동작 : 화면 상대방 보임
    else if(ras_data[0] < width_l){
        // pc.printf("2"); // 확인용 코드
        if(ras_data[1] == 1) ang = ang - inc;
        else if(ras_data[1] == 2) ang = ang - inc;
        else if(ras_data[1] == 3) ang = ang - inc;
        else if(ras_data[1] == 4) ang = ang - inc;
        pre_data0 = 1;
    }
    else if(width_l <= ras_data[0] && ras_data[0] < width_r){
        // pc.printf("3"); // 확인용 코드
        ang = ang;
        pre_data0 = 2;
    }
    else if(width_r <= ras_data[0]){
        // pc.printf("4"); // 확인용 코드
        if(ras_data[1] == 1) ang = ang + inc;
        else if(ras_data[1] == 2) ang = ang + inc;
        else if(ras_data[1] == 3) ang = ang + inc;
        else if(ras_data[1] == 4) ang = ang + inc;
        pre_data0 = 3;
    }

    // 중간동작 : 서보모터 각도 제한
    if (ang > 180.f){
        ang = 180.0;
    }
    else if (ang < 0.0f){
        ang = 0.0;
    }
    uint16_t pulseW = map<float>(ang, 180., 0., 500., 2600.);
    rc.pulsewidth_us(pulseW);

    // pc.printf("%f \n", ang); // 확인용 코드
}

void servo_chk(PwmOut &rc){
    ang+=inc;
    if (ang > 180.f || ang < 0.f){
        inc = -inc;
    }
    uint16_t pulseW = map<float>(ang, 0., 180., 500., 2600.);
    rc.pulsewidth_us(pulseW);
}

// DC 모터
void DC_set(){
    PwmL.period_us(66); // 주파수 : 15kHz(모터드라이버 : 최대 주파수 20kHz)
    PwmR.period_us(66); // 주파수 : 15kHz(모터드라이버 : 최대 주파수 20kHz)
}

void DC_move(float _PwmL, float _PwmR){
    if(_PwmL < 0) DirL = 0;
    else DirL = 1;

    if(_PwmR < 0) DirR = 0;
    else DirR = 1;

    PwmL = abs(_PwmL);
    PwmR = abs(_PwmR);
}

void DC_chk(){
    static char pc_serialInBuffer2[32];
    static int buff_cnt2 = 0;
        
    if(pc.readable()) {
        // pc.printf("%d \n", pc.readable()); // 확인용 코드
        // pc.printf("pc 연결 OK \n"); // pc, mbed 통신 출력
        char byteIn2 = pc.getc();
        if(byteIn2=='\n'){
            // pc.printf("byteIn == '띄워' : %c \n", byteIn2); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
            pc_serialInBuffer2[buff_cnt2] = '\0';
            pc_data[0]=atof(pc_serialInBuffer2);
            buff_cnt2=0;
            gotPacket2 = true;
        }
        else{
            // pc.printf("byteIn == '나머지' : %c \n", byteIn2); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
            pc_serialInBuffer2[buff_cnt2++]=byteIn2;
        }

        if(gotPacket2) {
            gotPacket2 = false;
            // pc.printf("PWM = %.3f \n\r", pc_data[0]); // 확인용 코드
        }
    }

    if (0 <= pc_data[0] && pc_data[0] <= 95){
        PwmL = 0.01 * pc_data[0];
        PwmR = 0.01 * pc_data[0];
    }

    DirR = 1;
    DirL = 1;
}

// 통신(pc, ras, mbed)
void in_SerialRx(){ // interrupt 전용
    static char ras_serialInBuffer[32];
    static int ras_data_cnt = 0, ras_buff_cnt = 0;

    if(ras.readable()){
        // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
        char byteIn = ras.getc();
        // ',' : 단어 자르기
        if(byteIn == ','){
            // pc.printf("byteIn == ',' : %c\n", byteIn); // 확인용 코드
            ras_serialInBuffer[ras_buff_cnt]='\0';
            ras_data[ras_data_cnt++]=atof(ras_serialInBuffer);
            ras_buff_cnt = 0;
        }
        // '/' : 시리얼 완료
        else if(byteIn=='/'){
            // pc.printf("byteIn == '/' : %c \n", byteIn); // 확인용 코드
            ras_serialInBuffer[ras_buff_cnt] = '\0';
            ras_data[ras_data_cnt]=atof(ras_serialInBuffer);
            ras_buff_cnt=0; ras_data_cnt=0;
            gotPacket = true;
        }
        // 이외 : 시리얼 저장
        else{
            // pc.printf("byteIn == '나머지' : %c\n", byteIn); // 확인용 코드
            ras_serialInBuffer[ras_buff_cnt++]=byteIn;
        }
    }
}

void in_SerialRx_main(){ // interrupt 전용
    // pc.printf("pc 연결 OK \n"); // pc, mbed 통신 출력
    if(gotPacket) {
        gotPacket = false;
        All_move = true;
        // pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]); // 확인용 코드
    }
}

void th_SerialRx(){ // thread 전용
    static char ras_serialInBuffer[32];
    static int ras_data_cnt = 0, ras_buff_cnt = 0;

    while(true){
        // mutex.lock();
        if(ras.readable()){
            // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
            char byteIn = ras.getc();
            
            // ',' : 단어 자르기
            if(byteIn == ','){
                // pc.printf("byteIn == ',' : %c\n", byteIn); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
                ras_serialInBuffer[ras_buff_cnt]='\0';
                ras_data[ras_data_cnt++]=atof(ras_serialInBuffer);
                ras_buff_cnt = 0;
            }
            // '/' : 시리얼 완료
            else if(byteIn=='/'){
                // pc.printf("byteIn == '/' : %c \n", byteIn); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
                ras_serialInBuffer[ras_buff_cnt] = '\0';
                ras_data[ras_data_cnt]=atof(ras_serialInBuffer);
                ras_buff_cnt=0; ras_data_cnt=0;
                gotPacket = true;
            }
            // 이외 : 시리얼 저장
            else{
                // pc.printf("byteIn == '나머지' : %c\n", byteIn); // 중간에 이게 있으면 pc와 mbed 서로가 데이터 주려고 함. 연산량이 많아져서 데이터를 띄엄띄엄 받음
                ras_serialInBuffer[ras_buff_cnt++]=byteIn;
            }
        }

        if(gotPacket) {
            gotPacket = false;
            // pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]); // 확인용 코드
        }

        // mutex.unlock();
    }
}

void whl_bundle(){
    in_SerialRx_main(); // interrupt 전용

    sensor_read();
    sensor_cal();
    // sensor_print(); // 확인용 코드
    all_print();

    servo_move(Servo);
    DC_move(speedL, speedR);
}

void all_print(){
    pc.printf("ㅡㅡㅡㅡㅡ총합ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("mode = %d \n", mode); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ통신ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("ras_data = %.3f, %.3f, %.3f\n", ras_data[0], ras_data[1], ras_data[2]); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ센서ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    sensor_print(); // 확인용 코드
    pc.printf("ㅡㅡㅡㅡㅡ모터ㅡㅡㅡㅡㅡ\n"); // 확인용 코드
    pc.printf("ang = %f, inc = %f, speedL = %f, speedR = %f\n", ang, inc, speedL, speedR); // 확인용 코드
    pc.printf("=======================\n"); // 확인용 코드
}