//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//◆선언 & 초기값

#include "mbed.h"
#include "rtos.h"
#include "GP2A.h"

//[통신 + 타이머 + 모터 + 센서 핀 선언 & 초기 값]
//uart 통신
//★★★★
Serial com(USBTX, USBRX, 115200); //USB포트 대신 PA_2, PA_3 사용 가능

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

//psd센서
//★★★★ 25, 150, 60, 0으로 지정한 이유 : psd 헤더파일 설명 / GP2Y0A02 부품 사양서 확인
GP2A PSDfl(PC_3, 25, 150, 60, 0);
GP2A PSDf(PC_2, 25, 150, 60, 0);
GP2A PSDfr(PC_4, 25, 150, 60, 0);

//★★★★double로 지정한 이유 : float - 유효 7자리 실수, double - 유효 16자리 실수
//psd센서 값은 소수점이 중요하다
double PSDfl_value, PSDf_value, PSDfr_value;

// 서보
PwmOut servo(PA_9);

float min_ang = 0;
float mid_ang = 90;
float max_ang = 180;
float ang = 90;

int flag = 0;

template <class T>
T map(T x, T in_min, T in_max, T out_min, T out_max);

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

//[함수 정의]
void sensor_read();
void servosetting(PwmOut &motor, float deg);
void servomove();

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//◆메인문

//[메인문]
int main(){
    //영역 보기 : 라즈베리파이 - mbed 통신
    //★★★★com.gets(c, 3);으로 한 이유 : 위에 char c[2]로 크기가 2인 배열 생성 / pc.gets(c, 3);으로 크기가 2인 배열 통신 받음
    servosetting(servo,mid_ang);
    while(true){
        //ir, psd 값 읽기
        sensor_read();
        com.printf("%d // %d // %d x// %d // %d // %d\n", irfl_value, irfr_value, irml_value, irmr_value, irbl_value, irbr_value);
        //com.printf("%d\n", irfl_value);
        //com.printf("PSDfl_value = %f ///// PSDf_value = %f ///// PSDfr_value = %f\n", PSDfl_value, PSDf_value, PSDfr_value);

        // if(ang <= max_ang){
        //     ang = ang + 0.1f;
        // }
        // if(flag == 1) {
        //     ang = ang + 1.0f;
        //     if(ang >= max_ang){
        //         flag = 0;
        //     }
        // }
        // else if(flag == 0){
        //     ang = ang - 1.0f;
        //     if (ang <= min_ang){
        //         flag = 1;
        //     }
        // }
        // servosetting(servo, ang);
    }
}

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//◆함수정의

//[기본 단위 함수]
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

void servosetting(PwmOut &motor, float deg){
    float pulseW=map<float>(deg, 0, 180, 500.,2500.);
    motor.pulsewidth_us(pulseW);
}

template <class T>
    T map(T x, T in_min, T in_max, T out_min, T out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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