// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

// [라이브러리]
#include "C:\Users\Hi\Mbed Programs\2_0_SRC_220305\BangLibrary\Preprocessing.h"  // 헤더파일 전처리
#include "C:\Users\Hi\Mbed Programs\2_0_SRC_220305\MPU9250\MPU9250.h"
// #include <math.h>

// [통신 + 타이머 + 모터 + 센서 class 선언 & 초기값]
// 모드
extern DigitalOut led1;
extern InterruptIn btn;

extern Ticker mode_tic;

extern int mode;

extern int tot_mode;
extern int tic_cnt;
extern int tic_even_cnt;

extern char pre_rotate_dir;
extern char rotate_dir;
extern char waiting_dir;

// 코드 위치 확인
extern int where;

// thread
extern Thread imu_th;

extern uint64_t Now_time, Work_time;

// Thread com_th;
// Mutex mutex;

// ir센서 + psd센서
extern AnalogIn irfl;
extern AnalogIn irfr;
extern AnalogIn irmr;
extern AnalogIn irbr;
extern AnalogIn irbl;
extern AnalogIn irml;
extern AnalogIn irfm;
extern AnalogIn irmr2;
extern AnalogIn irml2;

extern AnalogIn psdfm;
extern AnalogIn psdfl;
extern AnalogIn psdfr;
extern GP2A psdBb;
extern GP2A psdb;

extern uint16_t ir_val[9];
// 0 : fl
// 1 : fr
// 2 : mr
// 3 : br
// 4 : bl
// 5 : ml
// 6 : fm
// 7 : mr2
// 8 : ml2
extern bool ir_WhCol[7];
// 0 : fl + fr
// 1 : bl + br
// 2 : ml + fl
// 3 : fr + mr
// 4 : mr + br
// 5 : bl + ml

extern double psdfm_volts;
extern double psdfm_val;
extern double psdfl_volts;
extern double psdfl_val;
extern double psdfr_volts;
extern double psdfr_val;
extern double psdBb_val;
extern double psdb_val;
extern double psdb_list[5];
extern double psdb_pre_avg;
extern double psdb_now_avg;
extern int psdb_list_len;
extern int n;

extern uint16_t black;
extern uint16_t tilt_black;
extern double psdfm_dis;
extern double psdfl_fr_dis1;
extern double psdfl_fr_dis2;
extern double psdBb_dis;

// AC서보 모터
extern PwmOut Servo;

extern float ang;
extern float inc;
extern float small_inc;
extern float big_inc;

extern float angLL;
extern float angML;
extern float angMM;
extern float angMR;
extern float angRR;

// DC 모터
extern DigitalOut DirL;
extern DigitalOut DirR;
extern PwmOut PwmL;
extern PwmOut PwmR;

extern double speed;
extern double speedL;
extern double speedR;


extern float ratio;

// 통신
extern RawSerial ras;    // RawSerial 클래스에는 scanf가 정의되어있지 않다
extern RawSerial pc;    // RawSerial 클래스에는 scanf가 정의되어있지 않다
extern Serial blt;    // RawSerial 클래스에는 scanf가 정의되어있지 않다

// 통신 - ras_com
extern volatile bool All_move;
extern volatile bool gotPacket;
extern volatile float ras_data[4];
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
    // 매우 매우 큼 : 5
    // 매우 매우 매우 큼 : 6
    // 없음 : 9
// ras_data[2] : 빨간영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 빨간 영역 안 : 0(상대 가까울 때만 공격)
    // 빨간 영역 밖 : 1(무조건 공격)
// ras_data[3] : 파란영역 좌표 vs 상대 좌표 비교(거리, 영역)
    // 파란 영역 안 : 0(그대로 밀면 됨)
    // 파란 영역 밖 : 1(파란 영역 안으로 넣으면 best)
extern int pre_data0;
// 상대 최근 위치 왼쪽 : 1
// 상대 최근 위치 가운데 : 2
// 상대 최근 위치 오른쪽 : 3
// 상대 최근 위치 없음 : 9

extern int width;
extern int width_l; // 세부조정 필요!!!
extern int width_r; // 세부조정 필요!!!

// 통신 - DC_chk
extern volatile bool gotPacket2;
extern volatile float pc_data[3];

// 타이머
// ///////////////////////////////////////////////////
// Timer temp_tmr;
// ///////////////////////////////////////////////////

extern Timer control_tmr;
extern Timer brk_tmr;
extern Timer rotate_tmr;
extern Timer tilt_tmr;
extern Timer waiting_start_tmr;
extern Timer waiting_break_tmr;
extern Timer waiting_dir_tmr;
extern Timer com_check_tmr;
extern Timer escape_blue_all_tmr;
extern Timer escape_blue_go_tmr;

