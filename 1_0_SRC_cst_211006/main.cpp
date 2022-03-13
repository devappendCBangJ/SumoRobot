//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//◆선언 & 초기값

#include "mbed.h"
#include "rtos.h"
#include "GP2A.h"

//[통신 + 타이머 + 모터 + 센서 핀 선언 & 초기 값]
//uart 통신
//★★★★
Thread com_thread;
char c;
RawSerial com(USBTX, USBRX, 115200); //USB포트 대신 PA_2, PA_3 사용 가능
// Serial pc_mcu(PC_12, PD_2, 115200);  //값 읽기

//타이머
//★★★★
Timer timer1;
Timer timer2;

//모터
Thread servo_thread;
DigitalOut dirL(PB_6);
PwmOut pwmL(PC_7);
DigitalOut dirR(PC_9);
PwmOut pwmR(PB_8);
PwmOut servo(PA_9);

double lspd, rspd;
double ang = 90.0;
double min_ang = 0;
double min2_ang = 30;
double mid_ang = 90.0;
double max2_ang = 150;
double max_ang = 180.0;
double ang_range = 10.0;

//ir센서
AnalogIn irfl(PA_0);
AnalogIn irfr(PA_1);
AnalogIn irmr(PC_5);
AnalogIn irbr(PB_0);
AnalogIn irbl(PC_1);
AnalogIn irml(PC_0);

//★★★★uint16_t으로 한 이유 : uint16_t - 16비트 절대값 자료형, int32_t - 32비트 음수 포함 자료형
//ir센서 값은 소수점이 중요하지 않다
uint16_t irfl_value, irfr_value, irmr_value, irbr_value, irbl_value, irml_value;
uint16_t black = 20000;

//psd센서
//★★★★ 20, 150, 60, 0으로 지정한 이유 : psd 헤더파일 설명 / GP2Y0A02 부품 사양서 확인
GP2A PSDfl(PC_3, 20, 150, 60, 0);
GP2A PSDf(PC_2, 20, 150, 60, 0);
GP2A PSDfr(PC_4, 20, 150, 60, 0);

//★★★★double로 지정한 이유 : float - 유효 7자리 실수, double - 유효 16자리 실수
//psd센서 값은 소수점이 중요하다
double PSDfl_value, PSDf_value, PSDfr_value;

//로봇 사라졌을 때 회전 판단
int turn_where = 2; //오른쪽 회전

//로봇 페이즈
int move_mode = 1; //조건 이전 상태
int back_mode = 0; //후진 필요하지 않은 상태
int turn_mode = 0; //회전 필요하지 않은 상태

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

//[함수 정의]
void reset();
void com_function();
void motor_setting();
void servo_setting(PwmOut &motor, double deg);
void sensor_read();

void go();
void go_fast();
void turnL();
void go_turnL();
void straight_turnL();
void turnL_fast();
void straight_turnL_fast();
void turnR();
void go_turnR();
void straight_turnR();
void turnR_fast();
void straight_turnR_fast();
void stop();
void back();
void back_fast();
void back_turnL();
void back_turnL_fast();
void back_turnR();
void back_turnR_fast();

void servo_move();
void wait_move();
void find_move_nothing();
void find_move();
void back_and_turn_move();

void where_disappear();

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//◆메인문

//[메인문]
int main(){

    //초기화
    reset();

    //영역 보기 : 라즈베리파이 - mbed 통신
    //★★★★com.gets(c, 3);으로 한 이유 : 위에 char c[2]로 크기가 2인 배열 생성 / pc.gets(c, 3);으로 크기가 2인 배열 통신 받음

    while(true){
        //ir, psd 값 읽기
        sensor_read();
        //로봇 회전 판단 기준
        where_disappear();

        //행동 : 조건 이전 상태
        if (move_mode == 1){
            wait_move();
        }
        //행동 : 조건 이후 상태
        else if (move_mode == 2){
            //서보 움직임
            servo_thread.start(servo_move); //얘는 main문의 while이랑은 상관없이 새로운 도화지인가? 그래서 함수 내에서 while문 따로 또 돌려줘야하나?
            back_and_turn_move();
            if (back_mode == 0 && turn_mode == 0){
                find_move_nothing();
                find_move();
            }
        }
        //모터 움직임 세팅
        motor_setting();
        // pc_mcu.printf("move_mode = %d\n", move_mode);
    }
}

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//◆함수정의

//[기본 단위 함수]

//통신
void com_function(){
    while (true){
        if(com.readable()){
            c = com.getc();
        }
    }
}

//초기화(타이머, 모터, 서보 모터)
void reset(){
    com_thread.start(com_function);
    timer1.reset();
    timer2.reset();
    pwmL.period_us(100);
    pwmR.period_us(100);
    servo.period_ms(20);
    servo_setting(servo, mid_ang);
}

//모터 속도 세팅
void motor_setting(){
    pwmL = lspd;
    pwmR = rspd;
}

