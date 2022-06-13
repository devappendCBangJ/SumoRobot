#include "mbed.h"
#include "GP2A.h"

// [통신 + 버튼 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// thread
Thread snr;

// 버튼
InterruptIn btn(BUTTON1);
int move_mode_cnt = 0;

// 기타
int mode = 0;

// psd센서
GP2A psd(A0, 7, 80, 24.6, -0.297);
double psd_val;

// AC서보 모터
PwmOut Servo(PA_8);

float ang=90.0, inc_min = 1.1, inc=2.0;
float angL=90.0, incL=0.015;
float angR=90.0, incR=0.015;

template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max);

// DC 모터
DigitalOut DirL(PC_7);
DigitalOut DirR(PB_6);
PwmOut PwmL(PB_4);
PwmOut PwmR(PB_5);

// 통신
RawSerial ras(PA_9, PA_10, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
RawSerial pc(USBTX, USBRX, 115200);    // RawSerial 클래스에는 scanf가 정의되어있지 않다.
RawSerial opcr(PC_10, PC_11, 115200);

// 통신 - ras_com
volatile bool All_move = false;
volatile bool gotPacket = false;
volatile float ras_data[3];

// ras_data[0] : 상대 위치
    // 왼쪽 : 0
    // 가운데 : 1
    // 오른쪽 : 2
// ras_data[1] : 상대 크기
    // 작음 : 0
    // 중간(정렬) : 1
    // 큼(잡아) : 2
// ras_data[2] : 물체 잡기 판단
    // 잡지마 : 0
    // 잡아 : 1
    // 파괴 : 2

// 통신 - DC_chk
volatile bool gotPacket2 = false;
volatile float pc_data[3];

// 통신 - opcr_com
char opcrIn = '0';

// [함수정의]
void sensor_print();

void servo_move(PwmOut &rc);
void servo_chk(PwmOut &rc);

void DC_set();
void DC_move(int _dirL, int _dirR, float _PwmL, float _PwmR);

void in_SerialRx();
void in_SerialRx_main();

void move_mode();

// [main문]
int main(){
    // Servo 모터 세팅
    Servo.period_ms(10);
    uint16_t pulseW = map<float>(ang, 180., 0., 500., 2600.);
    Servo.pulsewidth_us(pulseW);

    // DC 모터 세팅
    PwmL.period_us(66);
    PwmR.period_us(66);

    ras.attach(&in_SerialRx); // interrupt 전용
    // com_th.start(&th_SerialRx); // thread 전용

    // 버튼(동작 세팅)
    btn.fall(&move_mode);

    while(1){
        in_SerialRx_main(); // interrupt 전용
        if(All_move == true){
            // 서보 움직임
            servo_move(Servo);

            // 센서값
            psd_val = psd.getDistance();
            sensor_print();

            if(mode == 0){
                pc.printf("mode = 0\n");
            }

            else if(mode == 1){     // 접근 모드
                pc.printf("mode = 1\n");
                
                // DC 모터 움직임
                if(ras_data[1] == 0){               // 물체 크기가 작으면
                    if(ang < 70){
                        DC_move(0, 1, 0.13, 0.13);
                    }
                    else if(70 <= ang && ang < 110){
                        DC_move(1, 1, 0.13, 0.13);
                    }
                    else if(110 <= ang){
                        DC_move(1, 0, 0.13, 0.13);
                    }
                }
                
                else if(ras_data[1] == 1){          // 물체 크기가 중간이면
                    if(ang < 87){
                        DC_move(0, 1, 0, 0.09);
                    }
                    else if(87 <= ang && ang < 97){
                        DC_move(1, 1, 0.09, 0.09);
                    }
                    else if(97 <= ang){
                        DC_move(1, 0, 0.09, 0);
                    }
                }
                
                else if(ras_data[1] == 2){          // 물체 크기가 크면
                    if(ang < 90){
                        DC_move(0, 1, 0, 0.09);
                    }
                    else if(90 <= ang && ang < 94){
                        DC_move(1, 1, 0, 0);
                        mode = 2;
                    }
                    else if(94 <= ang){
                        DC_move(1, 0, 0.09, 0);
                    }

                    // opencr에 잡기 신호 전송
                    if(move_mode_cnt == 1){
                        opcr.putc('2'); // 벌리기

                        DC_move(1, 1, 0, 0);
                        wait_ms(1000);
                        ras.putc('1');
                        // pc.printf("hihi %c\n", opcrIn);  // 확인용 코드

                        // DC 모터 반바퀴 회전 = 2초 0.2, 0.2 회전
                        DC_move(1, 0, 0.00, 0.00);
                        wait_ms(2000);
                        DC_move(1, 0, 0.20, 0.20);
                        wait_ms(3000);
                        mode = 3;
                    }
                }
                
                else if(ras_data[1] == 9){      // 너무 멀면
                    DC_move(1, 1, 0.20, 0.20);
                }
            }
            
            else if(mode == 2){                 // 물체 잡기
                pc.printf("mode = 2\n");
                // AC 서보모터 움직임
                // servo_chk(Servo); // Test 코드
                
                if(psd_val>9.7){
                    DC_move(1, 1, 0.09, 0.09);
                }
                    
                else{                           // psd가 10.5이하값이면 모드 3으로 넘어감
                    // stop
                    DC_move(1, 1, 0.00, 0.00);

                    // opencr에 잡기 신호 전송
                    if(move_mode_cnt == 0){
                        opcr.putc('1'); // 잡기
                    }

                    pc.printf("%c\n", opcrIn);   // 확인용 코드

                    // 잡았으면, pi에 잡았다고 전송 + mode 변환
                    wait_ms(1000);
                    ras.putc('1');
                    // pc.printf("hihi %c\n", opcrIn);  // 확인용 코드

                    // DC 모터 반바퀴 회전 = 2초 0.2, 0.2 회전
                    DC_move(1, 0, 0.00, 0.00);
                    wait_ms(2000);
                    DC_move(1, 0, 0.20, 0.20);
                    wait_ms(3000);
                    mode = 3;
                    // }
                }
            }
            
            else if(mode == 3){                 // 복귀 모드
                pc.printf("mode = 3\n");

                // DC 모터 움직임
                if(ras_data[1] == 0){
                    if(ang < 70){
                        DC_move(0, 1, 0.13, 0.13);
                    }
                    else if(70 <= ang && ang < 110){
                        DC_move(1, 1, 0.13, 0.13);
                    }
                    else if(110 <= ang){
                        DC_move(1, 0, 0.13, 0.13);
                    }
                }

                else if(ras_data[1] == 1){
                    if(ang < 87){
                        DC_move(0, 1, 0, 0.09);
                    }
                    else if(87 <= ang && ang < 97){
                        DC_move(1, 1, 0.09, 0.09);
                    }
                    else if(97 <= ang){
                        DC_move(1, 0, 0.09, 0);
                    }
                }

                else if(ras_data[1] == 2){
                    if(ang < 90){
                        DC_move(0, 1, 0, 0.09);
                    }
                    else if(90 <= ang && ang < 94){
                        DC_move(1, 1, 0, 0);
                    }
                    else if(94 <= ang){
                        DC_move(1, 0, 0.09, 0);
                    }
                    mode = 4;
                }
                else if(ras_data[1] == 9){
                    DC_move(1, 1, 0.13, 0.13);
                }
            }
            else if(mode == 4){
                pc.printf("mode = 4\n");

                // opencr에 잡기 신호 전송
                if(move_mode_cnt == 0){
                    opcr.putc('2'); // 벌리기
                }
                DC_move(1, 1, 0, 0);
            }
            All_move = false;
        }
    }
}

// [함수 정의]
// move_mode 지정
void move_mode(){
    move_mode_cnt += 1;
}

// ir + psd 센서
void sensor_print(){
    pc.printf("psd_val : | %lf |\n", psd_val);
}

// AC 서보 모터
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servo_move(PwmOut &rc){
    if(mode == 0){
        if(ras_data[0] == 9){
            // pc.printf("1");
            ang += inc_min;
            pc.printf("ang : %f\n", ang);
            if (ang > 140.0f || ang < 40.0f){
                inc_min = -inc_min;
            }
        }
        else{
            inc = 4.0;
            if(ras_data[0] == 0){
                // pc.printf("2");
                ang = ang - inc;
            }
            else if(ras_data[0] == 1){
                // pc.printf("3");
                ang = ang;
                wait_ms(1500);

                // opencr에 잡기 신호 전송
                if(move_mode_cnt == 0){
                    opcr.putc('2'); // 벌리기
                }
                else if(move_mode_cnt == 1){
                    opcr.putc('1'); // 잡기
                }
                mode = 1;
            }
            else if(ras_data[0] == 2){
                // pc.printf("4");
                ang = ang + inc;
            }
        }
    }

    else if(mode != 0){
        if(ras_data[0] == 9){
            ang += inc;
            if (ang > 140.0f || ang < 40.0f){
                inc = -inc;
            }
        }
        else{
            inc = 4.0;
            inc_min = 1.1;
            if(ras_data[0] == 0){
            // pc.printf("2");
                if(ras_data[1] != 2){
                    ang = ang - inc;
                }
                else if(ras_data[1] == 2){
                    ang = ang - inc_min;
                }
            }
            else if(ras_data[0] == 1){
                // pc.printf("3");
                ang = ang;
            }
            else if(ras_data[0] == 2){
                // pc.printf("4");
                if(ras_data[1] != 2){
                    ang = ang + inc;
                }
                else if(ras_data[1] == 2){
                    ang = ang + inc_min; 
                }
            }

            if (ang > 140.f){
                ang = 140.0;
            }
            else if (ang < 40.0f){
                ang = 40.0;
            }
        }
    }

    uint16_t pulseW = map<float>(ang, 180., 0., 500., 2600.);
    rc.pulsewidth_us(pulseW);
}

void servo_chk(PwmOut &rc){
    angL += incL;
    angR += incR;
    if (angL > 180.f || angL < 0.f){
        incL = -incL;
    }
    if (angR > 180.f || angR < 0.f){
        incR = -incR;
    }
    uint16_t pulseWL = map<float>(angL, 0., 180., 500., 2600.);
    uint16_t pulseWR = map<float>(angR, 0., 180., 500., 2600.);
    rc.pulsewidth_us(pulseWL);
    rc.pulsewidth_us(pulseWR);
}

void DC_move(int _dirL, int _dirR, float _PwmL, float _PwmR){
    DirL = _dirL;
    DirR = _dirR;
    PwmL = _PwmL;
    PwmR = _PwmR;
}

// 통신(pc, ras, mbed)
void in_SerialRx(){ // interrupt 전용
    static char ras_serialInBuffer[32];
    static int ras_data_cnt = 0, ras_buff_cnt = 0;

    // pc.printf("%d", ras.readable()); // 확인용 코드
    if(ras.readable()){
        // pc.printf("Ras 연결 OK\n"); // Ras, mbed 통신 출력
        char byteIn = ras.getc();
        // pc.printf("%c", byteIn); // 확인용 코드

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
        pc.printf("ras_data = %.3f, %.3f, %.3f \n\r", ras_data[0], ras_data[1], ras_data[2]);
    }
}