extern int turn_escape_time; // 세부조정 필요!!!
extern int back_escape_time; // 세부조정 필요!!!
extern int fight_back_escape_time; // 세부조정 필요!!!
extern int fight_back_break_check_time; // 세부조정 필요!!!
extern int rotate_recog_time; // 세부조정 필요!!!
extern int tilt_recog_time; // 세부조정 필요!!!
extern int waiting_start_time; // 세부조정 필요!!!
extern int waiting_break_time; // 세부조정 필요!!!
extern int waiting_dir_time; // 세부조정 필요!!!
extern int com_check_time; // 세부조정 필요!!!
extern double control_time; // 세부조정 필요!!!

extern int escape_blue_turn_time; // 세부조정 필요!!!
extern int escape_blue_go_time; // 세부조정 필요!!!
extern int imu_time;

///////////////////////////////////////////////////
extern double control_time; // 세부조정 필요!!!
///////////////////////////////////////////////////

// [MPU9250 초기값]
///////////////////////////////////////////////////
extern MPU9250 mpu9250;
extern float tilt_deg;
extern float tilt_break_deg;
///////////////////////////////////////////////////

extern uint8_t Ascale;     // AFS_2G, AFS_4G, AFS_8G, AFS_16G
extern uint8_t GscaleS; // GFS_250DPS, GFS_500DPS, GFS_1000DPS, GFS_2000DPS
extern uint8_t Mscale; // MFS_14BITS or MFS_16BITS, 14-bit or 16-bit magnetometer resolution
extern uint8_t Mmode;        // Either 8 Hz 0x02) or 100 Hz (0x06) magnetometer data ODR  
extern float aRes, gRes, mRes;      // scale resolutions per LSB for the sensors

//Set up I2C, (SDA,SCL)
extern I2C i2c;

extern DigitalOut myled;
    
// Pin definitions
extern int intPin;  // These can be changed, 2 and 3 are the Arduinos ext int pins

extern int16_t accelCount[3];  // Stores the 16-bit signed accelerometer sensor output
extern int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
extern int16_t magCount[3];    // Stores the 16-bit signed magnetometer sensor output
extern float magCalibration[3], magbias[3];  // Factory mag calibration and mag bias
extern float magScale[3];
// float gyroBias[3] = {0, 0, 0}, accelBias[3] = {0, 0, 0}; // Bias corrections for gyro and accelerometer
extern float gyroBias[3], accelBias[3]; // Bias corrections for gyro and accelerometer
extern float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values 
extern int16_t tempCount;   // Stores the real internal chip temperature in degrees Celsius
extern float temperature;
extern float SelfTest[6];

extern int delt_t; // used to control display output rate
extern int count;  // used to control display output rate

// parameters for 6 DoF sensor fusion calculations
extern float PI;
extern float GyroMeasError;     // gyroscope measurement error in rads/s (start at 60 deg/s), then reduce after ~10 s to 3
extern float beta;  // compute beta
extern float GyroMeasDrift;      // gyroscope measurement drift in rad/s/s (start at 0.0 deg/s/s)
extern float zeta;  // compute zeta, the other free parameter in the Madgwick scheme usually set to a small or zero value

extern float pitch, yaw, roll;
extern float pitch2, yaw2, roll2;
extern float AcX, AcY, GyX, GyY;
extern float roll_p, pitch_p;
extern float prev_x, prev_y;

extern float deltat;                             // integration interval for both filter schemes

// used to calculate integration interval                               // used to calculate integration interval
extern float q[4];           // vector to hold quaternion
extern float qq[4]; 
extern float e1, e2, e3;
extern float eInt[3];              // vector to hold integral error for Mahony method