//서보 모터 세팅
//★★★★template : 다양한 typename 활용 가능 (https://blog.naver.com/hikari1224/221482138453)
//★★★★void - return 불가 / template이나 int 같은 자료형 - return 가능
//★★★★현재 씨름로봇에서 template을 정의할 필요는 없지만 그냥 사용함
template<class T>
    T map(T x, T in_min, T in_max, T out_min, T out_max){
        return (out_max - out_min) / (in_max - in_min) * (x - in_min) + out_min;
    }

//★★★★★★★★포인터 : class형인 PwmOut에서 위에 정의한 PwmOut servo 대신에 motor라는 이름을 사용하겠다는 뜻
void servo_setting(PwmOut &motor, double deg){
    uint16_t pulseW=map<double>(deg, min_ang, max_ang, 500., 2500.);
    motor.pulsewidth_us(pulseW);
}

//ir, psd 값 읽기
//★★★★.read_u16으로 한 이유 : uint16_t - 16비트 절대값 자료형, int32_t - 32비트 음수 포함 자료형
//★★★★위에 irfl_value 초기값 선언해주어야 함
void sensor_read(){
    irfl_value = irfl.read_u16();
    irfr_value = irfr.read_u16();
    irmr_value = irmr.read_u16();
    irbr_value = irbr.read_u16();
    irbl_value = irbl.read_u16();
    irml_value = irml.read_u16();

    PSDfl_value = PSDfl.getDistance();
    PSDf_value = PSDf.getDistance();
    PSDfr_value = PSDfr.getDistance();
}

//전진
void go(){
    dirR = 1;
    dirL = 1;
    rspd = 0.30f;
    lspd = 0.30f;
}

//빠른 전진
void go_fast(){
    dirR = 1;
    dirL = 1;
    rspd = 0.60f;
    lspd = 0.60f;
}

//제자리 좌회전
void turnL(){
    dirR = 1;
    dirL = 0;
    rspd = 0.15f;
    lspd = 0.15f;
}

//좌회전 전진
void go_turnL(){
    dirR = 1;
    dirL = 1;
    rspd = 0.30f;
    lspd = 0.20f;
}

////좌회전 직진
void straight_turnL(){
    dirR = 1;
    dirL = 1;
    rspd = 0.30f;
    lspd = 0.25f;
}

//빠른 좌회전
void turnL_fast(){
    dirR = 1;
    dirL = 1;
    rspd = 0.60f;
    lspd = 0.40f;
}

//빠른 좌회전 직진
void straight_turnL_fast(){
    dirR = 1;
    dirL = 1;
    rspd = 0.60f;
    lspd = 0.50f;
}

//제자리 우회전
void turnR(){
    dirR = 0;
    dirL = 1;
    rspd = 0.15f;
    lspd = 0.15f;
}

//우회전 전진
void go_turnR(){
    dirR = 1;
    dirL = 1;
    rspd = 0.20f;
    lspd = 0.30f;
}

////우회전 직진
void straight_turnR(){
    dirR = 1;
    dirL = 1;
    rspd = 0.25f;
    lspd = 0.30f;
}

//빠른 우회전
void turnR_fast(){
    dirR = 1;
    dirL = 1;
    rspd = 0.40f;
    lspd = 0.60f;
}

//빠른 우회전 직진
void straight_turnR_fast(){
    dirR = 1;
    dirL = 1;
    rspd = 0.50f;
    lspd = 0.60f;
}

//정지
void stop(){
    dirR = 1;
    dirL = 1;
    rspd = 0.0f;
    lspd = 0.0f;
}

//후진
void back(){
    dirR = 0;
    dirL = 0;
    rspd = 0.30f;
    lspd = 0.30f;
}

//빠른 후진
void back_fast(){
    dirR = 0;
    dirL = 0;
    rspd = 0.42f;
    lspd = 0.42f;
}

//좌회전 후진
void back_turnL(){
    dirR = 0;
    dirL = 0;
    rspd = 0.30f;
    lspd = 0.10f;
}

//빠른 좌회전 후진
void back_turnL_fast(){
    dirR = 0;
    dirL = 0;
    rspd = 0.50f;
    lspd = 0.15f;
}

//우회전 후진
void back_turnR(){
    dirR = 0;
    dirL = 0;
    rspd = 0.10f;
    lspd = 0.30f;
}

//빠른 우회전 후진
void back_turnR_fast(){
    dirR = 0;
    dirL = 0;
    rspd = 0.15f;
    lspd = 0.50f;
}

//로봇 사라졌을 때 판단
void where_disappear(){
    if(c == 'l' || c == 'L'){
        turn_where = 1;
    }
    else if(c == 'r' || c == 'R'){
        turn_where = 2;
    }
    else if(c == 'm' || c == 'M'){
        turn_where = 3;
    }
}

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

//[행동 단위 함수]

