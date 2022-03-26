/*
● Microcontroller
    1. 프로세서
        (0) 개별 논리소자
            - 디지털 논리회로 구성 by 개별 논리소자
            ex. TTL, CMOS ♣♣♣
            1] 특징
                [0] fixed logic
                [1] 디지털 논리회로 수정 : 수정 hard
        (1) 마이크로 프로세서(MPU = MicroProcessor Unit)
            - 프로그래밍을 통한 수정
            1] 특징
                [0] programmable logic
                [1] 프로그램 수정 : 수정 easy
                [2] 유지보수 / 업그레이드 용이
                [3] 소형화 / 소비전력 작음
                [4] 회로 단순화 : 신뢰성 향상 ♣♣♣
                [5] 작은 CPU 역할 : 프로그래머가 작성한 프로그램(Firmware)대로 동작 ♣♣♣
                [6] 주변장치와 Interface : memory, I/O port, ADC, DAC, communications chip 등 주변장치와 interface by 외부장치와 정보교환 가능한 BUS ♣♣♣
            2] 구조
                // 강의자료 참조
        (2) 마이크로 컨트롤러(MCU = MicroController Unit) = One-chip = 마이콤 = SOC(System On Chip)
            1] 특징
                [0] 개념 : 단일 IC 집적 = MPU + Memory + GPIO + Timer/Counter + Communications
                [1] 하나의 작은 컴퓨터
                [2] 저전력 / 빠른 개발 
                [3] MPU에 비해 소형화 / 저렴한 시스템 구성 ♣♣♣
                [4] 활용 : 간단한 장치 or 제어기 구축 ♣♣♣
            2] 구조
                // 강의자료 참조
            3] 예시
                - Intel : 8051
                - Microchips : PIC
                - ATmel : AVR
        (3) DSP = Digital Signal Processor ♣♣♣
            1] 특징
                [1] 개념 : 특수 MPU = MPU + 디지털 신호처리에 적합한 연산기능
                [2] 활용 : Audio, Video 등 처리
    2. ARM
        1) 특징
            (1) 원래 Acorn computers라는 교육용 컴퓨터 회사
            (1) ARM(Advanced RISC Machines) Fabless 반도체 회사
            (2) RISC 프로세서 : 임베디드 특화
            (3) 저전력 / 고성능 mobile 프로세서 절대 강자
            (4) 세계 대기업들의 ARM 코어 사용
        2) 종류
            (1) Classic : 예전 모델
            (2) Embedded : MCU
                ex. 드론, 세탁기
            (3) Real-time : Real time system
                ex. 자동차, 로봇팔
            (4) Application : 고기능, 고성능 운영체제
                ex. 고성능 휴대폰, 노트북
    3. 프로그래밍
        1) MPU 프로그래밍
            (1) 과거 : 어셈블리어
                - 기계에게 효율적 but 프로그래머에게 비효율적
            (2) 과도기 : C(고급 언어)
                - 크로스 컴파일러 사용
                - 초창기 컴파일러의 기술적 한계로, 일부 중요한 부분은 직접 어셈블리어 코딩
            (3) 현재 : C++(고급 언어)
                - 크로스 컴파일러 사용
                - 프로세서 속도 증가, 컴파일러 기술 증가로 어셈블리어 거의 사용 안함
        2) MCU 프로그래밍
            (1) 과거 : 레지스터 접근
                - 상태, 제어, 입출력 담당 레지스터 접근
                - 방대한 양의 레지스터 숙지
                - 많은 시간 소요
            (2) 현재 : 레지스터 접근 거의 안함
                - 레지스터에 대한 직접적 접근 필요 없음
                - 추상적 함수들만으로 프로그래밍 가능
                - 적은 시간 소요(비전공자에게 유용)

    +a)
        1) RISC(Reduced Instruction Set Computer) 프로세서
            (1) 특징
                1] 명령어 단순화 / 개수 최소화
                2] 장점
                    - 효율적 하드웨어 사용
                    - 병렬 수행 like 파이프라인
                3] 단점
                    - 컴파일러 코드 최적화 복잡
                    - 프로그램 길어짐
        2) CISC(Complex Instruction Set Computer) 프로세서

● Mbed
    1. Mbed //♣♣♣
        1) 개념 : Cortex-M 마이크로컨트롤러 프로그램용 OS
            - 온라인 소프트웨어 개발 툴 + USB 메모리 & 전원
        2) 특징 //♣♣♣
            (1) 장점
                1] 개발 난이도
                    - 개발 환경 구축 신속
                    - Cortex-M 칩 : 브랜드, 부품 번호 변경에 대한 부담x
                    - 오픈 소스 : 방대한 C++ 기반 Mbed 라이브러리 in 온라인
                    - 레지스터 레벨 고려x : 복잡한 레지스터 고려x
                    - Mbed API 일관성 : 몇몇 API 기능 이해만으로 나머지 학습 용이
                2] 성능
                    - 낮은 가격 : 8비트 수준 가격으로 32비트 고성능 시스템 구축
                    - IOT 지향 : 연결성(Ethernet, USB, Bluetooth, CAN 등)
                    - RTOS 지원 : 실시간 프로그래밍
            (1) 장점
                1] 사용성 편리
                2] 회로 몰라도 개발 가능
                3] 별도 다운로드 장비 불필요
                4] 방대한 라이브러리 in 온라인
                5] 고성능 ARM 코어의 다양한 기능(RTOS, 연결성 등)
                    - Ethernet, USBHost, USBDevice, SPI, I2C, CAN, AnalogIn, PwmOut, AnalogOut, ...
                6] 많은 보드, 모듈, 부품 지원
            (2) 단점
                1] 레지스터 레벨 디버깅 불가
        3) Nucleo Board //♣♣♣
            (1) 특징
                1] 제조사 : STMicroelectronics
                2] 개발보드 : 수많은 lineup 존재
                    - Cortex-M
        4) Nucleo-f401re datasheet //♣♣♣
            (1) 프로세서 : STM32F401RET6
                1] 특징
                    [0] LQFP64 : 칩 패키지
                    [1] Core
                        - ARM32-bit Cortex-M4 CPU
                        - CPU frequency : max 84Mhz
                        - FPU : 부동소수점 연산기
                        - 연산속도 : 105MIPS
                    [2] Memory
                        - 프로그램 메모리 : 512KB Flash
                        - 데이터 메모리 : 96KB SRAM
                    [3] 동작 전원
                        - VDD : 1.7V ~ 3.6V
                    [4] USB
                        - USB 2.0 OTG FS
                    [5] GPIO
                        - GPIO : 50개. 모두 외부인터럽트 가능
                    [6] ADC
                        - ADC : 12bit. 16channel
                    [7] 통신
                        - USART/UART : 4개
                        - I2C : 3개
                        - SPI : 3개
                        - SDIO : 1개
                    [8] Timer
                        - RTC(Real Time Clock) : 실시간 클럭 내장
                        - Advanced-Control Timer : 3상 PWM 발생
                        - General Purpose Timers : 16비트 5개, 32비트 2개
                        - Watchdog Timers : 2개
            (2) ST-LINK/V2-1 debugger/programmer 내장 //♣♣♣
                - 점퍼 변경으로 ST-LINK/V2-1로도 사용 가능
            (3) USB mini B type : 3가지 접속 방법 제공 //♣♣♣
                1] Virtual COM port : 직렬 통신
                2] Mass storage(USB Disk drive)
                3] Debug port
            (4) Pin
                1] Arduino Uno R3 호환핀
                2] Morpho headers : STM32 I/O 핀을 위한 확장핀
            (5) 기타
                1] LED
                2] BUTTON
                3] RESET BUTTON
            (6) 전원 공급
                - 유연한 전원 공급 : 3.3V, 5V, 7V ~ 12V
        5) Mbed API //♣♣♣
            (1) Mbed OS 구조
                1] User Application Code -> Mbed OS5 API -> H/W Interface
                    [1] 핵심 Mbed OS5 API
                        - Core
                            Drivers : MCU 다양한 장치들에 대한 API
                            Platform : MCU 기능에 대한 기초 인프라
                            RTOS : Thread, Mutex 등 실시간 운영체제 관련 API
                            Storage : SD카드, Flash Memory 등 다양한 저장장치 관련 기능
                            USB : 마우스, 키보드, HID 장치, 오디오 장치 등 다양한 주변장치 역할
                            ...
                        - Connectivity
                            Network Socket : TCP, UDP를 근간으로 하는 소켓 만들어 데이터 주고 받음
                            Network Interface : Ethernet, WiFi, Cellular 등 네트워크 인터페이스 주고 받음
                            Bluetooth : BLE(Bluetooth Low Energy) 관련 기능
                            NFC : NFC 통신기능
                            ...
                        - Security
                            Security : 장치의 보안, 통신
    2. Mbed 프로그래밍 순서
        1) import 마법사
            - 이미 작성된 프로그램 수정
        1) New project
            1] 기본 예제 (mbed 라이브러리)
                - 이미 작성된 프로그램 수정
            2] Empty 프로젝트 + import mbed 라이브러리 + cpp 파일 생성
                - 프로그램 직접 생성
        2) 코드 수정
        3) Compile
        4) 보드 Sub 연결
        5) bin 파일 drag & drop

    +a)
        1) 유용한 기능 : 키워드 클릭 시 클래스 가이드 표시
            - 정의 위치, 관련 문서 등 확인 가능
        2) C vs C++ //♣♣♣
            (1) 특징 
                - C ⊂ C++
                1] C
                    [1] 설계 개념 : 절차적
                        - function driven
                        - top down
                    [2] 캡슐화 : data와 function 분리
                    [3] 정보 은닉 : x
                    [4] 오버로딩 : x
                    [5] 상속 : x
                    [6] 키워드 수 : 32
                    [7] 이름 공간 : x
                    [8] 메모리 할당 : malloc(), free()
                    [9] 참조 변수 : x
                    [10] 예외 처리 : x
                    [11] 문자, 불 자료형 : x
                    [12] 매개변수의 기본값 : x
                    [13] 인라인 함수 : x
                2] C++
                    [1] 설계 개념 : 객체지향적
                        - object driven
                        - bottom up
                    [2] 캡슐화 : data와 function 통합
                    [3] 정보 은닉 : 지원
                    [4] 오버로딩 : function, operator
                    [5] 상속 : Class
                    [6] 키워드 수 : 52
                    [7] 이름 공간 : 지원
                    [8] 메모리 할당 : new, delete operator
                    [9] 참조 변수 : 참조자 &
                    [10] 예외 처리 : try ~ catch 블록
                    [11] 문자, 불 자료형 : Bool, String
                    [12] 매개변수의 기본값 : 지원
                    [13] 인라인 함수 : 지원
        3) 객체지향 OPP
            (1) 개념
                1] 클래스 : 설계도
                    - 만드는 기준o. 실체x
                    ex. 자동차 설계도
                2] 객체 : 설계도로 만들어진 상품
                    - 실체o
                    ex. 자동차
            (2) 특징
                1] 추상화(Abstraction)
                    - 단순 >> 구체화
                    ex. 동물 -> 고양이 -> 사향 고양이
                2] 캡슐화(Encapsulation)
                    - 클래스 : 데이터와 함수(메서드)를 한 통에 넣는다
                3] 상속(Inheritance)
                    - 기본 클래스 상속 + 분화
                    - 효율성 증가
                4] 다형성(Polymorphism)
                    - 오버로딩 : 같은 이름의 함수나 연산자로도 다양한 기능
                5] 정보 은닉, 보안
            (3) 사용
                1] 객체 선언 : Class_name Object_name(var1, var2, ...)
                2] 멤버함수 호출
                    - Object_name.member_name(var1, ...)
                    - Object_name.member_name
                3] 접근 한정자 : 객체 내 멤버들에 대한 접근 제한
                    [1] public : 외부, 자식, 자신 모두 접근 가능
                    [2] protected : 자식, 자신만 접근 가능
                    [3] private : 자신만 접근 가능


● DigitalOut
    1. DigitalOut Class
        1) 생성자 : DigitalOut 객체명(Pin_name)
        2) 쓰기
            (1) 메소드 : 객체명.write(0 or 1)
            (2) 함수 오버로딩 : 객체명 = 0 or 1
        2) 읽기
            (1) 메소드 : int v = 객체명.read()
            (2) 연산자 오버로딩 : int v = 객체명
    
    +a) 
        0) wait
            (1) 대기
                1] sec 대기 : wait(sec) // 경고 발생하므로, 안정적인 us 사용
                2] ms 대기 : wait_ms(ms) // 경고 발생하므로, 안정적인 us 사용
                3] us 대기 : wait_us(us)
            
        0) rand
            (1) 사용
                1] 난수 seed 설정 : srand(time(NULL))
                    - time : 1970.1.1부터 지금까지 초수 반환
                2] 0 ~ RAND_MAX 난수 발생 : rand()
            
        1) 정수 자료형
            (1) 특징
                - 플랫폼간 호환
                - 바이트 크기 표현 명확, 간결
            (2) 종류
                1] int#_t
                    [1] char(1byte) = int8_t
                    [2] short int(2byte) = int16_t
                    [3] int(4byte) = int32_t
                    [4] long(4byte) = int32_t
                    [5] long long(8byte) = int64_t
                2] uint#_t
                    [1] char(1byte) = uint8_t
                    [2] short int(2byte) = uint16_t
                    [3] int(4byte) = uint32_t
                    [4] long(4byte) = uint32_t
                    [5] long long(8byte) = uint64_t
            
        2) DigitalOut.h 뜯어보기
            - 강의자료 참고 ♣
        
        3) HAL(Hardware Abstract Layer) : 추상화 계층
            (1) 특징
                - 하드웨어와 무관한 중립 추상 계층
                - 상위 레이어에서 레지스터 등 하드웨어의 구체적 사항 몰라도 사용 가능
                - 포함 관계 : Registers < HAL < Mbed

● DigitalIn
    1. DigitalIn Class
        1) 생성자 : DigitalIn 객체명(Pin_name)
        2) 입력모드 설정 : 객체명.mode(Pin_mode)
            - Pin_mode : PullUp / PullDown / PullNone
        3) 읽기 : 
            (1) 메소드 : int v = 객체명.read()
            (2) 연산자 오버로딩 : 특정 변수 = 객체명
            
    +a) 
        1) 스위치 회로
            (1) PullUp : 내부 회로 이용. 기본 High / 누르면 Low
            (2) PullDown : 내부 회로 이용. 기본 Low / 누르면 High
            (3) PullNone : 외부 회로 이용. 기본 Low / 누르면 High
            
        2) Mbed Serial 입출력
            (1) 종류
                1] 아두이노 시리얼 모니터
                2] Tera term
                3] Cool term
            (2) Serial 입출력
                1] 출력 : printf()
                2] 입력 : scanf()

        3) C++
            (1) C++ IDE
                1] Visual Studio
                    - 무거움
                    - 모든 것이 설치되어있음
                2] Visual Studio Code
                    - 가벼움
                    - 까다로운 설정
                3] Dev C++
                    - 가벼움. Simples
                    - 컴파일러 별도 설치

            (2) 참조자(reference variable)
                1] 개념 : 어떤 변수의 별명 만들기
                2] 사용 : 자료형& 별명 = 변수명
                3] 특징
                    [1] 원래 변수와 주소 동일
                    [2] 포인터 : 주소 저장 / 참조자 : 주소 동일
                    [3] 주의 사항
                        - 선언 시 초기화 필수
                        - NULL 참조자는 없다

● PwmOut
    0. Pwm 개요
        1) PWM(Pulse Width Modulation) : 펄스폭 변조
            - 일정한 주기의 펄스에서 펄스 폭 변경
            (1) 목적
                1] 정보 전달
                    - 정보 전달 by 일정 주기 펄스 폭 제공
                    ex. 서보모터
                2] 근사 아날로그 출력
                    - 근사 아날로그 출력 by 매우 짧은 주기
                    - 고가 DAC or 아날로그 회로 사용하지 않으므로 저렴함
                    ex. 모터 제어, 히터 제어, 조명 제어

    1. Pwm Class
        1) 생성자 : PwmOut 객체명(Pin_name)
        2) 주기 설정
            (1) 초 : 객체명.period(sec) // float sec
            (2) 밀리초 : 객체명.period_ms(ms) // int ms
            (3) 마이크로초 : 객체명.period_us(us) // int us
        2) 펄스폭 설정
            (1) 초 : 객체명.pulsewidth(sec) // float sec
            (2) 밀리초 : 객체명.pulsewidth_ms(ms) // int ms
            (3) 마이크로초 : 객체명.pulsewidth_us(us) // int us
        3) 쓰기
            (1) 메소드 : 객체명.write(value) // float value
            (2) 연산자 오버로딩 : 객체명 = 0.0 ~ 1.0
        4) 읽기
            (1) 메소드 : float v = 객체명.read()
            (2) 연산자 오버로딩 : float v = 객체명

    +a)
        1) RC 서보모터
            (1) 특징
                - 구성 : DC 모터, 감속기, POT
                - 각도 제어 feedback : 각도 명령 -> 제어기 -> 서보모터 -> 현재 각도 feedback
            (2) 명령 식
                수식 : pulse_width[ms] = (2.4-0.6)/180 * ang + 0.6
                - period : 10 ~ 20ms
                - 0도 : 펄스폭 600us
                - 180도 : 펄스폭 2400us
        2) map 함수 : 선형 mapping 사용자 정의 함수
            T map(T x, T in_min, T in_max, T out_min, T out_max)
        3) C++의 template
            1] 목적 : 클래스 or 함수의 매개변수 or 반환값이 여러개 일때
            2] 특징 : 자료형이 다른 여러 개 함수들을 따로 만들 필요 없이 하나의 틀로 만들기

            template <typename T> T sumTwo(T a, T b){
                return a + b;
            }
            float s1 = sumTwo<float>(3.2, 4.5);
            float s2 = sumTwo<int>(33,45);
            unsigned int s3 = sumTwo<unsigned int>(33, 45);

● AnalogIn
    0. ADC 개요
        1) ADC
            (1) ADC : 아날로그 입력 >> 디지털 데이터 변환
            (2) 샘플링 주기 : 아날로그 전압 읽는 주기
            (3) 양자화 : 샘플된 전압을 일시적으로 아날로그 전압을 유지 >> 유한한 비트 수의 디지털 데이터로 변환
        2) 분해능
            (1) 분해능 : 출력 데이터 1비트 변화시키는 입력 전압의 변화
            (1) 분해능 = V_span / (2^n)
                - V_span : 측정 스팬 전압
                - n : n비트로 변환
            (2) nucleo 보드 분해능
                - 12bit ADC : 0 ~ 3.3V 전압 >> 0 ~ 4095 수로 양자화
                - 분해능 : 1bit 변화 >> 3.3/4095 = 0.8mV 변화
    1. AnalogIn Class
        1) 생성자 : AnalogIn 객체명(Pin_name)
        2) 읽기
            1] 메소드 : float v = 객체명.read()
                - return : 아날로그 핀의 현재 ADC값 (0.0 ~ 1.0) // float
            2] 메소드 : uint16_t v = 객체명.read_u16()
                - return : 아날로그 핀의 현재 ADC값 (0 ~ 65535) // uint16_t
                - 분해능이 12bit but 16bit로 값을 읽음
            3] 연산자 오버로딩 : float v = 객체명
                - return : 아날로그 핀의 현재 ADC값 (0.0 ~ 65535) // float
    
    +a)
        1) 가변저항(POT)
            (1) 원리
                - 슬라이더
                - 저항체 트랙
                - 전극


● InterruptIn
    0. InterruptIn 개요
        0) Polling & Interrupt
            (0) 비유 : 영화 시청 중 중요 전화 받기
                main문 : 영화 감상
                Interrupt : 전화 통화
            (1) Polling : 영화 시청 중에 계속 전화기 확인
                영화를 제대로 보기 힘들다
            (2) Interrupt : 영화 편안하게 시청 >> 전화오면 일시정지 후 전화 받음 >> 통화를 마친 후 영화 재개
                영화를 제대로 볼 수 있다
            - ISR : Interrupt Service Routine
            - IRQ : Interrupt ReQuest
    1. Interrupt Class
        1) 생성자 : InterruptIn 객체명(Pin_name, Pin_mode)
            - Pin_name // PinName
            - Pin_mode : 입력핀 내부 Pull회로 지정 // Pinmode
                PullUp
                PullDown
                PullNone
                PullDefault
        2) 내부 Pull회로 지정 : 객체명.mode(Pin_mode)
            - Pin_mode : 입력핀 내부 Pull회로 지정 // Pinmode
                PullUp
                PullDown
                PullNone
                PullDefault
        3) 콜백 함수 지정
            (1) 일반함수 지정
                1] 상승 에지 : 객체명.rise(&func)
                    - &func : 상승 에지에서 호출될 반환형이 void인 함수 주소 // Callback<void()> func
                2] 하강 에지 : 객체명.fall(&func)
                    - &func : 하강 에지에서 호출될 반황형이 void인 함수 주소 // Callback<void()> func
            (2) 멤버함수 지정
                1] 상승 에지 : 객체명.rise(callback(obj, &func))
                    - obj : Interrupt 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 절대 주소 // M

                    class Counter(){
                        InterruptIn _but;
                        Counter(PinName p) _but(p);{
                            _but.rise(callback(this, &Counter::ISR));
                        }
                    }
                2] 하강 에지 : 객체명.fall(callback(obj, &func))
                    - obj : Interrupt 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 절대 주소 // M

                    class Counter(){
                        InterruptIn _but;
                        Counter(PinName p) _but(p);{
                            _but.fall(callback(this, &Counter::ISR));
                        }
                    }
        4) 읽기
            (1) 메소드 : int v = 객체명.read()
                - return : 입력핀의 현재값(0/1)
            (2) 연산자 오버로딩 : int v = 객체명
                - return : 입력핀의 현재값(0/1)
        5) Interrupt 활성화 / 비활성화
            (1) Interrupt 활성화 : 객체명.enable_irq() // void
            (2) Interrupt 비활성화 : 객체명.disable_irq() // void

    +a)
        1) 멤버 함수 콜백 지정
            Class A
                InterruptIn 멤버 객체 obj
                멤버함수 f2()
                    - 주소 : &A::f2

            Class A의 객체 a
                - 주소 : this

            일반함수 f1()
                - 주소 : &f1
        2) Callback, ISR 주의사항
            (1) 주의사항
                1] 속도
                    [1] 다른 큰 라이브러리 함수 호출 자제
                    [2] blocking code 사용 자제
                        - while문 이용한 대기
                        - wait() 함수 이용한 지연
                        - 기타 blocking 함수
                2] 꼬임
                    [1] 메모리 할당 malloc, new 사용 자제
                    [2] printf 사용 자제


● Ticker, Timer, TimeOut
    0. Timer 개요
        1) Ticker : 일정 주기 시간. callback 반복
        2) Timer : 임의 구간 시간
            - start : 기동
            - stop : 정지
            - reset : 0
            - read : 읽기
        3) TimeOut : 지정한 시간 후, 1회 callback 측정
    1. Timer Class
        1) 생성자 : Ticker 객체명
        2) 콜백 함수 지정
            (1) 일반 함수 지정
                1] 초 단위 주기 : 객체명.attach(&func, time)
                    - &func : 주기적으로 호출될 콜백 함수 절대 주소. 반환형이 void여야함 // Callback<void()>
                    - time : 콜백 함수 호출 주기(초단위) // float
                2] us 단위 주기 : 객체명.attach_us(&func, time_us)
                    - &func : 주기적으로 호출될 콜백 함수 절대 주소. 반환형이 void여야함 // Callback<void()>
                    - time_us : 콜백 함수 호출 주기(마이크로 초단위) // us_timestamp_t
            (2) 멤버 함수 지정
                1] 초 단위 주기 : 객체명.attach(Callback(obj, &func), time)
                    - obj : Ticker 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 절대 주소 // M
                    - time : 콜백 함수 호출 주기(마이크로 초단위) // us_timestamp_t

                    class myClass{
                        Ticker tic;
                        myClass(){
                            tic.attach(callback(this, &myClass::ISR), time)
                        }
                    }
                2] us 단위 주기 : 객체명.attach(Callback(obj, &func), time_us)
                    - obj : Ticker 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 절대 주소 // M
                    - time_us : 콜백 함수 호출 주기(마이크로 초단위) // us_timestamp_t

                    class myClass{
                        Ticker tic;
                        myClass(){
                            tic.attach_us(callback(this, &myClass::ISR), time_us)
                        }
                    }
        3) 콜백 함수 지정 해제 : 객체명.detach()


● UART
    0. UART 통신 개요
        1) 특징
            - 양방향 통신
            - 비동기(Asynchronous) 통신 : 동기 신호x. Baud Rate 의존
        2) UART 전송 프레임 정의 : 보드 간 UART 설정 같아야한다
            (1) BaudRate : 1초당 통신 bit 수(bps)
                - 2400의 배수
            (2) 비트 폭 : 1/(BaudRate)
            (3) Start bit : 시작 표시 (필수 포함)
            (4) 데이터 비트수 : 데이터 구성 비트 수
            (5) Parity bit : 데이터 오류 판별
                - 짝수모드이면 패리티 비트를 포함한 데이터 비트들에서 1인 비트 수가 짝수가 되도록 패리티 비트를 맞춤
                    짝수모드인데 데이터 비트 1개 손실되서 홀수가 나오면, 데이터가 손실된 것
                1] 홀수(ODD)
                2] 짝수(EVEN)
                3] 없음(NO)
            (6) Stop bit : 프레임 끝 표시

            ex. 문자 : 'J' 
            -> ASCII Hex : 0x4A
            -> ASCII BIN : 01001010

    1. Serial Class
        1) 생성자
            (1) Serial 생성 : Serial 객체명(Tx_Pin_name, Rx_Pin_name, baud rate) // RawSerial로 안하니까 오류 뜨는데??? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
                - Tx_Pin_name : Tx 핀 이름 // PinName
                - Rx_Pin_name : Rx 핀 이름 // PinName
                - buad rate : 통신 속도 // int
                    default : 9600
        2) Serial 설정
            (1) 통신 속도 : 객체명.baud(buad rate) // void
                - buad rate : 통신 속도 // int
            (2) 전송 프레임 : 객체명.format(bits, SerialBase::prty, stopb)
                - bits : 데이터 비트 수 // int
                    default : 8
                    데이터 비트수 설정
                - prty : 패리티 비트 설정 // Parity
                    default : None
                    없음 : None
                    홀수 : Odd
                    짝수 : Even
                - stopb : 정지 비트 수 // int
                    default : 1
                    1비트 : 1
                    2비트 : 2
        3) 콜백 함수 지정
            (1) 일반 함수 지정 : 객체명.attach(&func, type) // void
                - &func : 인터럽트 발생 시 호출될 함수 절대 주소 // Callback<void()>
                - type : 인터럽트 종류 // IrqType
                    default : RxIrq
                    수신 : RxIrq
                    발신 : TxIrq
            (2) 멤버 함수 지정 : 객체명.attach(Callback(obj, &func), type) // void
                - obj : Serial 객체가 포함된 객체의 주소
                - &func : 인터럽트 발생 시 호출될 함수 절대 주소
                - type : 인터럽트 종류 // IrqType
                    default : RxIrq
                    수신 : RxIrq
                    발신 : TxIrq

                    class myClass{
                        Serial pc; // 가계약
                        myClass(PinName tx, PinName rx): pc(tx, rx){
                            pc.attach(Callback(this, &myClass::ISR), type)
                        }
                    }
        4) 상태 확인
            (1) 쓰기 가능 : 객체명.writable() // bool
                - return : 통신 버퍼에 여유 있으면 true, 아니면 false

                if(pc.writable())
                    pc.putc('\n');
            (2) 읽기 가능 : 객체명.readable() // bool
                - return : 수신 버퍼에 데이터 있으면 true, 아니면 false
                - if(객체명.readable == true)를 사용하지않으면 getc 사용시 수신버퍼가 찰때까지 계속 기다린다 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
                
                if(pc.readable())
                    char c = pc.getc();s
        5) 쓰기 & 읽기
            (1) 쓰기 : 객체명.putc(c) // int
                - c : 전송할 1바이트 데이터 // int
                - return : 성공시 c, 실패시 -1
            (1) 쓰기 : 객체명.printf(~~~)
            (2) 읽기 : 객체명.getc() // int
                - return : 수신할 1바이트 데이터
            (2) 읽기 : 객체명.scanf(~~~)
    
    +a)
        1) RawSerial // ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
            - Serial : 오류 발생o
                printf 정의o
                scanf 정의o
            - RawSerial : 오류 발생x
                printf 정의x
                scanf 정의x
        2) volatile 자료형
            - compiler는 코드 optimizer를 위해 interrupt나 callback함수와 같이 뜬금없이 튀어오르는 값을
                없애는 경우가 많으므로, 휘발성 자료형이라고 compiler에게 미리 알려줌
        3) static 자료형
            - 함수 내의 변수는 나갈때 사라지는데, 정적 선언을 위해 코드 실행하는 동안 제거x
        4) atof(alphabet to float) : atof(배열)


● RTOS
    0. RTOS 개요
        1) RTOS(Real Time OS) : 작업 실시간 운영 지원
        2) RTOS APIs
            (1) Thread : 병렬 실행 작업 제어
            (2) Mutex(mutual exclusive = 상호 배제) : thread간 공유 데이터 동시 접근 관리
            (2) Semaphore : 공유하는 자료 접근 관리
            (3) Queue : 자료가 아닌 주소로 관리
                - FIFO(선입선출)
                - thread가 시간에 따라 생산한 자료를 다른 thread가 사용할 수 있도록 만듦
            (3) MemoryPool : 지정한 크기 자료의 메모리 풀로 관리 class
                - 자료를 실제 메모리에 할당
            (3) Mail : Queue + MemoryPool 기능 내장 class
            (4) EventFlags : 조건과 사건을 여러 thread에서 공유하게 하는 class
            (4) Event : 사건들을 queuing해서 이후에 처리할 수 있도록 하는 class
            
    1. Thread Class
        1) 생성자
            (1) Thread 생성 (자동 실행x) : Thread 객체명(pri, stk_sz, stk_mem, thr_name)
                - pri : 우선순위
                    default : osPriorityNormal
                    1]] osPriorityIdle : value = -3
                    2]] osPriorityLow : value = -2
                    3]] osPriorityBelowNormal : value = -1
                    4]] osPriorityNormal : value = 0
                    5]] osPriorityAboveNormal : value = 1
                    6]] osPriorityHigh : value = 2
                    7]] osPriorityRealtime : value = 3
                - stk_sz : 스택 사이즈
                    default : OS_STACK_SIZE
                - stk_mem : 스택 메모리의 포인터
                    default : NULL
                - name : 스레드의 이름
                    default : NULL
            (2) Thread 생성 (자동 실행o) : Thread 객체명(&task, pri, stk_sz, stk_mem, thr_name)
                - &task : 실행시키고자 하는 함수 절대 주소
                - pri : 우선순위
                    default : osPriorityNormal
                    1]] osPriorityIdle : value = -3
                    2]] osPriorityLow : value = -2
                    3]] osPriorityBelowNormal : value = -1
                    4]] osPriorityNormal : value = 0
                    5]] osPriorityAboveNormal : value = 1
                    6]] osPriorityHigh : value = 2
                    7]] osPriorityRealtime : value = 3
                - stk_sz : 스택 사이즈
                    default : OS_STACK_SIZE
                - stk_mem : 스택 메모리의 포인터
                    default : NULL
                - name : 스레드의 이름
                    default : NULL

        2) Thread 제어
            (1) Thread start : 객체명.start(&task)
                - &task : 실행시키고자 하는 함수 절대 주소
            (1) Thread start : 객체명.start(Callback(this, &myClass::task))
                - this : 객체가 생성된 주소
                - myClass : 객체가 있는 class 주소
                - task : Thread로 시작할 함수
            (2) Thread join : 객체명.join()
                - sub Thread 종료까지 main Thread가 종료하지 않고 기다림
                - return : Thread 명령 실행 상태
            (3) Thread terminate : 객체명.terminate()
                - return : Thread 명령 실행 상태
            (4) Thread 우선순위 설정 : 객체명.set_priority(pri)
                - pri : 우선순위
            (5) Thread 우선순위 읽기 : 객체명.get_priority()
                - return : Thread 우선순위

    +a) 
        1) 열거형 osStatus 구조체
            1]] enum osStatus : osOK / value : 0 / No Error
            2]] enum osStatus : osEventMessage / value : 0x10 / Message Event
            3]] enum osStatus : osEventMail / value : 0x20 / Mail Event
            4]] enum osStatus : osEventTimeout / value : 0x40 / 대기시간 초과
            5]] enum osStatus : osErrorParameter / value : 0x80 / 필수 매개변수 누락 or 잘못됨
            6]] enum osStatus : osErrorResource / value : 0x81 / 저장한 리소스가 유효하지 않음
            7]] enum osStatus : osErrorISR / value : 0x82 / 저장한 ISR 함수 호출 불가
            8]] enum osStatus : osErrorPriority / value : 0x84 / 우선순위 선정 불가 or 비정상
            9]] enum osStatus : osErrorNoMemory / value : 0x85 / 메모리 할당 불가
            10]] enum osStatus : osErrorValue / value : 0x86 / value 매개변수가 범위 초과
            11]] enum osStatus : osErrorOS / value : 0xFF / RTOS에서 미지의 오류 발생

        2) mbed-OS
            - 항상 최신 상태 유지
                mbed-os 폴더 우클릭 - update 클릭

    1. Mutex Class
        0) mutex 개념 : 2개의 Thread가 1개 자원 공유할때, 동시 접근으로 발생하는 문제 해결
            ex. 식당 화장실 열쇠
                - 열쇠가 하나라 열쇠가 돌아올때까지 대기. 사용했으면 바로 반환 
        1) 생성자
            (1) Mutex 생성 : Mutex 객체명
        2) 자료 접근 권한 제어
            (1) 권한 획득 : 객체명.lock()
                - return : 성공시 osOK
                - lock이 풀릴때까지 blocking(대기해야 함)
            (2) 권한 반환 : 객체명.unlock()
                - return : 성공시 osOK
            (3) 권한 획득 시도 for ms : 객체명.trylock_for(ms)
                - ms : ms초 동안 권한 획득 시도
                - return : 성공시 true, 실패시 false

    1. Queue Class
        1) 생성자
            (1) Queue 생성 : Queue<T, queue_sz> 객체명 //template
                - T : 메시지 자료형 in Queue // typename
                - queue_sz : 메시지 최대 개수 in Queue // uint32_t
        2) Queue 제어
            (1) Enqueue : 객체명.put(&msg, ms, pri) // osStatus
                - msg : 메시지의 포인터 in Queue // T
                - ms : 대기 timeout 시간 // uint32_t
                - pri : 우선 순위 // uint8_t
                    default : osPriorityNormal
                - return : 성공시 osOK
            (2) Dequeue : 객체명.get(ms) // osEvent
                - ms : 대기 시간
                    default : osWaitForever
                - return : 메시지가 포함된 osEvent 구조체
            (3) 상태 확인
                1] Empty? : 객체명.empty() // bool
                    - return : 비었으면 true, 아니면 false
                2] Full? : 객체명.full() // bool
                    - return : 찼으면 true, 아니면 false

            - Thread가 Queue를 통해 데이터 제때 읽지 못할 가능성
                >> Overflow로 데이터 손실 가능성
                >> Queue의 크기 적당히 크게 잡기
                
    +a)
        1) MemoryPool, Queue 예시
            (1) 구조체 생성
                typedef struct {
                    osStatus status;
                    union{
                        uint32_t v;
                        void *p;
                        int32_t signals;
                    } value;
                    union{
                        osMailQId mail_id;
                        osMessageQId message_id;
                    } def;
                } osEvent;

        2) MemoryPool, Queue 동작 원리
            (1) MemoryPool : 실제 메시지가 저장된 메모리
            (2) Queue : 주소 관리


● Encoder Sensor Project
    0. Encoder Sensor 개요
        1) Encoder 원리
            (1) Single track encoder : 한방향 속도
            (2) Multi track encoder : 양방향 속도

        2) Encoder 분류법
            (1) 측정값
                1] 선형(Linear) : 직선 변위 측정
                2] 회전(Rotary) : 회전 변위(각도) 측정
            (2) 출력 기준
                1] 절대식(Absolute) : 절대 위치
                    - 특정 위치 0도 기준, 펄스 위치 측정
                2] 증분식(Incremental) : 초기 위치에 대한 상대 위치
                    - 특정 위치가 0도 기준x, 지나간 펄스 개수만 측정
            (3) 측정 원리 of 증분식 엔코더
                1] 광학식(Optical) : 빛
                    - 휠 후면 : LED 2개
                    - 휠 전면 : 광센서. 위상차 90도 정도의 각도와 위치. 2개

                    [1] A, B상 위상차 : 방향 파악
                    [2] 펄스 개수 : 각도 파악
                2] 자기식(Magnetic) : 자기력
                    - 회전축 : 영구자석
                    - 보드 : Hall센서 120도 각도. 2개

                    [1] A, B상 위상차 : 방향 파악
                    [2] 펄스 개수 : 각도 파악

                    [1] Hall 센서
                        1]] 특징
                            - 싼 가격
                            - 튼튼
                            - 2개 센서 90도 위상차 : 방향 파악
                            - 펄스 개수 : 각도 파악
                        2]] 작동 원리
                            일정한 직류전류 to 금속판 >> 면에 수직한 자기력선 변화 줘봐 >> 전자의 진행 경로 바뀜
                            >> 전류 방향에 수직한 금속판 양단에 전위차 V_h 발생 (V_h ∝ B)
            (4) 출력 형태
                1] Push-Pull : 0V or 5V
                2] Open Collector : 사용자 원하는 전압 Pull up
                3] Line Drive : High는 5V, Low는 플로팅
                
        3) Encoder Pulse 해독 원리
            (1) 정방향
                A상 에지 상승 : 00 -> 10
                B상 에지 상승 : 10 -> 11
                A상 에지 하강 : 11 -> 01
                B상 에지 하강 : 01 -> 00
            (2) 역방향
                B상 에지 상승 : 00 -> 01
                A상 에지 상승 : 01 -> 11
                B상 에지 하강 : 11 -> 10
                A상 에지 하강 : 10 -> 00

            (3) 상태도
                정방향 : 00 -> 10 -> 11 -> 01 -> Circular 반복
                역방향 : 00 -> 01 -> 11 -> 10 -> Circular 반복
                에러 : 10 -> 01 or 01 -> 10 or 00 -> 11 or 11 -> 00

                - 원 : 상태
                - 화살표 : 상태 천이
                - 사각형 : 해야할 일
            
    1. Thread Class
        1) 생성자
            (1) Thread 생성 (자동 실행x) : Thread 객체명(pri, stk_sz, stk_mem, thr_name)
                - pri : 우선순위
                    default : osPriorityNormal
                    1]] osPriorityIdle : value = -3
                    2]] osPriorityLow : value = -2
                    3]] osPriorityBelowNormal : value = -1
                    4]] osPriorityNormal : value = 0
                    5]] osPriorityAboveNormal : value = 1
                    6]] osPriorityHigh : value = 2
                    7]] osPriorityRealtime : value = 3
                - stk_sz : 스택 사이즈
                    default : OS_STACK_SIZE
                - stk_mem : 스택 메모리의 포인터
                    default : NULL
                - name : 스레드의 이름
                    default : NULL

*/