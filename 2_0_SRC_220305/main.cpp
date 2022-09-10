// Servo 라이브러리 : https://os.mbed.com/users/simon/code/Servo/docs/tip/classServo.html

#include "Preprocessing.h"  // 헤더파일 전처리

// [main문]
int main(){
    pc.format(8, SerialBase::Even, 1);

    Servo.period_ms(10);
    DC_set();
    servo_set(Servo);
    ras.attach(&in_SerialRx); // interrupt 전용

    btn.fall(&btn_flip);
    mode_tic.attach(&led_flash, 0.10);

    // com_th.start(&th_SerialRx); // thread 전용
    while(1){
        // all_print();

        in_SerialRx_main(); // interrupt 전용

        sensor_read();
        sensor_cal();
        // sensor_print(); // 확인용 코드

        if(All_move == true){ // 통신 받음
            // servo_chk(Servo); // Test 코드
            // DC_chk(); // Test 코드

            // mutex.lock();

            // pc.printf("tot_mode : %d \n", tot_mode); // 확인용 코드

            if(tot_mode == 0){
                // 초기 동작 : 상대 탐색
                if(mode == 0){
                    if(ras_data[0] == 999){ // 상대 안보임
                        speedL = 0.45; speedR = -0.45;
                    }
                    else if(ras_data[0] < width_l){ // 화면 왼쪽 보임
                        speedL = -0.45; speedR = 0.45;
                    }
                    else if(width_l <= ras_data[0] && ras_data[0] < width_r){ // 화면 가운데 보임
                        speedL = 0.0; speedR = 0.0;
                        mode = 1;
                        // pc.printf("mode = 1"); // 확인용 코드
                    }
                    else if(width_r <= ras_data[0]){ // 화면 오른쪽 보임
                        speedL = 0.45; speedR = -0.45;
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

                                if(pre_data0 == 1){
                                    speedL = -0.45; speedR = 0.45;
                                }
                                else if(pre_data0 == 2){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else if(pre_data0 == 3){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else{
                                    speedL = 0.45; speedR = -0.45;
                                }
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                                // pc.printf("상대 보임 \n"); // 확인용 코드

                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        if(angLL < ang){ // 서보 보통 왼쪽
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                                        }
                                        else if(ang <= angLL){ // 서보 매우 왼쪽
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);
                                        }
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        if(angLL < ang){ // 서보 보통 왼쪽
                                            speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                            speedR = 0.60;
                                        }
                                        else if(ang <= angLL){ // 서보 매우 왼쪽
                                            speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                            speedR = 0.50;
                                        }
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                }
                                else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_val[6], "<", black, -0.45, 0.45);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 오른쪽 전진
                                        speedL = 0.60; speedR = 0.10;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 왼쪽 전진
                                        speedL = 0.30; speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 오른쪽 전진
                                        speedL = 0.60; speedR = 0.30;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 매우 왼쪽 전진
                                    speedL = 0.10; speedR = 0.60; // 0.225;
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                    if(ang <= angLL){
                                        speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                        speedR = 0.50;
                                    }
                                    else if(ang > angLL){
                                        speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                        speedR = 0.60;
                                    }
                                }
                                else{ // 그 외 : 왼쪽 전진
                                    speedL = 0.27; speedR = 0.60;
                                }
                            }
                            if(ras_data[1] == 4){ // 화면 매우 큼
                                if(abs(speedL) <= 0.66 && abs(speedR) <= 0.66){
                                    speedL = speedL * (1.50);
                                    speedR = speedR * (1.50);
                                }
                            }
                        }

                        else if(angML < ang && ang < angMR){ // 서보 중간
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                if(pre_data0 == 1){
                                    speedL = -0.45; speedR = 0.45;
                                }
                                else if(pre_data0 == 2){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else if(pre_data0 == 3){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else{
                                    speedL = 0.45; speedR = -0.45;
                                }
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                                // pc.printf("상대 보임 \n"); // 확인용 코드

                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.70, -0.70);
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 빠른 자유롭게 공격
                                        ///////////////////////////////////////////////////////
                                        // speedL = 0.50; speedR = 0.50;
                                        ///////////////////////////////////////////////////////
                                        speedL = 0.60; speedR = 0.60;
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                }
                                else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞
                                    if(ang >= 90){ // 서보 조금이라도 오른쪽 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_val[6], "<", black, 0.45, -0.45);
                                    }
                                    else if(ang < 90){ // 서보 조금이라도 왼쪽 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_val[6], "<", black, -0.45, 0.45);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 오른쪽 전진
                                        speedL = 0.60; speedR = 0.10;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                    }
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 왼쪽 전진
                                        speedL = 0.10; speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                    }
                                }
                                else if(
                                    (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
                                    (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
                                ){
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 전진
                                        speedL = 0.35; speedR = 0.35;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 왼쪽 전진
                                    speedL = 0.27; speedR = 0.60;
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 오른쪽 전진
                                    speedL = 0.60; speedR = 0.27;
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                    ///////////////////////////////////////////////////////
                                    // speedL = 0.50; speedR = 0.50;
                                    ///////////////////////////////////////////////////////
                                    speedL = 0.60; speedR = 0.60;
                                }
                                else{ // 그 외 : 전진
                                    speedL = 0.60; speedR = 0.60;
                                }
                            }
                            if(ras_data[1] == 4){ // 화면 원통 매우 큼
                                if(abs(speedL) <= 0.66 && abs(speedR) <= 0.66){
                                    speedL = speedL * (1.50);
                                    speedR = speedR * (1.50);
                                }
                            }
                        }
                        
                        else if(angMR <= ang){ // 서보 오른쪽
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                if(pre_data0 == 1){
                                    speedL = -0.45; speedR = 0.45;
                                }
                                else if(pre_data0 == 2){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else if(pre_data0 == 3){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else{
                                    speedL = 0.45; speedR = -0.45;
                                }
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                                // pc.printf("상대 보임 \n"); // 확인용 코드

                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        if(ang < angRR){ // 서보 보통 오른쪽
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                                        }
                                        else if(angRR <= ang){ // 서보 매우 오른쪽
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                                        }
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        if(angRR <= ang){
                                            speedL = 0.50;
                                            speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.18);
                                        }
                                        else if(ang < angRR){
                                            speedL = 0.60;
                                            speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                        }
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 왼쪽 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                }
                                else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_val[6], "<", black, 0.45, -0.45);
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 왼쪽 전진
                                        speedL = 0.10; speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 왼쪽 전진
                                        speedL = 0.30; speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                    }
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 오른쪽 전진
                                        speedL = 0.60; speedR = 0.30;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 매우 오른쪽 전진
                                    speedL = 0.60; speedR = 0.10; // 0.225;
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                    if(angRR <= ang){
                                        speedL = 0.60;
                                        speedR = -map<float>(ang, 180.0, angRR, 0.60, 0.18);
                                    }
                                    else if(ang < angRR){
                                        speedL = 0.60;
                                        speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                    }
                                }
                                else{ // 그 외 : 오른쪽 전진
                                    speedL = 0.60; speedR = 0.27;
                                }
                            }
                            if(ras_data[1] == 4){ // 화면 원통 매우 큼
                                if(abs(speedL) <= 0.66 && abs(speedR) <= 0.66){
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
                                speedL = -0.45; speedR = 0.45;
                            }
                            else if(pre_data0 == 2){
                                speedL = 0.45; speedR = -0.45;
                            }
                            else if(pre_data0 == 3){
                                speedL = 0.45; speedR = -0.45;
                            }
                            else{
                                speedL = 0.45; speedR = -0.45;
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
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -map<float>(ang, angML, angLL, 0.60, 0.85);
                                                speedR = -0.50;

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                            speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                            speedR = 0.60;
                                        }
                                    }
                                    else if(
                                        (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -map<float>(ang, angML, angLL, 0.60, 0.85);
                                            speedR = -0.50;

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                                        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                            speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                            speedR = 0.60;
                                        }
                                        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);


                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -map<float>(ang, angML, angLL, 0.60, 0.85);
                                                speedR = -0.50;

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                    }
                                    else{ // 그 외 : 자유롭게 공격
                                        speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                        speedR = 0.60;
                                    }
                                }
                                else if(ang <= angLL){ // 서보 매우 왼쪽
                                    if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                        if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -map<float>(ang, angLL, 0.0, 0.85, 0.95);
                                                speedR = -0.50;

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                            speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                            speedR = 0.50;
                                        }
                                    } 
                                    else if(
                                        (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -map<float>(ang, angLL, 0.0, 0.85, 0.95);
                                            speedR = -0.50;

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                                        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                            speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                            speedR = 0.50;
                                        }
                                        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -map<float>(ang, angLL, 0.0, 0.85, 0.95);
                                                speedR = -0.50;

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                    }
                                    else{ // 그 외 : 자유롭게 공격
                                        speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                        speedR = 0.50;
                                    }
                                }
                            }
                            else if(angML < ang && ang < angMR){ // 서보 중간
                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -0.50);

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -0.50;
                                            speedR = -0.50;

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        speedL = 0.60;
                                        speedR = 0.60;
                                    }
                                }
                                else if(
                                    (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                ){
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -0.50);

                                    fight_back_tmr.start();
                                    while(fight_back_tmr.read_us() < fight_back_time){
                                        speedL = -0.50;
                                        speedR = -0.50;

                                        whl_bundle();
                                    }
                                    fight_back_tmr.reset();
                                    fight_back_tmr.stop();
                                }
                                else if(
                                    (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴
                                    (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) // 오른쪽 앞 바퀴
                                ){
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                        speedL = 0.60;
                                        speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -0.50);

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -0.50;
                                            speedR = -0.50;

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                }
                                else{ // 그 외 : 자유롭게 공격
                                    speedL = 0.60;
                                    speedR = 0.60;
                                }
                            }
                            else if(angMR <= ang){ // 서보 오른쪽
                                // speed = map<float>(ang, 180.0, 105.0, 0.35, 0.20);
                                // DC_move(1, 0, speed, speed);

                                if(ang < angRR){ // 서보 보통 오른쪽
                                    if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                        if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -0.50;
                                                speedR = -map<float>(ang, angMR, angRR, 0.60, 0.85);

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                            speedL = 0.60;
                                            speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                        }
                                    }
                                    else if(
                                        (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -0.50;
                                            speedR = -map<float>(ang, angMR, angRR, 0.60, 0.85);

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                                        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                            speedL = 0.60;
                                            speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                        }
                                        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -0.50;
                                                speedR = -map<float>(ang, angMR, angRR, 0.60, 0.85);

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                    }
                                    else{ // 그 외 : 자유롭게 공격
                                        speedL = 0.60;
                                        speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                    }
                                }
                                else if(angRR <= ang){ // 서보 매우 오른쪽
                                    if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                        if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -0.50;
                                                speedR = -map<float>(ang, angRR, 180.0, 0.85, 0.95);

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                            speedL = 0.50;
                                            speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                        }
                                    }
                                    else if(
                                        (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -0.50;
                                            speedR = -map<float>(ang, angRR, 180.0, 0.85, 0.95);

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                                        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                            speedL = 0.50;
                                            speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                        }
                                        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -0.50;
                                                speedR = -map<float>(ang, angRR, 180.0, 0.85, 0.95);

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                    }
                                    else{ // 그 외 : 자유롭게 공격
                                        speedL = 0.50;
                                        speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                    }
                                }
                            }

                            if(ang <= angLL && psdf_val <= 10){ // 앞 PSD 10cm 이하 + 각도 매우 왼쪽 : 매우 빠른 후진
                                normal_tmr_move(&psdf_val, "<", 20, -1.0, -0.6);
                            }
                            else if(ang >= angRR && psdf_val <= 10){ // 앞 PSD 10cm 이하 + 각도 매우 오른쪽 : 매우 빠른 후진
                                normal_tmr_move(&psdf_val, "<", 20, -0.6, -1.0);
                            }
                        }
                        if(ras_data[1] == 4){ // 화면 원통 매우 큼
                            if(abs(speedL) <= 0.66 && abs(speedR) <= 0.66){
                                speedL = speedL * (1.50);
                                speedR = speedR * (1.50);
                            }
                        }
                    }
                }
                // mutex.unlock();
            }

            else if(tot_mode >= 1){
                // 초기 동작 : 상대 탐색
                if(mode == 0){
                    if(ras_data[0] == 999){ // 상대 안보임
                        speedL = 0.45; speedR = -0.45;
                    }
                    else if(ras_data[0] < width_l){ // 화면 왼쪽 보임
                        speedL = -0.45; speedR = 0.45;
                    }
                    else if(width_l <= ras_data[0] && ras_data[0] < width_r){ // 화면 가운데 보임
                        speedL = 0.0; speedR = 0.0;
                        mode = 1;
                        // pc.printf("mode = 1"); // 확인용 코드
                    }
                    else if(width_r <= ras_data[0]){ // 화면 오른쪽 보임
                        speedL = 0.45; speedR = -0.45;
                    }
                }

                // 중간 동작1 : 원까지 접근
                else if(mode == 1){
                    // 화면 매우 왼쪽이나 매우 오른쪽 제외 상대방 보임 + 화면 빨간색 1개만 출력 + 상대방이 빨간색 끝좌표 안쪽에 위치 (ir 사용o. 전면 카메라 사용x)
                    if(ras_data[2] == 0){ // 상대방이 빨간색 끝좌표 안쪽에 위치
                        if(ras_data[1] == 9){ // 화면 원통 안보임
                            // pc.printf("상대 안보임 \n"); // 확인용 코드

                            if(pre_data0 == 1){
                                speedL = -0.45; speedR = 0.45;
                            }
                            else if(pre_data0 == 2){
                                speedL = 0.45; speedR = -0.45;
                            }
                            else if(pre_data0 == 3){
                                speedL = 0.45; speedR = -0.45;
                            }
                            else{
                                speedL = 0.45; speedR = -0.45;
                            }
                        }
                        else if(ras_data[1] != 9){ // 화면 원통 보임
                            if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 전진
                                speedL = 0.80; speedR = 0.80;
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

                            if(pre_data0 == 1){
                                speedL = -0.45; speedR = 0.45;
                            }
                            else if(pre_data0 == 2){
                                speedL = 0.45; speedR = -0.45;
                            }
                            else if(pre_data0 == 3){
                                speedL = 0.45; speedR = -0.45;
                            }
                            else{
                                speedL = 0.45; speedR = -0.45;
                            }
                        }
                        else if(ras_data[1] != 9){ // 화면 원통 보임
                            if(0 <= ras_data[0] < width_l){ // 화면 왼쪽
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
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.80, -0.50);
                                    }
                                    else{ // 그 외 : 제자리 좌회전
                                        speedL = -0.30; speedR = 0.30;
                                    }
                                }
                                if(ras_data[1] == 4){ // 화면 매우 큼 : 모드 변경
                                    mode = 3;
                                }
                            }

                            else if(width_l <= ras_data[0] < width_r){ // 화면 중간
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
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.65, -0.65);
                                    }
                                    else{ // 그 외 : 제자리 우회전
                                        speedL = 0.30; speedR = -0.30;
                                    }
                                }
                                if(ras_data[1] == 4){ // 화면 매우 큼 : 모드 변경
                                    mode = 3;
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
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 왼쪽 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 왼쪽 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 왼쪽 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -0.80);
                                    }
                                    else{ // 그 외 : 정지
                                        speedL = 0.00; speedR = 0.00;
                                    }
                                }
                                if(ras_data[1] == 4){ // 화면 매우 큼 : 모드 변경
                                    mode = 3;
                                }
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

                                if(pre_data0 == 1){
                                    speedL = -0.45; speedR = 0.45;
                                }
                                else if(pre_data0 == 2){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else if(pre_data0 == 3){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else{
                                    speedL = 0.45; speedR = -0.45;
                                }
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                                // pc.printf("상대 보임 \n"); // 확인용 코드

                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        if(angLL < ang){ // 서보 보통 왼쪽
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);
                                        }
                                        else if(ang <= angLL){ // 서보 매우 왼쪽
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);
                                        }
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        if(angLL < ang){ // 서보 보통 왼쪽
                                            speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                            speedR = 0.60;
                                        }
                                        else if(ang <= angLL){ // 서보 매우 왼쪽
                                            speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                            speedR = 0.50;
                                        }
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                }
                                else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_val[6], "<", black, -0.45, 0.45);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 오른쪽 전진
                                        speedL = 0.60; speedR = 0.10;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 왼쪽 전진
                                        speedL = 0.30; speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 오른쪽 전진
                                        speedL = 0.60; speedR = 0.30;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.45, -0.30);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 매우 왼쪽 전진
                                    speedL = 0.10; speedR = 0.60; // 0.225;
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                    if(ang <= angLL){
                                        speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                        speedR = 0.50;
                                    }
                                    else if(ang > angLL){
                                        speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                        speedR = 0.60;
                                    }
                                }
                                else{ // 그 외 : 왼쪽 전진
                                    speedL = 0.27; speedR = 0.60;
                                }
                            }
                            if(ras_data[1] == 4){ // 화면 매우 큼
                                if(abs(speedL) <= 0.66 && abs(speedR) <= 0.66){
                                    speedL = speedL * (1.50);
                                    speedR = speedR * (1.50);
                                }
                            }
                        }

                        else if(angML < ang && ang < angMR){ // 서보 중간
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                if(pre_data0 == 1){
                                    speedL = -0.45; speedR = 0.45;
                                }
                                else if(pre_data0 == 2){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else if(pre_data0 == 3){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else{
                                    speedL = 0.45; speedR = -0.45;
                                }
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                                // pc.printf("상대 보임 \n"); // 확인용 코드

                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.70, -0.70);
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 빠른 자유롭게 공격
                                        ///////////////////////////////////////////////////////
                                        // speedL = 0.50; speedR = 0.50;
                                        ///////////////////////////////////////////////////////
                                        speedL = 0.60; speedR = 0.60;
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                }
                                else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞
                                    if(ang >= 90){ // 서보 조금이라도 오른쪽 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_val[6], "<", black, 0.45, -0.45);
                                    }
                                    else if(ang < 90){ // 서보 조금이라도 왼쪽 : 제자리 좌회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_val[6], "<", black, -0.45, 0.45);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 오른쪽 전진
                                        speedL = 0.60; speedR = 0.10;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                    }
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 왼쪽 전진
                                        speedL = 0.10; speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                    }
                                }
                                else if(
                                    (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
                                    (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
                                ){
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 전진
                                        speedL = 0.35; speedR = 0.35;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.35, -0.35);
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 왼쪽 전진
                                    speedL = 0.27; speedR = 0.60;
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 뒷 바퀴 : 오른쪽 전진
                                    speedL = 0.60; speedR = 0.27;
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                    ///////////////////////////////////////////////////////
                                    // speedL = 0.50; speedR = 0.50;
                                    ///////////////////////////////////////////////////////
                                    speedL = 0.60; speedR = 0.60;
                                }
                                else{ // 그 외 : 전진
                                    speedL = 0.60; speedR = 0.60;
                                }
                            }
                            if(ras_data[1] == 4){ // 화면 원통 매우 큼
                                if(abs(speedL) <= 0.66 && abs(speedR) <= 0.66){
                                    speedL = speedL * (1.50);
                                    speedR = speedR * (1.50);
                                }
                            }
                        }
                        
                        else if(angMR <= ang){ // 서보 오른쪽
                            if(ras_data[1] == 9){ // 화면 원통 안보임
                                // pc.printf("상대 안보임 \n"); // 확인용 코드

                                if(pre_data0 == 1){
                                    speedL = -0.45; speedR = 0.45;
                                }
                                else if(pre_data0 == 2){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else if(pre_data0 == 3){
                                    speedL = 0.45; speedR = -0.45;
                                }
                                else{
                                    speedL = 0.45; speedR = -0.45;
                                }
                            }
                            else if(ras_data[1] == 1 || ras_data[1] == 2 || ras_data[1] == 3 || ras_data[1] == 4){ // 화면 원통 작음 or 보통 or 큼 or 매우 큼
                                // pc.printf("상대 보임 \n"); // 확인용 코드

                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 빠른 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        if(ang < angRR){ // 서보 보통 오른쪽
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));
                                        }
                                        else if(angRR <= ang){ // 서보 매우 오른쪽
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));
                                        }
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        if(angRR <= ang){
                                            speedL = 0.50;
                                            speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.18);
                                        }
                                        else if(ang < angRR){
                                            speedL = 0.60;
                                            speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                        }
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 왼쪽 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                }
                                else if(ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 : 제자리 우회전 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_val[6], "<", black, 0.45, -0.45);
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 매우 왼쪽 전진
                                        speedL = 0.10; speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 왼쪽 전진
                                        speedL = 0.30; speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                    }
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 조금 오른쪽 전진
                                        speedL = 0.60; speedR = 0.30;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                    }
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true){ // 왼쪽 앞 바퀴 + 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                }
                                else if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.30, -0.45);
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 왼쪽 뒷 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 매우 오른쪽 전진
                                    speedL = 0.60; speedR = 0.10; // 0.225;
                                }
                                else if(ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 모두 검은색 : 자유롭게 공격
                                    if(angRR <= ang){
                                        speedL = 0.60;
                                        speedR = -map<float>(ang, 180.0, angRR, 0.60, 0.18);
                                    }
                                    else if(ang < angRR){
                                        speedL = 0.60;
                                        speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                    }
                                }
                                else{ // 그 외 : 오른쪽 전진
                                    speedL = 0.60; speedR = 0.27;
                                }
                            }
                            if(ras_data[1] == 4){ // 화면 원통 매우 큼
                                if(abs(speedL) <= 0.66 && abs(speedR) <= 0.66){
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
                                speedL = -0.45; speedR = 0.45;
                            }
                            else if(pre_data0 == 2){
                                speedL = 0.45; speedR = -0.45;
                            }
                            else if(pre_data0 == 3){
                                speedL = 0.45; speedR = -0.45;
                            }
                            else{
                                speedL = 0.45; speedR = -0.45;
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
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -map<float>(ang, angML, angLL, 0.60, 0.85);
                                                speedR = -0.50;

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                            speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                            speedR = 0.60;
                                        }
                                    }
                                    else if(
                                        (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -map<float>(ang, angML, angLL, 0.60, 0.85);
                                            speedR = -0.50;

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                                        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                            speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                            speedR = 0.60;
                                        }
                                        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angML, angLL, 0.60, 0.85), -0.50);


                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -map<float>(ang, angML, angLL, 0.60, 0.85);
                                                speedR = -0.50;

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                    }
                                    else{ // 그 외 : 자유롭게 공격
                                        speedL = map<float>(ang, angML, angLL, 0.30, 0.18);
                                        speedR = 0.60;
                                    }
                                }
                                else if(ang <= angLL){ // 서보 매우 왼쪽
                                    if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                        if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -map<float>(ang, angLL, 0.0, 0.85, 0.95);
                                                speedR = -0.50;

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                            speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                            speedR = 0.50;
                                        }
                                    } 
                                    else if(
                                        (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 우회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -map<float>(ang, angLL, 0.0, 0.85, 0.95);
                                            speedR = -0.50;

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 왼쪽 앞 바퀴
                                        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                            speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                            speedR = 0.50;
                                        }
                                        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 우회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -map<float>(ang, angLL, 0.0, 0.85, 0.95), -0.50);

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -map<float>(ang, angLL, 0.0, 0.85, 0.95);
                                                speedR = -0.50;

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                    }
                                    else{ // 그 외 : 자유롭게 공격
                                        speedL = -map<float>(ang, angLL, 0.0, 0.15, 0.50);
                                        speedR = 0.50;
                                    }
                                }
                            }
                            else if(angML < ang && ang < angMR){ // 서보 중간
                                if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                    if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -0.50);

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -0.50;
                                            speedR = -0.50;

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                        speedL = 0.60;
                                        speedR = 0.60;
                                    }
                                }
                                else if(
                                    (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴) 
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                ){
                                    normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -0.50);

                                    fight_back_tmr.start();
                                    while(fight_back_tmr.read_us() < fight_back_time){
                                        speedL = -0.50;
                                        speedR = -0.50;

                                        whl_bundle();
                                    }
                                    fight_back_tmr.reset();
                                    fight_back_tmr.stop();
                                }
                                else if(
                                    (ir_WhCol[2] == true && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴
                                    (ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) // 오른쪽 앞 바퀴
                                ){
                                    if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                        speedL = 0.60;
                                        speedR = 0.60;
                                    }
                                    else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -0.50);

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -0.50;
                                            speedR = -0.50;

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                }
                                else{ // 그 외 : 자유롭게 공격
                                    speedL = 0.60;
                                    speedR = 0.60;
                                }
                            }
                            else if(angMR <= ang){ // 서보 오른쪽
                                // speed = map<float>(ang, 180.0, 105.0, 0.35, 0.20);
                                // DC_move(1, 0, speed, speed);

                                if(ang < angRR){ // 서보 보통 오른쪽
                                    if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                        if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -0.50;
                                                speedR = -map<float>(ang, angMR, angRR, 0.60, 0.85);

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                            speedL = 0.60;
                                            speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                        }
                                    }
                                    else if(
                                        (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -0.50;
                                            speedR = -map<float>(ang, angMR, angRR, 0.60, 0.85);

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                                        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                            speedL = 0.60;
                                            speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                        }
                                        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angMR, angRR, 0.60, 0.85));

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -0.50;
                                                speedR = -map<float>(ang, angMR, angRR, 0.60, 0.85);

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                    }
                                    else{ // 그 외 : 자유롭게 공격
                                        speedL = 0.60;
                                        speedR = map<float>(ang, angRR, angMR, 0.18, 0.30);
                                    }
                                }
                                else if(angRR <= ang){ // 서보 매우 오른쪽
                                    if(ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == true){ // 모든 바퀴
                                        if(psdb_val >= 70.0){ // 뒤 PSD 70cm 이상 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -0.50;
                                                speedR = -map<float>(ang, angRR, 180.0, 0.85, 0.95);

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                        else if(psdb_val < 70.0){ // 뒤 PSD 70cm 이하 : 자유롭게 공격
                                            speedL = 0.50;
                                            speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                        }
                                    }
                                    else if(
                                        (ir_WhCol[0] == true && ir_WhCol[2] == false && ir_WhCol[3] == false && ir_WhCol[4] == false && ir_WhCol[5] == false) || // ir 왼쪽 앞 + ir 오른쪽 앞 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false) || // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == true) || // 왼쪽 뒷 바퀴 + 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                        (ir_WhCol[2] == true && ir_WhCol[3] == true && ir_WhCol[4] == true && ir_WhCol[5] == false) // 왼쪽 앞 바퀴 + 오른쪽 앞 바퀴 + 오른쪽 뒷 바퀴 : 좌회 후진 (ir 왼쪽 앞 바퀴, 오른쪽 앞 바퀴 검은색 될때까지, 시간 지나면 자동으로 빠져나옴)
                                    ){
                                        normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));

                                        fight_back_tmr.start();
                                        while(fight_back_tmr.read_us() < fight_back_time){
                                            speedL = -0.50;
                                            speedR = -map<float>(ang, angRR, 180.0, 0.85, 0.95);

                                            whl_bundle();
                                        }
                                        fight_back_tmr.reset();
                                        fight_back_tmr.stop();
                                    }
                                    else if(ir_WhCol[2] == false && ir_WhCol[3] == true && ir_WhCol[4] == false && ir_WhCol[5] == false){ // 오른쪽 앞 바퀴
                                        if(ir_WhCol[0] == false){ // ir 왼쪽 앞 + ir 오른쪽 앞 X : 자유롭게 공격
                                            speedL = 0.50;
                                            speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                        }
                                        else if(ir_WhCol[0] == true){ // ir 왼쪽 앞 + ir 오른쪽 앞 O : 좌회 후진 (ir 가운데 앞 바퀴가 검은색일 때까지, 시간 지나면 자동으로 빠져나옴)
                                            normal_tmr_move(&ir_WhCol[0], "==", true, -0.50, -map<float>(ang, angRR, 180.0, 0.85, 0.95));

                                            fight_back_tmr.start();
                                            while(fight_back_tmr.read_us() < fight_back_time){
                                                speedL = -0.50;
                                                speedR = -map<float>(ang, angRR, 180.0, 0.85, 0.95);

                                                whl_bundle();
                                            }
                                            fight_back_tmr.reset();
                                            fight_back_tmr.stop();
                                        }
                                    }
                                    else{ // 그 외 : 자유롭게 공격
                                        speedL = 0.50;
                                        speedR = -map<float>(ang, 180.0, angRR, 0.50, 0.15);
                                    }
                                }
                            }

                            if(ang <= angLL && psdf_val <= 10){ // 앞 PSD 10cm 이하 + 각도 매우 왼쪽 : 매우 빠른 후진
                                normal_tmr_move(&psdf_val, "<", 20, -1.0, -0.6);
                            }
                            else if(ang >= angRR && psdf_val <= 10){ // 앞 PSD 10cm 이하 + 각도 매우 오른쪽 : 매우 빠른 후진
                                normal_tmr_move(&psdf_val, "<", 20, -0.6, -1.0);
                            }
                        }
                        if(ras_data[1] == 4){ // 화면 원통 매우 큼
                            if(abs(speedL) <= 0.66 && abs(speedR) <= 0.66){
                                speedL = speedL * (1.50);
                                speedR = speedR * (1.50);
                            }
                        }
                    }
                }
            }

            DC_move(speedL, speedR);

            // all_print();

            All_move = false;
        }
        // speedL = 0.18; speedR = 0.40;
        // DC_move(speedL, speedR);
    }
}