//서보 움직임
void servo_move(){
    while(true){
    //★★★★strcmp(c, "FM") : c에서 오는 값이 "FM" 이라면
        if(c == 'l' || c == 'L'){ //화면 왼쪽 : 서보 왼쪽으로 (서보 각도 제한)
            ang = ang - 0.0015f;
            if(ang <= min_ang) ang = min_ang;
        }
        else if(c == 'r' || c == 'R'){ //화면 오른쪽 : 서보 오른쪽으로 (서보 각도 제한)
            ang = ang + 0.0015f;
            if(ang >= max_ang) ang = max_ang;
        }
        else if(c == 'm' || c == 'M'){ //화면 중간 : 서보 그대로
        }
        else if(c == 'n'){ //상대 로봇 안보임
            if(turn_where == 1){ //화면 왼쪽에서 사라짐 : 서보 왼쪽으로 (서보 각도 제한)
                ang = ang - 0.0015f;
                if(ang <= min_ang) ang = min_ang;
            }
            else if(turn_where == 2){ //화면 오른쪽에서 사라짐 : 서보 오른쪽으로 (서보 각도 제한)
                ang = ang + 0.0015f;
                if(ang >= max_ang) ang = max_ang;
            }
            else if(turn_where == 3){ //화면 가운데에서 사라짐 : 서보 그대로
            }
        }
        servo_setting(servo, ang); //서보 움직임
    }
}

//행동 : 조건 이전 상태 >> 서보 작동 x
void wait_move(){
    if(c == 'L' || c == 'R' || c == 'M' || c == 'G'){ //상대 로봇 가까움 >> 조건 해제 (기다릴 때 벽이 잡히지 않도록 거리 잘 설정)
        move_mode = 2;
        timer1.stop();
    }
    else if(c == 'l'){ //화면 왼쪽 멀음 : 제자리 회전
        turnL();
    }
    else if(c == 'r'){ //화면 오른쪽 멀음 : 제자리 우회전
        turnR();
    }
    else if(c == 'm'){ //화면 중간 멀음 : 정지
        stop();
        timer1.start();
        timer2.start();
    }
    else if(c == 'n'){  //상대 로봇 안보임 >> 제자리 회전
        if(turn_where == 1){ //화면 왼쪽에서 사라짐 : 제자리 좌회전
            turnL();
        }
        else if(turn_where == 2){ //화면 오른쪽에서 사라짐 : 제자리 우회전
            turnR();
        }
        else if(turn_where == 3){ //화면 가운데에서 사라짐 : 제자리 우회전
            turnL();
        }
    }
    if(timer1.read_ms() >= 8000){ //시간 45초 이상 >> 조건 해제
        move_mode = 2;
        timer1.stop();
    }
}

//행동 : 조건 이후 상태 >> 서보 작동
void find_move_nothing(){ //상대 로봇 안보임 & 서보 각도 최대 >> 제자리 회전
    if(c == 'n'){
        if(turn_where == 1 && ang == 0){ //화면 왼쪽에서 사라짐 & 서보 0도 : 제자리 좌회전
            turnL();
        }
        else if(turn_where == 2 && ang == 180){ //화면 오른쪽에서 사라짐 & 서보 180도 : 제자리 우회전
            turnR();
        }
        else if(turn_where == 3){ //화면 가운데서 사라짐 : 제자리 좌회전 (이유 없음)
            turnL();
        }
    }
}