// [main문]
int main(){
    // osThreadSetPriority(osThreadGetId(), osPriorityRealtime7);

    pc.format(8, SerialBase::Even, 1);

    Servo.period_ms(10);
    DC_set();
    servo_set(Servo);
    ras.attach(&in_SerialRx); // interrupt 전용

    btn.fall(&btn_flip);
    mode_tic.attach(&led_flash, 0.10);

    control_tmr.start();

    ///////////////////////////////////////////////////
    // mpu9250.resetMPU9250(); // Reset registers to default in preparation for device calibration
    // mpu9250.MPU9250SelfTest(SelfTest); // Start by performing self test and reporting values 
    // mpu9250.calibrateMPU9250(gyroBias, accelBias); // Calibrate gyro and accelerometers, load biases in bias registers  
    // ThisThread::sleep_for(100);
    mpu9250.initMPU9250();

    mpu9250.getAres(); // Get accelerometer sensitivity +-2g 4g 8g
    mpu9250.getGres(); // Get gyro sensitivity      250  500   1000
    ///////////////////////////////////////////////////

    imu_th.start(&imu_read);

    blt.printf("mbed reset\n"); // 확인용 코드

    // ///////////////////////////////////////////////////
    // const double PI = 3.1415926;

    // int theta = 0;
    // double total_time = 1000000;
    // double for_time = total_time / 180;
    // double a = cos(theta * PI / 180);
    // ///////////////////////////////////////////////////


    // com_th.start(&th_SerialRx); // thread 전용
    while(1){
        // all_print(); // 확인용 코드

        in_SerialRx_main(); // interrupt 전용

        sensor_read();
        sensor_cal();
        // sensor_print(); // 확인용 코드

        // blt.printf("%.1f, %.2f, %.2f\n", pitch_p, speedL, speedR); // 확인용 코드

        if(All_move == true){ // 통신 받음
            // all_print();

            // servo_chk(Servo); // Test 코드
            // DC_chk(); // Test 코드

            // pc.printf("tot_mode : %d \n", tot_mode); // 확인용 코드
            blt.printf("w%d\n", where);
            blt.printf("p%.2f\n", pitch_p);
            // blt.printf("b%d\n", brk_tmr.read_ms());
            // blt.printf("| %u | %u | %u | %u | %.2f\n", ir_val[7]/1000, ir_val[3]/1000, ir_val[4]/1000, ir_val[8]/1000, pitch_p);

            // mutex.lock(); // thread 전용

            DC_ratio_inc();

            if(tot_mode == 0){
                // blt.printf("%d, %.1f, %.2f, %.2f, %d, %d\n", mode, pitch_p, speedL, speedR, rotate_tmr.read_us(), tilt_tmr.read_us()); // 확인용 코드
                // blt.printf("%.2f, %.2f, %d, %.1f\n", speedL, speedR, rotate_tmr.read_us(), pitch_p); // 확인용 코드
                // blt.printf("%.2f\n", pitch_p); // 확인용 코드
                // blt.printf("%d\n", tilt_tmr.read_us()); // 확인용 코드

                // blt.printf("w%d\n", where); // 확인용 코드
                // if(where == 46 || where == 24 || where == 6) blt.printf("---%d---\n", where);
                // blt.printf("i%d\n", ir_val[6]);

                // blt.printf("r%d\n", rotate_tmr.read_ms()); // 확인용 코드
                // blt.printf("ir_val : | %u | %u | %u | %u | %u | %u | %u |\n", ir_val[0]/1000, ir_val[1]/1000, ir_val[2]/1000, ir_val[3]/1000, ir_val[4]/1000, ir_val[5]/1000, ir_val[6]/1000); // 확인용 코드
                
                // 초기 동작 : 상대 탐색
                if(mode == 0){
                    init_move();
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
                        //     brk_tmr.start();
                        //     while(ir_val[6] < black){
                        //         speedL = -0.40; speedR = 0.40;

                        //         whl_bundle();
                        //         if(brk_tmr.read_us() > turn_escape_time){
                        //             brk_tmr.reset();
                        //             brk_tmr.stop();
                        //             break;
                        //         }
                        //         // if(ras_data[1] == 4) break;
                        //     }
                        // }

                        // // 서보 오른쪽 최대 + 화면 매우 오른쪽 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치
                        // else if(ang == 180 && width_r < ras_data[0]){ // 서보 오른쪽 최대 + 화면 매우 오른쪽 상대방 보임 // 상대 로봇이 클때의 경우도 and 조건으로 추가해야함!!!
                        //     brk_tmr.start();
                        //     while(ir_val[6] < black){
                        //         speedL = -0.40; speedR = 0.40;

                        //         whl_bundle();
                        //         if(brk_tmr.read_us() > turn_escape_time){
                        //             brk_tmr.reset();
                        //             brk_tmr.stop();
                        //             break;
                        //         }
                        //         // if(ras_data[1] == 4) break;
                        //     }
                        // }

                        // 서보가 많이 돌아가있음(테스트 필요)
                        // 상대 감지됐는데 화면 매우 큼(테스트 필요)
                        // 벽에 박을 정도로 psd 앞 매우 가까이 감지. 앞판이 벽면에 완전히 박혀있을 때처럼 제자리 회전마저도 불가능한 경우 기준으로 측정(테스트 필요)
                        // 후진 1.0으로 밀어버림

                        // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치 (ir 사용o. 전면 카메라 사용x)
                        if(ang <= angML){ // 서보 왼쪽
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                no_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3){ // 화면 원통 작음 or 보통 or 큼
                                red_in_servo_left_can_see_move();

                                tmr_reset(&tilt_tmr);
                                tmr_reset(&rotate_tmr);
                            }

                            else if(ras_data[1] == 4 || ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 큼 or 매우 매우 큼 or 매우 매우 매우 큼
                                // if(tilt_tmr.read_us() < tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                    red_in_servo_left_can_see_move();
                                // }
                                // else if(tilt_tmr.read_us() > tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                //     tilt_tmr_move(); // 1.5초 이상 로봇 각도 5도 이상 : 매우 빠른 후진
                                //     where = 205;
                                // }

                                tilt_tmr_judgment(); // 로봇 각도 5도 이상 체크

                                if(abs(speedL) <= 0.60 && abs(speedR) <= 0.60){
                                    speedL = speedL * (1.666);
                                    speedR = speedR * (1.666);
                                }
                            }
                        }

                        else if(angML < ang && ang < angMR){ // 서보 중간
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                no_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3){ // 화면 원통 작음 or 보통 or 큼
                                red_in_servo_mid_can_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 4 || ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 큼 or 매우 매우 큼 or 매우 매우 매우 큼
                                // if(tilt_tmr.read_us() < tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                    red_in_servo_mid_can_see_move();
                                // }
                                // else if(tilt_tmr.read_us() > tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                //     tilt_tmr_move(); // 1.5초 이상 로봇 각도 5도 이상 : 매우 빠른 후진
                                //     where = 215;
                                // }

                                tilt_tmr_judgment(); // 로봇 각도 5도 이상 체크 !!!

                                if(abs(speedL) <= 0.60 && abs(speedR) <= 0.60){
                                    speedL = speedL * (1.666);
                                    speedR = speedR * (1.666);
                                }
                            }
                        }
                        
                        else if(angMR <= ang){ // 서보 오른쪽
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                no_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3){ // 화면 원통 작음 or 보통 or 큼
                                red_in_servo_right_can_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 4 || ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 큼 or 매우 매우 큼 or 매우 매우 매우 큼
                                // if(tilt_tmr.read_us() < tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                    red_in_servo_right_can_see_move();
                                // }
                                // else if(tilt_tmr.read_us() > tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                //     tilt_tmr_move(); // 1.5초 이상 로봇 각도 5도 이상 : 매우 빠른 후진
                                //     where = 225;
                                // }

                                tilt_tmr_judgment(); // 로봇 각도 5도 이상 체크 !!!

                                if(abs(speedL) <= 0.60 && abs(speedR) <= 0.60){
                                    speedL = speedL * (1.666);
                                    speedR = speedR * (1.666);
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

                            no_see_move();

                            tmr_reset(&tilt_tmr); 
                            tmr_reset(&rotate_tmr);
                        }
                        else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3){ // 화면 원통 작음 or 보통 or 큼
                            red_out_servo_all_can_see_move();

                            tmr_reset(&tilt_tmr); 
                            tmr_reset(&rotate_tmr);
                        }
                        else if(ras_data[1] == 4 || ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 큼 or 매우 매우 큼 or 매우 매우 매우 큼
                            if(tilt_tmr.read_us() < tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                if(ang <= angLL && psdfm_val <= 10){ // 앞 PSD 10cm 이하 + 각도 매우 왼쪽 : 매우 빠른 후진
                                    back_tmr_move<double>(&brk_tmr, &back_escape_time, &psdfm_val, "<", 20.0, -1.0, -0.6);
                                    where = 235;
                                }
                                else if(ang >= angRR && psdfm_val <= 10){ // 앞 PSD 10cm 이하 + 각도 매우 오른쪽 : 매우 빠른 후진
                                    back_tmr_move<double>(&brk_tmr, &back_escape_time, &psdfm_val, "<", 20.0, -0.6, -1.0);
                                    where = 236;
                                }
                                else{ // 그 외
                                    red_out_servo_all_can_see_move();
                                }
                            }
                            else if(tilt_tmr.read_us() > tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                tilt_tmr_move(); // 1.5초 이상 로봇 각도 5도 이상 : 매우 빠른 후진
                                where = 237;
                            }

                            tilt_tmr_judgment(); // 로봇 각도 5도 이상 체크
                            rotate_tmr_judgment(); // 로봇 빨간원 주위 회전 체크

                            if(abs(speedL) <= 0.60 && abs(speedR) <= 0.60){
                                speedL = speedL * (1.666);
                                speedR = speedR * (1.666);
                            }
                        }
                    }
                }

                // mutex.unlock();
            }

            else if(tot_mode >= 1){
                // blt.printf("%d, %.1f, %.2f, %.2f, %d, %d\n", mode, pitch_p, speedL, speedR, rotate_tmr.read_us(), tilt_tmr.read_us()); // 확인용 코드
                // blt.printf("%.2f, %.2f, %d, %.1f\n", speedL, speedR, rotate_tmr.read_us(), pitch_p); // 확인용 코드
                // blt.printf("%.2f\n", pitch_p); // 확인용 코드
                // blt.printf("b%d\n", brk_tmr.read_ms()); // 확인용 코드
                // blt.printf("r%d\n", rotate_tmr.read_ms()); // 확인용 코드

                // blt.printf("mode : %d\n", mode); // 확인용 코드
                // blt.printf("w%d\n", where); // 확인용 코드
                // if(where == 46 || where == 24 || where == 6) blt.printf("---%d---\n", where);
                // blt.printf("i%d\n", ir_val[6]);

                // blt.printf("%d\n", tilt_tmr.read_us()); // 확인용 코드
                // blt.printf("%d\n", brk_tmr.read_us()); // 확인용 코드
                // blt.printf("ir_val : | %u | %u | %u | %u | %u | %u | %u |\n", ir_val[0]/1000, ir_val[1]/1000, ir_val[2]/1000, ir_val[3]/1000, ir_val[4]/1000, ir_val[5]/1000, ir_val[6]/1000); // 확인용 코드

                // 초기 동작 : 상대 탐색
                if(mode == 0){
                    init_move();
                }

                // 중간 동작1 : 원까지 접근
                else if(mode == 1){
                    // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치 (ir 사용o. 전면 카메라 사용x)
                    if(ras_data[2] == 0){ // 상대방이 빨간색 끝좌표 안쪽에 위치
                        if(ras_data[1] == 9){ // 화면 원통 안보임
                            // pc.printf("상대 안보임 \n"); // 확인용 코드

                            no_see_move();
                        }
                        else if(ras_data[1] != 9){ // 화면 원통 보임
                            if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 전진
                                speedL = 1.0; speedR = 1.0;
                            }
                            else{
                                speedL = 0.0; speedR = 0.0;
                                mode = 2;
                            }
                        }
                    }
                    
                    // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 바깥에 위치
                    // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 2개만 출력 + 상대방이 빨간색 끝좌표 안에 위치
                    // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 빨간색 안보임
                    else if(ras_data[2] == 1){ // 상대방이 빨간색 끝좌표 바깥에 위치
                        mode = 3;
                    }
                }

                // 중간 동작2 : 원 앞에서 대기
                else if(mode == 2){
                    if(ras_data[2] == 0){ // 상대방이 빨간색 끝좌표 안쪽에 위치
                        // pc.printf("상대방이 빨간색 안에 위치,   "); // 확인용 코드

                        // // 서보 왼쪽 최대 + 화면 매우 왼쪽 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치
                        // if(ang == 0 && ras_data[0] < width_l){ // 서보 왼쪽 최대 + 화면 매우 왼쪽 상대방 보임 // 상대 로봇이 클때의 경우도 and 조건으로 추가해야함!!!
                        //     brk_tmr.start();
                        //     while(ir_val[6] < black){
                        //         speedL = -0.40; speedR = 0.40;

                        //         whl_bundle();
                        //         if(brk_tmr.read_us() > turn_escape_time){
                        //             brk_tmr.reset();
                        //             brk_tmr.stop();
                        //             break;
                        //         }
                        //         // if(ras_data[1] == 4) break;
                        //     }
                        // }

                        // // 서보 오른쪽 최대 + 화면 매우 오른쪽 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치
                        // else if(ang == 180 && width_r < ras_data[0]){ // 서보 오른쪽 최대 + 화면 매우 오른쪽 상대방 보임 // 상대 로봇이 클때의 경우도 and 조건으로 추가해야함!!!
                        //     brk_tmr.start();
                        //     while(ir_val[6] < black){
                        //         speedL = -0.40; speedR = 0.40;

                        //         whl_bundle();
                        //         if(brk_tmr.read_us() > turn_escape_time){
                        //             brk_tmr.reset();
                        //             brk_tmr.stop();
                        //             break;
                        //         }
                        //         // if(ras_data[1] == 4) break;
                        //     }
                        // }

                        // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치 (ir 사용o. 전면 카메라 사용x)
                        if(ras_data[1] == 9){ // 화면 원통 안보임
                            // pc.printf("상대 안보임 \n"); // 확인용 코드

                            no_see_move();

                            tmr_reset(&waiting_start_tmr);
                            tmr_reset(&waiting_dir_tmr);
                        }
                        else if(ras_data[1] != 9){ // 화면 원통 보임
                            if(ras_data[0] < width_l){ // 화면 왼쪽
                                if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                                    // pc.printf("상대 보임 \n"); // 확인용 코드

                                    if(
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) || // 모든 바퀴 : 모드 변경
                                        (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 모드 변경
                                        (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 모드 변경
                                    ){
                                        mode = 3;
                                    }
                                    else if(
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.80, -0.50);
                                    }
                                    else{ // 그 외 : 제자리 좌회전
                                        speedL = -0.30; speedR = 0.30;
                                    }
                                }
                                else if(ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 매우 큼 or 매우 매우 매우 큼 : 모드 변경
                                    mode = 3;
                                }

                                tmr_reset(&waiting_start_tmr);
                                tmr_reset(&waiting_dir_tmr);
                            }

                            else if(width_l <= ras_data[0] && ras_data[0] < width_r){ // 화면 중간
                                if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                                    // pc.printf("상대 보임 \n"); // 확인용 코드

                                    if(
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) || // 모든 바퀴 : 모드 변경
                                        (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 모드 변경
                                        (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 모드 변경
                                    ){
                                        mode = 3;

                                        tmr_reset(&waiting_start_tmr);
                                        tmr_reset(&waiting_dir_tmr);
                                    }
                                    else if(
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.65, -0.65);

                                        tmr_reset(&waiting_start_tmr);
                                        tmr_reset(&waiting_dir_tmr);
                                    }
                                    else{ // 그 외 : 정지
                                        waiting_start_tmr.start();
                                        if(waiting_start_tmr.read_us() > waiting_start_time){ // 5초 이상 : 조금씩 회전
                                            waiting_dir_tmr.start();
                                            if(waiting_dir_tmr.read_us() > waiting_dir_time){ // 0.2초 이상 : 회전 방향 변경
                                                if(waiting_dir == 'l') waiting_dir = 'r';
                                                else if(waiting_dir == 'r') waiting_dir = 'l';
                                                tmr_reset(&waiting_dir_tmr);
                                            }
                                            else{ // 0.2초 이하 : 조금씩 회전
                                                if(waiting_dir == 'r'){ // 회전 방향 오른쪽
                                                    speedL = 0.05; speedR = -0.05;
                                                }
                                                else if(waiting_dir == 'l'){ // 회전 방향 왼쪽
                                                    speedL = -0.05; speedR = 0.05;
                                                }
                                            }
                                        }
                                        else{ // 5초 이하 : 정지
                                            speedL = 0.0; speedR = 0.0;

                                            tmr_reset(&waiting_dir_tmr);
                                        }
                                    }
                                }
                                else if(ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 매우 큼 or 매우 매우 매우 큼 : 모드 변경
                                    mode = 3;

                                    tmr_reset(&waiting_start_tmr);
                                    tmr_reset(&waiting_dir_tmr);
                                }
                            }

                            else if(width_r <= ras_data[0]){ // 화면 오른쪽
                                if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                                    // pc.printf("상대 보임 \n"); // 확인용 코드

                                    if(
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) || // 모든 바퀴 : 모드 변경
                                        (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 모드 변경
                                        (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true) // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 모드 변경
                                    ){
                                        mode = 3;
                                    }
                                    else if(
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) || // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        back_tmr_move<bool>(&brk_tmr, &back_escape_time, &ir_WhCol[0], "==", true, -0.50, -0.80);
                                    }
                                    else{ // 그 외 : 제자리 우회전
                                        speedL = 0.30; speedR = -0.30;
                                    }
                                }
                                else if(ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 매우 큼 or 매우 매우 매우 큼 : 모드 변경
                                    mode = 3;
                                }

                                tmr_reset(&waiting_start_tmr);
                                tmr_reset(&waiting_dir_tmr);
                            }
                        }
                    }

                    // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 바깥에 위치
                    // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 2개만 출력 + 상대방이 빨간색 끝좌표 안에 위치
                    // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 빨간색 안보임
                    else if(ras_data[2] == 1){ // 상대방이 빨간색 끝좌표 바깥에 위치
                        mode = 3;

                        tmr_reset(&waiting_start_tmr);
                        tmr_reset(&waiting_dir_tmr);
                    }
                }

                // 후반 동작 : 공격 전략에서 사용하는 중간 동작과 같음
                else if(mode == 3){
                    // 서보모터 움직임
                    servo_move(Servo);
                    
                    // pc.printf("mode = 1 \n"); // 확인용 코드

                    if(ras_data[2] == 0){ // 상대방이 빨간색 끝좌표 안쪽에 위치
                        // pc.printf("상대방이 빨간색 안에 위치,   "); // 확인용 코드

                        // // 서보 왼쪽 최대 + 화면 매우 왼쪽 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치
                        // if(ang == 0 && ras_data[0] < width_l){ // 서보 왼쪽 최대 + 화면 매우 왼쪽 상대방 보임 // 상대 로봇이 클때의 경우도 and 조건으로 추가해야함!!!
                        //     brk_tmr.start();
                        //     while(ir_val[6] < black){
                        //         speedL = -0.80; speedR = 0.80;

                        //         whl_bundle();
                        //         if(brk_tmr.read_us() > turn_escape_time){
                        //             brk_tmr.reset();
                        //             brk_tmr.stop();
                        //             break;
                        //         }
                        //         // if(ras_data[1] == 4) break;
                        //     }
                        // }

                        // // 서보 오른쪽 최대 + 화면 매우 오른쪽 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치
                        // else if(ang == 180 && width_r < ras_data[0]){ // 서보 오른쪽 최대 + 화면 매우 오른쪽 상대방 보임 // 상대 로봇이 클때의 경우도 and 조건으로 추가해야함!!!
                        //     brk_tmr.start();
                        //     while(ir_val[6] < black){
                        //         speedL = -0.80; speedR = 0.80;

                        //         whl_bundle();
                        //         if(brk_tmr.read_us() > turn_escape_time){
                        //             brk_tmr.reset();
                        //             brk_tmr.stop();
                        //             break;
                        //         }
                        //         // if(ras_data[1] == 4) break;
                        //     }
                        // }

                        // 서보가 많이 돌아가있음(테스트 필요)
                        // 상대 감지됐는데 화면 매우 큼(테스트 필요)
                        // 벽에 박을 정도로 psd 앞 매우 가까이 감지. 앞판이 벽면에 완전히 박혀있을 때처럼 제자리 회전마저도 불가능한 경우 기준으로 측정(테스트 필요)
                        // 후진 1.0으로 밀어버림

                        // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치 (ir 사용o. 전면 카메라 사용x)
                        if(ang <= angML){ // 서보 왼쪽
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                no_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3){ // 화면 원통 작음 or 보통 or 큼
                                red_in_servo_left_can_see_move();

                                tmr_reset(&tilt_tmr);
                                tmr_reset(&rotate_tmr);
                            }

                            else if(ras_data[1] == 4 || ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 큼 or 매우 매우 큼 or 매우 매우 매우 큼
                                // if(tilt_tmr.read_us() < tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                    red_in_servo_left_can_see_move();
                                // }
                                // else if(tilt_tmr.read_us() > tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                //     tilt_tmr_move(); // 1.5초 이상 로봇 각도 5도 이상 : 매우 빠른 후진
                                //     where = 205;
                                // }

                                tilt_tmr_judgment(); // 로봇 각도 5도 이상 체크

                                if(abs(speedL) <= 0.60 && abs(speedR) <= 0.60){
                                    speedL = speedL * (1.666);
                                    speedR = speedR * (1.666);
                                }
                            }
                        }

                        else if(angML < ang && ang < angMR){ // 서보 중간
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                no_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3){ // 화면 원통 작음 or 보통 or 큼
                                red_in_servo_mid_can_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 4 || ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 큼 or 매우 매우 큼 or 매우 매우 매우 큼
                                // if(tilt_tmr.read_us() < tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                    red_in_servo_mid_can_see_move();
                                // }
                                // else if(tilt_tmr.read_us() > tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                //     tilt_tmr_move(); // 1.5초 이상 로봇 각도 5도 이상 : 매우 빠른 후진
                                //     where = 215;
                                // }

                                tilt_tmr_judgment(); // 로봇 각도 5도 이상 체크 !!!

                                if(abs(speedL) <= 0.60 && abs(speedR) <= 0.60){
                                    speedL = speedL * (1.666);
                                    speedR = speedR * (1.666);
                                }
                            }
                        }
                        
                        else if(angMR <= ang){ // 서보 오른쪽
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                no_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3){ // 화면 원통 작음 or 보통 or 큼
                                red_in_servo_right_can_see_move();

                                tmr_reset(&tilt_tmr); 
                                tmr_reset(&rotate_tmr);
                            }
                            else if(ras_data[1] == 4 || ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 큼 or 매우 매우 큼 or 매우 매우 매우 큼
                                // if(tilt_tmr.read_us() < tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                    red_in_servo_right_can_see_move();
                                // }
                                // else if(tilt_tmr.read_us() > tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                //     tilt_tmr_move(); // 1.5초 이상 로봇 각도 5도 이상 : 매우 빠른 후진
                                //     where = 225;
                                // }

                                tilt_tmr_judgment(); // 로봇 각도 5도 이상 체크 !!!

                                if(abs(speedL) <= 0.60 && abs(speedR) <= 0.60){
                                    speedL = speedL * (1.666);
                                    speedR = speedR * (1.666);
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

                            no_see_move();

                            tmr_reset(&tilt_tmr); 
                            tmr_reset(&rotate_tmr);
                        }
                        else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3){ // 화면 원통 작음 or 보통 or 큼
                            red_out_servo_all_can_see_move();

                            tmr_reset(&tilt_tmr); 
                            tmr_reset(&rotate_tmr);
                        }
                        else if(ras_data[1] == 4 || ras_data[1] == 5 || ras_data[1] == 6){ // 화면 원통 매우 큼 or 매우 매우 큼 or 매우 매우 매우 큼
                            if(tilt_tmr.read_us() < tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                if(ang <= angLL && psdfm_val <= 10){ // 앞 PSD 10cm 이하 + 각도 매우 왼쪽 : 매우 빠른 후진
                                    back_tmr_move<double>(&brk_tmr, &back_escape_time, &psdfm_val, "<", 20.0, -1.0, -0.6);
                                    where = 235;
                                }
                                else if(ang >= angRR && psdfm_val <= 10){ // 앞 PSD 10cm 이하 + 각도 매우 오른쪽 : 매우 빠른 후진
                                    back_tmr_move<double>(&brk_tmr, &back_escape_time, &psdfm_val, "<", 20.0, -0.6, -1.0);
                                    where = 236;
                                }
                                else{ // 그 외
                                    red_out_servo_all_can_see_move();
                                }
                            }
                            else if(tilt_tmr.read_us() > tilt_recog_time){ // 타이머 일정 시간 이상 : 특정 움직임
                                tilt_tmr_move(); // 1.5초 이상 로봇 각도 5도 이상 : 매우 빠른 후진
                                where = 237;
                            }

                            tilt_tmr_judgment(); // 로봇 각도 5도 이상 체크
                            rotate_tmr_judgment(); // 로봇 빨간원 주위 회전 체크

                            if(abs(speedL) <= 0.60 && abs(speedR) <= 0.60){
                                speedL = speedL * (1.666);
                                speedR = speedR * (1.666);
                            }
                        }
                    }
                }
                // mutex.unlock();
            }

            // all_print();

            pre_rotate_dir = rotate_dir; // while bundle에는 이거 안넣어도 되나???
            rotate_dir = 'n';
            All_move = false;

            tmr_reset(&com_check_tmr); // 통신값 여부 타이머 초기화
        }
        else if(All_move == false){ // 통신 못받음
            com_check_tmr.start(); // 통신값 여부 타이머 시작
            if(com_check_tmr.read_us() > com_check_time){ // 1.5초 이상 통신 못받음 : 제자리 회전
                speedL = 0.45; speedR = -0.45;
            }
            // 1.5초 이하 통신 못받음 : 원래 동작 유지
        }

        DC_move(speedL, speedR);

        // ///////////////////////////////////////////////////
        // if(temp_tmr.read_us() >= for_time){
        //     theta++;
        //     speedL = cos(theta * PI / 180); speedR = cos(theta * PI / 180);

        //     if(speedL >= 0.95) for_time = total_time / 2880;
        //     else if(speedL <= -0.95) for_time = total_time / 2880;
        //     else if(speedL >= -0.05 && speedL <= 0.05) for_time = total_time / 90;

        //     // for_time = total_time / 90;
        //     // if(speedL >= 0.95) theta = 90;
        //     // else if(speedL <= -0.95) theta = 270;

        //     // pc.printf("%d, %lf, %lf \n", theta, speedL, speedR);
        //     // pc.printf("%lf, %lf, %lf \n", speedL, speedR, for_time);

        //     temp_tmr.reset();
        // }
        // ///////////////////////////////////////////////////

        // // 빠르게 뒤로 or 앞으로 전환
        // // 목표보다 작으면 : ++
        // // 목표보다 크면 : --
        // // 목표와 같으면 : 그대로
    }
}