// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "mbed.h"
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

GP2A psdb(PB_1, 10, 80, 22.5, 0.1606);

uint16_t ir_val[6];
// 0 : fl
// 1 : fr
// 2 : mr
// 3 : br
// 4 : bl
// 5 : ml
bool ir_res[6];
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
    // 없음 : 0
    // 왼쪽 : 1
    // 가운데 : 2
    // 오른쪽 : 3
// ras_data[1] : 상대 크기
    // 작음 : 1
    // 보통 : 2
    // 큼 : 3
    // 매우 큼 : 4
// ras_data[2] : 빨간영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 빨간 영역 안 : 0(상대 가까울 때만 공격)
    // 빨간 영역 밖 : 1(무조건 공격)
// ras_data[3] : 파란영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 파란 영역 안 : 0(그대로 밀면 됨)
    // 파란 영역 밖 : 1(파란 영역 안으로 넣으면 best)
int pre_data0 = 0;
// 상대 최근 위치 없음 : 0
// 상대 최근 위치 왼쪽 : 1
// 상대 최근 위치 가운데 : 2
// 상대 최근 위치 오른쪽 : 3

// 통신 - DC_chk
volatile bool gotPacket2 = false;
volatile float pc_data[3];

Mutex mutex;

// 기타
int mode = 0;

// [함수정의]
void sensor_read();
void sensor_cal();
void sensor_print();

void servo_set(PwmOut &rc);
void servo_move(PwmOut &rc);
void servo_chk(PwmOut &rc);

void DC_set();
void DC_move(int _dirL, int _dirR, float _PwmL, float _PwmR);
void DC_chk();