//행동 : 조건 이후 상태 >> 서보 작동
void find_move(){
    // pc_mcu.printf("find_move = work\n");
    
    if(c == 'L' || c == 'R' || c == 'M' || c == 'G'){ //상대 로봇 가까움
        // pc_mcu.printf("find_move = near\n");
        if(70 <= ang && ang < 90){ //서보 왼쪽 가운데
            if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value > black){ //ir 앞
                // if(irml_value > black && irmr_value > black){
                //     straight_turnL_fast();
                // }
                // else {
                    back_turnL();
                // }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){ //ir 뒤
                // if(irml_value > black && irmr_value > black){
                //     back_mode = 1;
                // }
                // else {
                    back_mode = 1;
                // }
            }
            else if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value < black){ //ir 앞 + 뒤 오른쪽
                // if(irml_value > black && irmr_value > black){
                //     straight_turnL_fast();
                // }
                // else {
                    back_turnL();
                // }
            }
            else if(irfl_value < black && irfr_value < black && irbl_value < black && irbr_value > black){ //ir 앞 + 뒤 왼쪽
                // if(irml_value > black && irmr_value > black){
                //     straight_turnL_fast();
                // }
                // else {
                    back_turnR();
                // }
            }
            else {
                straight_turnL_fast();
            }
        }
        else if(90 <= ang && ang < 110){
            if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value > black){ //ir 앞
                // if(irml_value > black && irmr_value > black){
                //     straight_turnR_fast();
                // }
                // else {
                    back_turnR();
                // }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){ //ir 뒤
                // if(irml_value > black && irmr_value > black){
                //     back_mode = 1;
                // }
                // else {
                    back_mode = 1;
                // }
            }
            else if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value < black){ //ir 앞 + 뒤 오른쪽
                // if(irml_value > black && irmr_value > black){
                //     straight_turnR_fast();
                // }
                // else {
                    back_turnL();
                // }
            }
            else if(irfl_value < black && irfr_value < black && irbl_value < black && irbr_value > black){ //ir 앞 + 뒤 왼쪽
                // if(irml_value > black && irmr_value > black){
                //     straight_turnR_fast();
                // }
                // else {
                    back_turnR();
                // }
            }
            else {
                straight_turnR_fast();
            }
        }
        else if(40 <= ang && ang < 70){
            if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value > black){ //ir 앞
                // if(irml_value > black && irmr_value > black){
                //     turnL_fast();
                // }
                // else {
                    back_turnL();
                // }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){ //ir 뒤
                // if(irml_value > black && irmr_value > black){
                //     back_mode = 2;
                // }
                // else {
                    back_mode = 2;
                // }
            }
            else if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value < black){ //ir 앞 + 뒤 오른쪽
                // if(irml_value > black && irmr_value > black){
                //     turnL_fast();
                // }
                // else {
                    back_turnL();
                // }
            }
            else if(irfl_value < black && irfr_value < black && irbl_value < black && irbr_value > black){ //ir 앞 + 뒤 왼쪽
                // if(irml_value > black && irmr_value > black){
                //     turnL_fast();
                // }
                // else {
                    back_turnR();
                // }
            }
            else {
                turnL_fast();
            }
        }
        else if(110 <= ang && ang < 140){
            if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value > black){ //ir 앞
                // if(irml_value > black && irmr_value > black){
                //     turnR_fast();
                // }
                // else {
                    back_turnR();
                // }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){ //ir 뒤
                // if(irml_value > black && irmr_value > black){
                //     back_mode = 1;
                // }
                // else {
                    back_mode = 1;
                // }
            }
            else if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value < black){ //ir 앞 + 뒤 오른쪽
                // if(irml_value > black && irmr_value > black){
                //     turnR_fast();
                // }
                // else {
                    back_turnL();
                // }
            }
            else if(irfl_value < black && irfr_value < black && irbl_value < black && irbr_value > black){ //ir 앞 + 뒤 왼쪽
                // if(irml_value > black && irmr_value > black){
                //     turnR_fast();
                // }
                // else {
                    back_turnR();
                // }
            }
            else {
                turnR_fast();
            }
        }
        else if(ang < 40){
            turnL();
        }
        else if(140 <= ang){
            turnR();
        }
    }
    if(c == 'l' || c == 'r' || c == 'm'){ //상대 로봇 멀리 보임
        // pc_mcu.printf("find_mode = long\n");
        if(70 <= ang && ang < 90){ //서보 왼쪽 가운데(서보 67.5도 ~ 90도 & 로봇 원통 큼)
            if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value > black){ //ir 앞만 색 영역 o : 빠른 전진
                // if(irml_value > black && irmr_value > black){
                    turn_mode = 1;
                // }
                // pc_mcu.printf("find_move = 7090f2b0\n");
            }
            else if(irfl_value < black && irbl_value < black){ //ir 왼쪽만 + 오른쪽 하나 색 영역 o : 우회 직진
                if(irfr_value < black || irbr_value < black){
                    straight_turnR();
                    // pc_mcu.printf("find_move = 7090l2r12\n");
                }
            }
            else if(irfr_value < black && irbr_value < black){ //ir 오른쪽만 + 왼쪽 하나 색 영역 o : 좌회 직진
                if(irfl_value < black || irbl_value < black){
                    straight_turnL();
                    // pc_mcu.printf("find_move = 7090r2l12\n");
                }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){  //ir 뒤만 색 영역 o : 빠른 전진
                // if(irml_value > black && irmr_value > black){
                    straight_turnL_fast();
                    // pc_mcu.printf("find_move = 7090f0b2\n");
                // }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){  //ir 뒤만 + 중간 하나 색 영역 o : 빠른 전진
                // if(irml_value < black || irmr_value < black){
                    straight_turnL_fast();
                    // pc_mcu.printf("find_move = 7090f0b2\n");
                // }
            }
            else { //ir 그 외의 경우 o : 전진
                straight_turnL();
                // pc_mcu.printf("find_move = 7090else\n");
            }
        }
        else if(90 <= ang && ang < 110){ //서보 오른쪽 가운데(서보 90도 ~ 112.5도 & 로봇 원통 큼)
            if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value > black){ //ir 앞만 색 영역 o : 빠른 전진
                // if(irml_value > black && irmr_value > black){
                    turn_mode = 2;
                    // pc_mcu.printf("find_move = 90110f2b0\n");
                // }
            }
            else if(irfl_value < black && irbl_value < black){ //ir 왼쪽만 + 오른쪽 하나 색 영역 o : 우회 직진
                if(irfr_value < black || irbr_value < black){
                    straight_turnR();
                    // pc_mcu.printf("find_move = 90110l2r12\n");
                }
            }
            else if(irfr_value < black && irbr_value < black){ //ir 오른쪽만 + 왼쪽 하나 색 영역 o : 좌회 직진
                if(irfl_value < black || irbl_value < black){
                    straight_turnL();
                    // pc_mcu.printf("find_move = 90110r2l12\n");
                }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){  //ir 뒤만 색 영역 o : 빠른 전진
                // if(irml_value > black && irmr_value > black){
                    straight_turnR_fast();
                    // pc_mcu.printf("find_move = 90110f0b2\n");
                // }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){  //ir 뒤만 + 중간 하나 색 영역 o : 빠른 전진
                // if(irml_value < black || irmr_value < black){
                    straight_turnR_fast();
                    // pc_mcu.printf("find_move = 90110f0b2\n");
                // }
            }
            else { //ir 그 외의 경우 o : 전진
                straight_turnR();
                // pc_mcu.printf("find_move = 90110else\n");
            }
        }
        else if(30 <= ang && ang < 70){ //서보 왼쪽(서보 30도 ~ 67.5도 & 로봇 원통 큼)
            if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value > black){ //ir 앞만 색 영역 o : 빠른 전진
                // if(irml_value > black && irmr_value > black){
                    turn_mode = 1;
                    // pc_mcu.printf("find_move = 3070f2b0\n");
                // }
            }
            else if(irfl_value < black && irbl_value < black){ //ir 왼쪽만 + 오른쪽 하나 색 영역 o : 우회 직진
                if(irfr_value < black || irbr_value < black){
                    straight_turnR();
                    // pc_mcu.printf("find_move = 3070l2r12\n");
                }
            }
            else if(irfr_value < black && irbr_value < black){ //ir 오른쪽만 + 왼쪽 하나 색 영역 o : 좌회 직진
                if(irfl_value < black || irbl_value < black){
                    straight_turnL();
                    // pc_mcu.printf("find_move = 3070r2l12\n");
                }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){ //ir 뒤만 색 영역 o : 빠른 좌회전
                // if(irml_value > black && irmr_value > black){
                    turnL_fast();
                    // pc_mcu.printf("find_move = 3070f0b2\n");
                // }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){ //ir 뒤만 + 중간 하나 색 영역 o : 빠른 좌회전
                // if(irml_value < black || irmr_value < black){
                    turnL_fast();
                    // pc_mcu.printf("find_move = 3070f0b2\n");
                // }
            }
            else { //ir 그 외의 경우 o : 좌회 직진
                go_turnL();
                // pc_mcu.printf("find_move = 3070else\n");
            }
        }
        else if(110 <= ang && ang < 140){ //서보 오른쪽(서보 112.5도 ~ 150도 & 로봇 원통 큼)
            if(irfl_value < black && irfr_value < black && irbl_value > black && irbr_value > black){ //ir 앞만 색 영역 o : 빠른 전진
                // if(irml_value > black && irmr_value > black){
                    turn_mode = 2;
                    // pc_mcu.printf("find_move = 110140f2b0\n");
                // }
            }
            else if(irfl_value < black && irbl_value < black){ //ir 왼쪽만 + 오른쪽 하나 색 영역 o : 우회 직진
                if(irfr_value < black || irbr_value < black){
                    straight_turnR();
                    // pc_mcu.printf("find_move = 110140l2r12\n");
                }
            }
            else if(irfr_value < black && irbr_value < black){ //ir 오른쪽만 + 왼쪽 하나 색 영역 o : 좌회 직진
                if(irfl_value < black || irbl_value < black){
                    straight_turnL();
                    // pc_mcu.printf("find_move = 110140r2l12\n");
                }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){ //ir 뒤만 색 영역 o : 1초간 빠른 좌회전 후진
                // if(irml_value > black && irmr_value > black){
                    turnR_fast();
                    // pc_mcu.printf("find_move = 110140f0b2\n");
                // }
            }
            else if(irfl_value > black && irfr_value > black && irbl_value < black && irbr_value < black){ //ir 뒤만 + 중간 하나 색 영역 o : 1초간 빠른 좌회전 후진
                // if(irml_value < black || irmr_value < black){
                    turnR_fast();
                    // pc_mcu.printf("find_move = 110140f0b2\n");
                // }
            }
            else { //ir 그 외의 경우 o : 우회 직진
                go_turnR();
                // pc_mcu.printf("find_move = 110140else\n");
            }
        }
        else if(ang < 40){ //서보 매우 왼쪽 : 제자리 좌회전
            turnL();
            // pc_mcu.printf("find_move = 40\n");
        }
        else if(140 <= ang){ //서보 매우 오른쪽 : 제자리 우회전
            turnR();
            // pc_mcu.printf("find_move = 140\n");
        }
    }
}

