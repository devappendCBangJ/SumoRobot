// [라이브러리]
#include "Preprocessing.h"

// ir + psd 센서
void sensor_read(){
    ir_val[0] = irfl.read_u16();
    ir_val[1] = irfr.read_u16();
    ir_val[2] = irmr.read_u16();
    ir_val[3] = irbr.read_u16();
    ir_val[4] = irbl.read_u16();
    ir_val[5] = irml.read_u16();
    ir_val[6] = irfm.read_u16();

    psdf_volts = psdf.read() * 3.3;
    psdf_val = 13 * pow(psdf_volts, -1.0);
    if(psdf_val > 30) psdf_val = 30;

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
    pc.printf("psdf_val : | %lf |, psdb_val : | %lf |\n", psdf_val, psdb_val); // 확인용 코드
}

// AC 서보 모터
template <class T> T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void servo_set(PwmOut &rc){
    uint16_t pulseW = map<float>(ang, 180., 0., 500., 2600.);
    // uint16_t pulseW = map<float>(ang, 180., 0., 833., 2266.);
    rc.pulsewidth_us(pulseW);
}

void servo_move(PwmOut &rc){
    if(0 <= ras_data[0] && ras_data[0] < width/2) inc = map<float>(ras_data[0], width/2, 1.0, small_inc, big_inc);
    else if(width/2 <= ras_data[0] && ras_data[0] <= width) inc = map<float>(ras_data[0], width/2, width, small_inc, big_inc);

    // 중간 동작 : 화면 상대방 안보임
    if(ras_data[0] == 999){
        // pc.printf("1"); // 확인용 코드
        inc = big_inc;

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
    // uint16_t pulseW = map<float>(ang, 180., 0., 833., 2266.);
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

// 타이머 움직임
void normal_tmr_move(bool* _while_brk_sensor, const char* _inequality, bool _sensor_val, double _speedL, double _speedR){
    brk_tmr.start();
    if(_inequality[0] == '='){
        while(*_while_brk_sensor == _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(brk_tmr.read_us() > back_escape_time){
                brk_tmr.reset();
                brk_tmr.stop();
                break;
            }
        }
    }
    else if(_inequality[0] == '>'){
        while(*_while_brk_sensor > _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(brk_tmr.read_us() > back_escape_time){
                brk_tmr.reset();
                brk_tmr.stop();
                break;
            }
        }
    }
    else if(_inequality[0] == '<'){
        while(*_while_brk_sensor < _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(brk_tmr.read_us() > back_escape_time){
                brk_tmr.reset();
                brk_tmr.stop();
                break;
            }
        }
    }
}

void normal_tmr_move(uint16_t * _while_brk_sensor, const char* _inequality, uint16_t _sensor_val, double _speedL, double _speedR){
    brk_tmr.start();
    if(_inequality[0] == '='){
        while(*_while_brk_sensor == _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(brk_tmr.read_us() > back_escape_time){
                brk_tmr.reset();
                brk_tmr.stop();
                break;
            }
        }
    }
    else if(_inequality[0] == '>'){
        while(*_while_brk_sensor > _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(brk_tmr.read_us() > back_escape_time){
                brk_tmr.reset();
                brk_tmr.stop();
                break;
            }
        }
    }
    else if(_inequality[0] == '<'){
        while(*_while_brk_sensor < _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(brk_tmr.read_us() > back_escape_time){
                brk_tmr.reset();
                brk_tmr.stop();
                break;
            }
        }
    }
}

void normal_tmr_move(double* _while_brk_sensor, const char* _inequality, double _sensor_val, double _speedL, double _speedR){
    brk_tmr.start();
    if(_inequality[0] == '='){
        while(*_while_brk_sensor == _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(brk_tmr.read_us() > back_escape_time){
                brk_tmr.reset();
                brk_tmr.stop();
                break;
            }
        }
    }
    else if(_inequality[0] == '>'){
        while(*_while_brk_sensor > _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(brk_tmr.read_us() > back_escape_time){
                brk_tmr.reset();
                brk_tmr.stop();
                break;
            }
        }
    }
    else if(_inequality[0] == '<'){
        while(*_while_brk_sensor < _sensor_val){
            speedL = _speedL; speedR = _speedR;

            whl_bundle();
            if(brk_tmr.read_us() > back_escape_time){
                brk_tmr.reset();
                brk_tmr.stop();
                break;
            }
        }
    }
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
            ras_serialInBuffer[ras_buff_cnt] = '\0';
            ras_data[ras_data_cnt++] = atof(ras_serialInBuffer);
            ras_buff_cnt = 0;
        }
        // '/' : 시리얼 완료
        else if(byteIn=='/'){
            // pc.printf("byteIn == '/' : %c \n", byteIn); // 확인용 코드
            ras_serialInBuffer[ras_buff_cnt] = '\0';
            ras_data[ras_data_cnt]=atof(ras_serialInBuffer);
            ras_buff_cnt=0; ras_data_cnt = 0;
            gotPacket = true;
        }
        // 이외 : 시리얼 저장
        else{
            // pc.printf("byteIn == '나머지' : %c\n", byteIn); // 확인용 코드
            ras_serialInBuffer[ras_buff_cnt++] = byteIn;
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

void btn_flip(){
    tot_mode++;
}

void led_flash(){
    tic_cnt++;
    if(tic_cnt % 2 == 0){
        tic_even_cnt++;
        if(tot_mode > (tic_even_cnt % 10)){
            led1 = 1;
        }
    }
    else led1 = 0;
}

void whl_bundle(){
    in_SerialRx_main(); // interrupt 전용

    sensor_read();
    sensor_cal();
    // sensor_print(); // 확인용 코드

    servo_move(Servo);
    DC_move(speedL, speedR);

    all_print();
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