void in_SerialRx();
void in_SerialRx_main();
void th_SerialRx();

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
        sensor_print(); // 확인용 코드

        if(All_move == true){
            // servo_chk(Servo); // Test 코드
            // DC_chk(); // Test 코드

            // mutex.lock();
            // 초기 동작 : 상대 탐색
            if(mode == 0){
                if(ras_data[0] == 9){
                    DC_move(1, 0, 0.20, 0.20);  // 제자리 우회전
                }
                else if(ras_data[0] == 1){
                    DC_move(0, 1, 0.20, 0.20);  // 제자리 좌회전
                }
                else if(ras_data[0] == 2){
                    DC_move(1, 1, 0.0, 0.0);    // 정지
                    mode = 1;
                    // pc.printf("mode = 1");
                }
                else if(ras_data[0] == 3){
                    DC_move(1, 0, 0.20, 0.20);  // 제자리 우회전
                }
            }
            
            // 중간 동작 : 상대 탐색 + 원 회전 + 공격
            else if(mode == 1){
                // 서보모터 움직임
                servo_move(Servo);

                pc.printf("mode = 1 \n"); // 확인용 코드

                // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치
                if(ras_data[2] == 0){ // 상대방이 빨간색 끝좌표 안쪽에 위치
                    pc.printf("상대방이 빨간색 안에 위치,   "); // 확인용 코드
                    if(ang <= 75){ // 서보 왼쪽
                        // if(ras_data[1] == '1'){ // 화면 원통 작음
                            if(ras_data[0] == 9){ // 상대 안보임
                                pc.printf("상대 안보임 \n"); // 확인용 코드
                                if(pre_data0 == 1) DC_move(0, 1, 0.40, 40.0);
                                else if(pre_data0 == 2) DC_move(1, 0, 0.40, 0.40);
                                else if(pre_data0 == 3) DC_move(1, 0, 0.40, 0.40);
                                else DC_move(1, 0, 0.40, 0.40);
                            }
                            else if(ras_data[0] != 9){ // 상대 보임
                                pc.printf("상대 보임 \n"); // 확인용 코드
                                if(ir_res[2] == true && ir_res[3] == false && ir_res[4] == false && ir_res[5] == false){ // 왼쪽 앞 바퀴 : 전진
                                    DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == true && ir_res[3] == false && ir_res[4] == false && ir_res[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 : 전진
                                    DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == false && ir_res[4] == false && ir_res[5] == true){ // 왼쪽 뒷 바퀴 : 왼쪽 전진
                                    DC_move(1, 1, 0.10, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == true && ir_res[4] == false && ir_res[5] == false){ // 오른쪽 앞 바퀴 : 왼쪽 전진 // ● 불가능한 경우. 필요 없다
                                    // DC_move(1, 1, 0.15, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == true && ir_res[4] == true && ir_res[5] == false){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 전진 // ● 불가능한 경우. 필요 없다
                                    // DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == false && ir_res[4] == true && ir_res[5] == false){ // 오른쪽 뒷 바퀴 : 전진 // ● 불가능한 경우. 필요 없다
                                    // DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[1] == true){ // 왼쪽 앞 부분 + 오른쪽 앞 부분 : 왼쪽 회전
                                    DC_move(0, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == false && ir_res[4] == false && ir_res[5] == false){ // 모두 검은색 : 자유롭게 공격
                                    if(ang <= 30){
                                        speedL = map<float>(ang, 30.0, 0.0, 0.15, 0.50);
                                        DC_move(0, 1, speedL, 0.5);
                                    }
                                    else{
                                        speedL = map<float>(ang, 75.0, 30.0, 0.35, 0.15);
                                        DC_move(1, 1, speedL, 0.5);
                                    }
                                }
                                else{ // 그 외 : 왼쪽 전진
                                    DC_move(1, 1, 0.10, 0.20);
                                }
                            }
                        // }
                    }

                    else if(75 < ang && ang < 105){ // 서보 중간
                        // if(ras_data[1] == '1'){ // 화면 원통 작음
                            if(ras_data[0] == 9){ // 상대 안보임
                                pc.printf("상대 안보임 \n"); // 확인용 코드
                                if(pre_data0 == 1) DC_move(0, 1, 0.40, 40.0);
                                else if(pre_data0 == 2) DC_move(1, 0, 0.40, 0.40);
                                else if(pre_data0 == 3) DC_move(1, 0, 0.40, 0.40);
                                else DC_move(1, 0, 0.40, 0.40);
                            }
                            else if(ras_data[0] != 9){ // 상대 보임
                                pc.printf("상대 보임 \n"); // 확인용 코드
                                if(ir_res[2] == true && ir_res[3] == false && ir_res[4] == false && ir_res[5] == false){ // 왼쪽 앞 바퀴 : 오른쪽 전진 // ● 불가능한 경우. 필요 없다
                                    // DC_move(1, 1, 0.20, 0.10);
                                }
                                else if(ir_res[2] == true && ir_res[3] == false && ir_res[4] == false && ir_res[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 : 전진 // ● 불가능한 경우. 필요 없다
                                    // DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == false && ir_res[4] == false && ir_res[5] == true){ // 왼쪽 뒷 바퀴 : 왼쪽 전진 // ● 불가능한 경우. 필요 없다
                                    // DC_move(1, 1, 0.10, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == true && ir_res[4] == false && ir_res[5] == false){ // 오른쪽 앞 바퀴 : 전진
                                    DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == true && ir_res[4] == true && ir_res[5] == false){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 전진
                                    DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == false && ir_res[4] == true && ir_res[5] == false){ // 오른쪽 뒷 바퀴 : 오른쪽 전진
                                    DC_move(1, 1, 0.20, 0.10);
                                }
                                else if(ir_res[1] == true){ // 왼쪽 앞 부분 + 오른쪽 앞 부분 : 오른쪽 회전
                                    DC_move(1, 0, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == false && ir_res[4] == false && ir_res[5] == false){ // 모두 검은색 : 자유롭게 공격
                                    DC_move(1, 1, 0.50, 0.50);
                                }
                                else{ // 그 외 : 오른쪽 전진
                                    DC_move(1, 1, 0.20, 0.10);
                                }
                            }
                        // }
                    }

                    else if(105 <= ang){ // 서보 오른쪽
                        // if(ras_data[1] == '1'){ // 화면 원통 작음
                            if(ras_data[0] == 9){ // 상대 안보임
                                pc.printf("상대 안보임 \n"); // 확인용 코드
                                if(pre_data0 == 1) DC_move(0, 1, 0.40, 40.0);
                                else if(pre_data0 == 2) DC_move(1, 0, 0.40, 0.40);
                                else if(pre_data0 == 3) DC_move(1, 0, 0.40, 0.40);
                                else DC_move(1, 0, 0.40, 0.40);
                            }
                            else if(ras_data[0] != 9){ // 상대 보임
                                pc.printf("상대 보임 \n"); // 확인용 코드
                                if(ir_res[2] == true && ir_res[3] == false && ir_res[4] == false && ir_res[5] == false){ // 왼쪽 앞 바퀴 : 오른쪽 전진 // ● 불가능한 경우. 필요 없다
                                    // DC_move(1, 1, 0.20, 0.10);
                                }
                                else if(ir_res[2] == true && ir_res[3] == false && ir_res[4] == false && ir_res[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 : 전진 // ● 불가능한 경우. 필요 없다
                                    // DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == false && ir_res[4] == false && ir_res[5] == true){ // 왼쪽 뒷 바퀴 : 왼쪽 전진 // ● 불가능한 경우. 필요 없다
                                    // DC_move(1, 1, 0.10, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == true && ir_res[4] == false && ir_res[5] == false){ // 오른쪽 앞 바퀴 : 전진
                                    DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == true && ir_res[4] == true && ir_res[5] == false){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 전진
                                    DC_move(1, 1, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == false && ir_res[4] == true && ir_res[5] == false){ // 오른쪽 뒷 바퀴 : 오른쪽 전진
                                    DC_move(1, 1, 0.20, 0.10);
                                }
                                else if(ir_res[1] == true){ // 왼쪽 앞 부분 + 오른쪽 앞 부분 : 오른쪽 회전
                                    DC_move(1, 0, 0.20, 0.20);
                                }
                                else if(ir_res[2] == false && ir_res[3] == false && ir_res[4] == false && ir_res[5] == false){ // 모두 검은색 : 자유롭게 공격
                                    if(150 <= ang){
                                        speedR = map<float>(ang, 180.0, 150.0, 0.50, 0.15);
                                        DC_move(1, 0, 0.5, speedR);
                                    }
                                    else{
                                        speedR = map<float>(ang, 150.0, 105.0, 0.15, 0.35);
                                        DC_move(1, 1, 0.5, speedR);
                                    }
                                }
                                else{ // 그 외 : 오른쪽 전진
                                    DC_move(1, 1, 0.20, 0.10);
                                }
                            }
                        // }
                    }
                }

                // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 바깥에 위치
                else if(ras_data[2] == 1){
                    if(ras_data[0] == 9){ // 상대 보임
                        pc.printf("상대 안보임 \n"); // 확인용 코드

                        if(pre_data0 == 1) DC_move(0, 1, 0.40, 40.0);
                        else if(pre_data0 == 2) DC_move(1, 0, 0.40, 0.40);
                        else if(pre_data0 == 3) DC_move(1, 0, 0.40, 0.40);
                        else DC_move(1, 0, 0.40, 0.40);
                    }
                    else if(ras_data[0] != 9){ // 상대 안보임
                        pc.printf("상대 보임 \n"); // 확인용 코드

                        // 90 == 0
                        // 0 == 40
                        if(ang <= 75){
                            // speed = map<float>(ang, 75.0, 0.0, 0.20, 0.35);
                            // DC_move(0, 1, speed, speed);

                            if(ang <= 30){
                                speedL = map<float>(ang, 30.0, 0.0, 0.15, 0.50);
                                DC_move(0, 1, speedL, 0.5); // 제자리 좌회전
                            }
                            else{
                                speedL = map<float>(ang, 75.0, 30.0, 0.35, 0.15);
                                DC_move(1, 1, speedL, 0.5); // 좌 전진
                            }
                        }
                        else if(75 < ang && ang < 105){
                            DC_move(1, 1, 0.50, 0.50); // 직 전진
                        }
                        else if(105 <= ang){
                            // speed = map<float>(ang, 180.0, 105.0, 0.35, 0.20);
                            // DC_move(1, 0, speed, speed);

                            if(150 <= ang){
                                speedR = map<float>(ang, 180.0, 150.0, 0.50, 0.15);
                                DC_move(1, 0, 0.5, speedR); // 제자리 우회전
                            }
                            else{
                                speedR = map<float>(ang, 150.0, 105.0, 0.15, 0.35);
                                DC_move(1, 1, 0.5, speedR); // 우 전진
                            }
                        }
                    }
                }

                pc.printf("ang : %4f   ", ang);
                pc.printf("| speedL : %4f   ", speedL);
                pc.printf("| speedR : %4f   \n", speedR);
            }
            // mutex.unlock();

            All_move = false;
        }
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

    psdb_val = psdb.getDistance();
}

void sensor_cal(){
    if(ir_val[0] < black && ir_val[1] < black){
        ir_res[0] = true;
    }
    else ir_res[0] = false;
    if(ir_val[3] < black && ir_val[4] < black){
        ir_res[1] = true;
    }
    else ir_res[1] = false;
    if(ir_val[5] < black && ir_val[0] < black){
        ir_res[2] = true;
    }
    else ir_res[2] = false;
    if(ir_val[1] < black && ir_val[2] < black){
        ir_res[3] = true;
    }
    else ir_res[3] = false;
    if(ir_val[2] < black && ir_val[3] < black){
        ir_res[4] = true;
    }
    else ir_res[4] = false;
    if(ir_val[4] < black && ir_val[5] < black){
        ir_res[5] = true;
    }
    else ir_res[5] = false;
}

void sensor_print(){
    pc.printf("ir_val : | %u | %u | %u | %u | %u | %u |\n", ir_val[0], ir_val[1], ir_val[2], ir_val[3], ir_val[4], ir_val[5]);
    pc.printf("ir_res : | %d | %d | %d | %d | %d | %d |\n", ir_res[0], ir_res[1], ir_res[2], ir_res[3], ir_res[4], ir_res[5]);
    pc.printf("psd_val : | %lf |\n", psdb_val);
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
    // 중간 동작 : 화면 상대방 안보임
    if(ras_data[0] == 9){
        // pc.printf("1");
        if(pre_data0 == 1) ang = ang - inc;
        else if(pre_data0 == 2) ang = ang + inc;
        else if(pre_data0 == 3) ang = ang + inc;
        else ang = ang + inc;
    }

    // 중간 동작 : 화면 상대방 보임
    else if(ras_data[0] == 1){
        // pc.printf("2");
        if(ras_data[1] == 1) ang = ang - inc;
        else if(ras_data[1] == 2) ang = ang - inc;
        else if(ras_data[1] == 3) ang = ang - inc;
        else if(ras_data[1] == 4) ang = ang - inc;
        pre_data0 = 1;
    }
    else if(ras_data[0] == 2){
        // pc.printf("3");
        ang = ang;
        pre_data0 = 2;
    }
    else if(ras_data[0] == 3){
        // pc.printf("4");
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

    // bpc.printf("%f \n", ang); // 확인용 코드
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

void DC_move(int _dirL, int _dirR, float _PwmL, float _PwmR){
    DirL = _dirL;
    DirR = _dirR;
    PwmL = _PwmL;
    PwmR = _PwmR;
}

void DC_chk(){
    static char pc_serialInBuffer2[32];
    static int buff_cnt2 = 0;
        
    if(pc.readable()) {
        // pc.printf("%d \n", pc.readable());
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
            pc.printf("PWM = %.3f \n\r", pc_data[0]);
        }
    }

    if (0 <= pc_data[0] && pc_data[0] <= 95){
        PwmL = 0.01 * pc_data[0];
        PwmR = 0.01 * pc_data[0];
    }

    DirR = 1;
    DirL = 1;
    // PwmL = 0.3f;
    // PwmR = 0.3f;
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
            // pc.printf("byteIn == ',' : %c\n", byteIn); // 
            ras_serialInBuffer[ras_buff_cnt]='\0';
            ras_data[ras_data_cnt++]=atof(ras_serialInBuffer);
            ras_buff_cnt = 0;
        }
        // '/' : 시리얼 완료
        else if(byteIn=='/'){
            // pc.printf("byteIn == '/' : %c \n", byteIn); // 
            ras_serialInBuffer[ras_buff_cnt] = '\0';
            ras_data[ras_data_cnt]=atof(ras_serialInBuffer);
            ras_buff_cnt=0; ras_data_cnt=0;
            gotPacket = true;
        }
        // 이외 : 시리얼 저장
        else{
            // pc.printf("byteIn == '나머지' : %c\n", byteIn); // 
            ras_serialInBuffer[ras_buff_cnt++]=byteIn;
        }
    }
}

void in_SerialRx_main(){ // interrupt 전용
    // pc.printf("pc 연결 OK \n"); // pc, mbed 통신 출력
    if(gotPacket) {
        gotPacket = false;
        All_move = true;
        pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]); // 확인용 코드
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
            pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]); // 확인용 코드
        }

        // mutex.unlock();
    }
}