// 행동 : 후진 모드
void back_and_turn_move(){
    if(back_mode == 1){ //왼쪽으로 후진해야할 경우 : 빠른 좌회전 후진
        back_turnL_fast();

        // pc_mcu.printf("back_mode = 1\n");

        if(irfl_value > black && irfr_value > black){ //ir 앞 모두 검은색 : 기존 동작으로
            back_mode = 0;
        }
    }
    else if(back_mode == 2){ //오른쪽으로 후진해야할 경우 : 빠른 오른쪽 후진
        back_turnR_fast();

        // pc_mcu.printf("back_mode = 2\n");

        if(irfl_value > black && irfr_value > black){ //ir 앞 모두 검은색 : 기존 동작으로
            back_mode = 0;
        }
    }
    if(turn_mode == 1){
        turnL();

        // pc_mcu.printf("turn_mode = 1\n");
        
        if(irfl_value > black){ //ir 왼쪽 앞 검은색 : 기존 동작으로
            turn_mode = 0;
        }
    }
    else if(turn_mode == 2){
        turnR();

        // pc_mcu.printf("turn_mode = 2\n");
        
        if(irfr_value > black){ //ir 오른쪽 앞 검은색 : 기존 동작으로
            turn_mode = 0;
        }
    }
}

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

//[모든 함수 요약]

