// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "mbed.h"
#include "GP2A.h"

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기 값]
// thread
Thread snr;

// psd센서
GP2A psdl(PA_5, 30, 150, 60, 0);
GP2A psdf(PA_6, 30, 150, 60, 0);
GP2A psdr(PA_7, 30, 150, 60, 0);

double psdl_val;
double psdf_val;
double psdr_val;

// AC서보 모터
PwmOut Servo(PA_8);

PwmOut ServoL(PB_8);
PwmOut ServoR(PC_9);

float ang=90.0, inc_min = 1.5, inc=3;
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

// 기타
int mode = 0;

// [함수정의]
void sensor_print();

void servo_move(PwmOut &rc);
void servo_moveLR(PwmOut &rcL, PwmOut &rcR);
void servo_chk(PwmOut &rc);

void DC_set();
void DC_move(int _dirL, int _dirR, float _PwmL, float _PwmR);

void in_SerialRx();
void in_SerialRx_main();
void out_SerialRx();

// [main문]
int main(){
    // Servo 모터 세팅
    Servo.period_ms(10);
    ServoL.period_ms(10);
    ServoR.period_ms(10);

    uint16_t pulseW = map<float>(ang, 180., 0., 500., 2600.);
    uint16_t pulseWL = map<float>(angL, 180., 0., 500., 2600.);
    uint16_t pulseWR = map<float>(angR, 180., 0., 500., 2600.);
    Servo.pulsewidth_us(pulseW);
    ServoL.pulsewidth_us(pulseWL);
    ServoR.pulsewidth_us(pulseWR);

    // DC 모터 세팅
    PwmL.period_us(66);
    PwmR.period_us(66);

    ras.attach(&in_SerialRx); // interrupt 전용
    // com_th.start(&th_SerialRx); // thread 전용

    while(1){
        in_SerialRx_main(); // interrupt 전용
        if(All_move == true){
            // 서보 움직임
            servo_move(Servo);

            // 센서값
            psdl_val = psdl.getDistance();
            psdf_val = psdf.getDistance();
            psdr_val = psdr.getDistance();
            // sensor_print();

            if(mode == 0){
                pc.printf("mode = 0\n");
            }

            else if(mode == 1){
                pc.printf("mode = 1\n");
                // DC 모터 움직임
                if(ras_data[1] == 0){
                    if(ang < 70){
                        DC_move(0, 1, 0.20, 0.20);
                    }
                    else if(70 <= ang && ang < 110){
                        DC_move(1, 1, 0.20, 0.20);
                    }
                    else if(110 <= ang){
                        DC_move(1, 0, 0.20, 0.20);
                    } 
                }

                else if(ras_data[1] == 1){
                    if(ang < 85){
                        DC_move(0, 1, 0.08, 0.08);
                    }
                    else if(85 <= ang && ang < 95){
                        DC_move(1, 1, 0.08, 0.08);
                    }
                    else if(95 <= ang){
                        DC_move(1, 0, 0.08, 0.08);
                    }
                }

                else if(ras_data[1] == 2){
                    if(ang < 85){
                        DC_move(0, 1, 0.08, 0.08);
                    }
                    else if(85 <= ang && ang < 95){
                        DC_move(1, 1, 0, 0);
                        mode = 2;
                    }
                    else if(95 <= ang){
                        DC_move(1, 0, 0.08, 0.08);
                    }
                }

                else if(ras_data[1] == 9){
                    DC_move(1, 1, 0.20, 0.20);
                }
            }

            else if(mode == 2){
                pc.printf("mode = 2\n");
                // AC 서보모터 움직임
                // servo_chk(Servo); // Test 코드
                servo_moveLR(ServoL, ServoR);
                if(psdf_val <= 30){
                    ras.putc('1');
                    mode = 3;
                }
            }

            else if(mode == 3){
                pc.printf("mode = 3\n");
                // DC 모터 움직임
                if(ras_data[1] == 0){
                    if(ang < 70){
                        DC_move(0, 1, 0.20, 0.20);
                    }
                    else if(70 <= ang && ang < 110){
                        DC_move(1, 1, 0.20, 0.20);
                    }
                    else if(110 <= ang){
                        DC_move(1, 0, 0.20, 0.20);
                    }
                }

                else if(ras_data[1] == 1){
                    if(ang < 85){
                        DC_move(0, 1, 0.08, 0.08);
                    }
                    else if(85 <= ang && ang < 95){
                        DC_move(1, 1, 0.08, 0.08);
                    }
                    else if(95 <= ang){
                        DC_move(1, 0, 0.08, 0.08);
                    }
                }

                else if(ras_data[1] == 2){
                    if(ang < 85){
                        DC_move(0, 1, 0.08, 0.08);
                    }
                    else if(85 <= ang && ang < 95){
                        DC_move(1, 1, 0, 0);
                        mode = 1;
                    }
                    else if(95 <= ang){
                        DC_move(1, 0, 0.08, 0.08);
                    }
                }
                else if(ras_data[1] == 9){
                    DC_move(1, 1, 0.20, 0.20);
                }
            }
            All_move = false;
        }
    }
}

// [함수 정의]
// ir + psd 센서
void sensor_print(){
    printf("psdl_val : | %lf |\n", psdl_val);
    printf("psdf_val : | %lf |\n", psdf_val);
    printf("psdr_val : | %lf |\n", psdr_val);
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
            if (ang > 180.0f || ang < 0.0f){
                inc_min = -inc_min;
            }
        }
        else{
            inc = 5;
            if(ras_data[0] == 0){
                // pc.printf("2");
                ang = ang - inc;
            }
            else if(ras_data[0] == 1){
                // pc.printf("3");
                ang = ang;
                wait_ms(1500);
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
            if (ang > 180.0f || ang < 0.0f){
                inc = -inc;
            }
        }
        else if(ras_data[0] == 0){
            // pc.printf("2");
            ang = ang - inc;
        }
        else if(ras_data[0] == 1){
            // pc.printf("3");
            ang = ang;
        }
        else if(ras_data[0] == 2){
            // pc.printf("4");
            ang = ang + inc; 
        }

        if (ang > 180.f){
            ang = 180.0;
        }
        else if (ang < 0.0f){
            ang = 0.0;
        }
    }

    uint16_t pulseW = map<float>(ang, 180., 0., 500., 2600.);
    rc.pulsewidth_us(pulseW);
}

void servo_moveLR(PwmOut &rcL, PwmOut &rcR){
    if(ras_data[0] == 0){
        angL = 0.0;
        angR = 180.0;
    }
    else if(ras_data[0] == 1){
        angL = angL + incL;
        angR = angR - incR;
    }

    if (angL > 90.0){
        angL = 90.0;
    }
    else if (angL < 0.0){
        angL = 0.0;
    }
    if (angR < 90.0){
        angR = 90.0;
    }
    else if (angR > 180){
        angR = 180.0;
    }
    uint16_t pulseWL = map<float>(angL, 180., 0., 500., 2600.);
    uint16_t pulseWR = map<float>(angR, 180., 0., 500., 2600.);
    rcL.pulsewidth_us(pulseWL);
    rcR.pulsewidth_us(pulseWR);
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

void out_SerialRx(){
    ras.putc('\n');
}