//기본 단위 함수
    // 초기화(타이머, 모터, 서보 모터) void reset();
    // 모터 속도 세팅 void motor_setting();
    // 서보 모터 세팅 void servo_setting(PwmOut &motor, double deg);
    // 센서 읽기 void sensor_read();

    // 전진 void go();
    // 빠른 전진 void go_fast();
    // 제자리 좌회전 void turnL();
    // 좌회전 전진 void go_turnL();
    // 좌회전 직진 void straight_turnL();
    // 빠른 좌회전 void turnL_fast();
    // 빠른 좌회전 직진 void straight_turnL_fast();
    // 제자리 우회전 void turnR();
    // 우회전 전진 void go_turnR();
    // 우회전 직진 void straight_turnR();
    // 빠른 우회전 void turnR_fast();
    // 빠른 우회전 직진 void straight_turnR_fast();
    // 정지 void stop();
    // 후진 void back();
    // 빠른 후진 void back_fast();
    // 좌회전 후진 void back_turnL();
    // 빠른 좌회전 후진 void back_turnL_fast();
    // 우회전 후진 void back_turnR();
    // 빠른 우회전 후진 void back_turnR_fast();

//행동 단위 함수
    // 서보 무브 void servo_move();
    // 행동 : 조건 이전 상태 void wait_move();
    // 행동 : 조건 이후 안보임 상태 void find_move_nothing();
    // 행동 : 조건 이후 보임 상태 void find_move();
    // 행동 : 뒤 영역 시 검정 영역까지 후진 void back_and_turn_move();
    
    // 로봇 사라졌을 때 판단 void where_disappear();

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

//[모든 함수 세부]

// 전제조건 : 카메라가 서보 범위 내에서 항상 상대방을 보고 있음
//            처음엔 카메라가 아무것도 못 잡으니까 servomove함수 사용하지 않고 있다가 로봇을 돌려 상대 로봇을 포착하고 나서 사용
//            카메라가 서보 범위 밖으로 벗어났을 경우에는 로봇을 돌려서 찾음
//            상대방이 가까울 때 / 멀 때 서보 pwm값을 다르게 주는 코드 작성

//영역 넣어야할 코드
//영역이 상대 로봇 앞에 있을 때, 뒤에 있을 때 고려해서 짜기
//싸울 때 근처에 어떤 영역이 있는 지 파악하기

//카메라 psd 넣어야할 코드
//화면이 보지 않는 곳의 psd는 전부 벽
//서보를 정확하게 돌릴 수 있어야 전투 상황에서 못 찾는 경우가 없음
//서보모터, psd의 값에 따라 모터 조절 다르게 주는 것이 베스트
//코드의 순서 고려

//5초 연속 득점 보다는 나오게 한 다음에 바로 득점 먹이는 게 더 이득아님?

//서보모터 & 카메라 문제점
//360도 회전 가능 카메라 필요 이유 : 현재 앞을 보면서 아무런 명령도 내리지 않음. 상대방을 보는 용도로만 사용하고 있음
//카메라가 360도 돌 수 있으면 화각이 넓지 않아도 되고, 화각이 안되서 로봇을 돌리는 리스크도 해결 가능
//생각해보니 어차피 돌아야되서 리스크가 크진 않을 듯 + 화각이 넓어야 영역을 보고 상대를 밀 수 있음
//상대방을 카메라에 맞춰서 돌리게 한다면 상관은 없음. 근데 그러면 애초에 서보모터를 돌릴 필요도 없음
//해결책 : 360도 회전 가능한 모터 사용 + psd 10개 사용
//해결책2 : 모터 그대로 쓰고, 리스크 감수
//결과 : 360도 도는 서보모터는 너무 비쌈 / 스텝모터는 너무 무거움 >> 화각 넓은 카메라로만 전환

//psd 문제점
//psd 앞에 2개 더 달 경우에 psd값이 50cm 정도되야 정확해짐. 아닌 경우 10cm 정도 되야 정확해짐
//내 로봇 앞이 상대 로봇을 계속 보고 있을 경우에는 psd 없어도 상관 없음
//해결책 : 카메라 그대로 사용 + psd를 앞 옆에 4개 더 달음 + 코드 수정
//결과 : 카메라 영역 크기 실험해서 판단 or 카메라 그대로 사용 + psd를 앞 옆에 4개 더 달음 + 코드 수정
//psd의 쓸모는 상대가 가까이 있을 때 밖에 없음. 나머지는 벽일 수도 있으므로 의미 없는 데이터
//상대방이 벽쪽에 있는 경우를 제외하면 전부 밀어버리면 득점
//그렇다는 것은 내가 벽쪽에 붙어있으면 유리하다는 뜻

//정확하게 박으려면 수학적인 조작이 필요할 듯

//함수 종류
    // ir, psd 센서 읽기
    // 통신
    // 영역보기, 서보 움직임
    // 모터 움직임

//영상처리 >> psd >> ir >> 움직임

//서보 움직임
    //상대 로봇 보임 >> 서보 움직임
        //화면 왼쪽 : 서보 왼쪽으로 (서보 각도 제한)
        //화면 오른쪽 : 서보 오른쪽으로 (서보 각도 제한)
        //화면 중간 : 서보 그대로
    //상대 로봇 안보임 & 서보 각도 최대 x >> 서보 움직임
        //화면 왼쪽에서 사라짐 : 서보 왼쪽으로 (서보 각도 제한)
        //화면 오른쪽에서 사라짐 : 서보 오른쪽으로 (서보 각도 제한)
        //화면 가운데에서 사라짐 : 서보 그대로
            
//행동 : 조건 이전 상태
    //상대 로봇 보임
        //상대 로봇 가까움 >> 조건 해제 (기다릴 때 벽이 잡히지 않도록 거리 잘 설정)
        //상대 로봇 멀음 >> 제자리 회전
            //화면 왼쪽 : 제자리 좌회전
            //화면 오른쪽 : 제자리 우회전
            //화면 중간 : 정지, 타이머 시작
    //상대 로봇 안보임 >> 제자리 회전
        //화면 왼쪽에서 사라짐 : 제자리 좌회전
        //화면 오른쪽에서 사라짐 : 제자리 우회전
        //화면 가운데에서 사라짐 : 제자리 좌회전 (이유 없음)
    //시간 45초 이상 >> 조건 해제

//행동 : 조건 이후 상태 >> 서보 작동
    //상대 로봇 안보임 & 서보 각도 최대 >> 제자리 회전
        //화면 왼쪽에서 사라짐 & 서보 0도 : 제자리 좌회전
        //화면 오른쪽에서 사라짐 & 서보 180도 : 제자리 우회전
        //화면 가운데에서 사라짐 : 제자리 좌회전 (이유 없음)

//행동 : 조건 이후 상태 >> 서보 작동
    // 기존 모드
        //상대 로봇 가까이 보임
            //서보 가운데 (서보 67.5도 ~ 112.5도 & 로봇 원통 큼 - 가장 우선 순위)
                //ir 앞만 색 영역 o : 빠른 전진
                //ir 앞만 + 중간 하나 색 영역 o : 후진
                //ir 뒤만 색 영역 o : 1초간 빠른 좌회전 후진 (이유 없음 그냥 좌회전) ■왼쪽 오른쪽 psd 있다면 벽과의 거리가 먼 곳으로 돌게 할 수 있음
                //ir 뒤만 + 중간 하나 색 영역 o : 1초간 빠른 좌회전 후진 (이유 없음 그냥 좌회전)
                //ir 그 외의 경우 o : 빠른 전진
            //서보 왼쪽 (서보 30도 ~ 67.5도 & 로봇 원통 큼 - 2번째 우선 순위)
                //ir 앞만 색 영역 o : 빠른 좌회전
                //ir 앞만 + 중간 하나 색 영역 o : 좌회전 후진
                //ir 뒤만 색 영역 o : 1초간 빠른 우회전 후진
                //ir 뒤만 + 중간 하나 색 영역 o : 1초간 빠른 우회전 후진
                //ir 그 외의 경우 o : 빠른 좌회전
            //서보 오른쪽 (서보 112.5도 ~ 150도 & 로봇 원통 큼 -2번째 우선 순위)
                //ir 앞만 색 영역 o : 빠른 우회전
                //ir 앞만 + 중간 하나 색 영역 o : 우회전 후진
                //ir 뒤만 색 영역 o : 1초간 빠른 좌회전 후진
                //ir 뒤만 + 중간 하나 색 영역 o : 1초간 빠른 좌회전 후진
                //ir 그 외의 경우 o : 빠른 우회전
            //서보 매우 왼쪽 : 제자리 좌회전
            //서보 매우 오른쪽 : 제자리 우회전

        //상대 로봇 멀리 보임
            //서보 왼쪽 가운데
                //ir 앞만 색 영역 : 좌회전
                //ir 왼쪽 색 영역 많고, 앞 오른쪽 색 영역 o : 우회 직진
                //ir 오른쪽 색 영역 많고, 앞 왼쪽 색 영역 o : 좌회 직진
                //ir 뒤만 색 영역 o : 빠른 전진
                //ir 뒤만 + 중간 하나 색 영역 o : 빠른 전진
                //ir 그 외의 경우 o : 전진
            //서보 오른쪽 가운데
                //ir 앞만 색 영역 : 우회전
                //ir 왼쪽 색 영역 많고, 앞 오른쪽 색 영역 o : 우회 직진
                //ir 오른쪽 색 영역 많고, 앞 왼쪽 색 영역 o : 좌회 직진
                //ir 뒤만 색 영역 o : 빠른 전진
                //ir 뒤만 + 중간 하나 색 영역 o : 빠른 전진
                //ir 그 외의 경우 o : 전진
            //서보 왼쪽
                //ir 오른쪽 색 영역 많고, 왼쪽 하나라도 색 영역 o : 좌회 직진
                //ir 왼쪽 색 영역 많고, 오른쪽 하나라도 색 영역 o : 우회 직진
                //ir 뒤만 색 영역 o : 빠른 좌회전
                //ir 뒤만 + 중간 하나 색 영역 o : 빠른 좌회전
                //ir 그 외의 경우 o : 좌회 직진
            //서보 오른쪽
                //ir 왼쪽 색 영역 많고, 오른쪽 하나라도 색 영역 o : 우회 직진
                //ir 오른쪽 색 영역 많고, 왼쪽 하나라도 색 영역 o : 좌회 직진
                //ir 뒤만 색 영역 o : 빠른 우회전
                //ir 뒤만 + 중간 하나 색 영역 o : 빠른 우회전
                //ir 그 외의 경우 o : 우회 직진
            //서보 매우 왼쪽 : 제자리 좌회전
            //서보 매우 오른쪽 : 제자리 우회전



// 행동 : 후진 모드 & 회전 모드
    //왼쪽으로 후진해야할 경우 : 빠른 좌회전 후진
        //ir 앞 모두 검은색 : 기존 모드
    //오른쪽으로 후진해야할 경우 : 빠른 오른쪽 후진
        //ir 앞 모두 검은색 : 기존 모드

    //왼쪽으로 돌아야할 경우 : 제자리 좌회전
        //ir 왼쪽 앞 검은색 : 기존 모드
    //오른쪽으로 돌아야할 경우 : 제자리 우회전
        //ir 오른쪽 앞 검은색 : 기존 모드

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

//[로봇 전체 움직임]

//메인문
    //초기화

//반복문
    //ir, psd 값 읽기
    //모터 움직임
    //영역 보기 : 라즈베리파이 - mbed 통신
    //서보 움직임
    //로봇 회전 판단 기준

    //행동 : 조건 이전 상태
    //행동 : 조건 이후 상태
        //ir 앞중간 잡힐 때까지 카메라 방향에 psd가 잡히면 공격
        //ir 뒤 잡히면 빠져나옴

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

//[기타]

//라즈베리 파이에서 받는 명령어
    // 빨간색 영역이 왼쪽에 위치 : 'L'
    // 빨간색 영역이 오른쪽에 위치 : 'R'
    // 빨간색 영역이 양쪽에 위치 (빨간색 영역이 중앙에 위치하고 로봇이 가린 경우) : 'M'
    // 파란색 영역이 왼쪽에 위치 : 'L'
    // 파란색 영역이 오른쪽에 위치 : 'R'
    // 파란색 영역이 양쪽에 위치 (파란색 영역이 중앙에 위치하고 로봇이 가린 경우) : 'M'
    // 로봇이 보임 + 영역이 안보임 : 'G'
    // 로봇이 전방 좌측 방향에 위치 : 'l'
    // 로봇이 전방 중앙에 위치 : 'm'
    // 로봇이 전방 우측 방향에 위치 : 'r'
    // 로봇이 화면에 보이지 않음 : 'n'

//핀 사용 : pin map 보면서 해당 부품에 사용할 핀 자리 고려 >> 부품 선정
// >> 해당 부품에서 사용할 함수 정의 or 라이브러리 찾아서 include >> 라이브러리 설명 확인하며 핀 선언
// >> 초기값 설정 & 함수 정의 >> 메인문 완성

//핀 사용
    // D8(PA_9) : servo motor
    // PC_9 : MotorDriverDirR
    // D15(PB_8) : MotorDriverPwmR
    // top right side Ground : MotorDriverR
    // D10(PB_6) : MotorDriverDirL
    // D9(PC_7) : MotorDriverPwmL
    // top right side Ground : MotorDriverL
    // A0(PA_0) : ir0 앞 왼쪽 대각선
    // A1(PA_1) : ir1 앞 오른쪽 대각선
    // A2(PA_4) : ir2 오른쪽
    // A3(PB_0) : ir3 뒤 오른쪽 대각선
    // A4(PC_1) : ir4 뒤 왼쪽 대각선
    // A5(PC_0) : ir5 왼쪽
    // PC_3 : GP2A PSD0 앞 왼쪽 대각선
    // PC_2 : GP2A PSD1 정면 20~150cm
    // PC_4 : GP2A PSD2 앞 오른족 대각선

    // PB_1 : GP2A PSD3 우측
    //GP2A PSD4 뒤 오른쪽 대각선
    //GP2A PSD5 후면 
    //GP2A PSD6 뒤 왼쪽 대각선
    // PC_5 : GP2A PSD7 좌측