/*
● Microcontroller
    1. 프로세서
        (0) 개별 논리소자
            - 디지털 논리회로 구성 by 개별 논리소자
            ex. TTL, CMOS ♣♣♣♣♣♣
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
                [0] 개념 : 단일 IC 집적 = MPU + Memory + GPIO + Timer/Counter + Communications ♣♣♣
                [1] 하나의 작은 컴퓨터 ♣♣♣
                [2] 저전력 / 빠른 개발 ♣♣♣
                [3] MPU에 비해 소형화 / 저렴한 시스템 구성 ♣♣♣
                [4] 활용 : 간단한 장치 or 제어기 구축 ♣♣♣
            2] 구조
                // 강의자료 참조 ♣♣♣♣♣♣
            3] 예시 ♣♣♣♣♣♣
                - Intel : 8051
                - Microchips : PIC
                - ATmel : AVR
		(3) SOC(System On Chip)
			1] 특징 : 하나의 칩 안에 프로세서와 주변 장치 모두 포함. MCU보다 대규모의 시스템 의미
        (4) DSP = Digital Signal Processor ♣♣♣♣♣♣
            1] 특징
                [1] 개념 : 특수 MPU = MPU + 디지털 신호처리에 적합한 연산기능
                [2] 활용 : Audio, Video 등 처리
    2. ARM
        1) 특징
            (1) 원래 Acorn computers라는 교육용 컴퓨터 회사 ♣♣♣♣♣♣
            (1) ARM(Advanced RISC Machines) Fabless 반도체 회사
            (2) RISC 프로세서 : 임베디드 특화
            (3) 저전력 / 고성능 mobile 프로세서 절대 강자
            (4) 세계 대기업들의 ARM 코어 사용
        2) 종류
            (1) Classic : 예전 모델 ♣♣♣
            (2) Embedded(M) : MCU
                ex. 드론, 세탁기
            (3) Real-time(R) : Real time system
                ex. 자동차, 로봇팔
            (4) Application(A) : 고기능, 고성능 운영체제
                ex. 고성능 휴대폰, 노트북
    3. 프로그래밍
        1) MPU 프로그래밍
            (1) 과거 : 어셈블리어
                - 기계에게 효율적 but 프로그래머에게 비효율적
            (2) 과도기 : C(고급 언어) ♣♣♣
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
        1) RISC(Reduced Instruction Set Computer) 프로세서 ♣♣♣♣♣♣
            (1) 특징
                1] 명령어 단순화 / 개수 최소화
                2] 장점 ♣♣♣
                    - 효율적 하드웨어 사용
                    - 병렬 수행 like 파이프라인
                3] 단점 ♣♣♣
                    - 컴파일러 코드 최적화 복잡
                    - 프로그램 길어짐
        2) CISC(Complex Instruction Set Computer) 프로세서

● Mbed
    1. Mbed
        1) 개념 : Cortex-M 마이크로컨트롤러 프로그램용 OS ♣♣♣
            - 온라인 소프트웨어 개발 툴 + USB 메모리 & 전원
        2) 특징
            (1) 장점
                1] 개발 난이도
                    - 개발 환경 구축 신속 ♣♣♣
                    - Cortex-M 칩 : 브랜드, 부품 번호 변경에 대한 부담x ♣♣♣
                    - 오픈 소스 : 방대한 C++ 기반 Mbed 라이브러리 in 온라인
                    - 레지스터 레벨 고려x : 복잡한 레지스터 고려x
                    - Mbed API 일관성 : 몇몇 API 기능 이해만으로 나머지 학습 용이
                2] 성능
                    - 낮은 가격 : 8비트 수준 가격으로 32비트 고성능 시스템 구축
                    - IOT 지향 : 연결성(Ethernet, USB, Bluetooth, CAN 등) ♣♣♣
                    - RTOS 지원 : 실시간 프로그래밍
            (1) 장점
                1] 사용성 편리
                2] 회로 몰라도 개발 가능 ♣♣♣
                3] 별도 다운로드 장비 불필요 ♣♣♣
                4] 방대한 라이브러리 in 온라인
                5] 고성능 ARM 코어의 다양한 기능(RTOS, 연결성 등)
                    - Ethernet, USBHost, USBDevice, SPI, I2C, CAN, AnalogIn, PwmOut, AnalogOut, ...
                6] 많은 보드, 모듈, 부품 지원
            (2) 단점 ♣♣♣
                1] 레지스터 레벨 디버깅 불가
        3) Nucleo Board
            (1) 특징
                1] 제조사 : STMicroelectronics
                2] 개발보드 : 수많은 lineup 존재
                    - Cortex-M
        4) Nucleo-f401re datasheet ♣♣♣♣♣♣
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
        5) Mbed API ♣♣♣
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
        3) Compile ♣♣♣♣♣♣
        4) 보드 Sub 연결 ♣♣♣♣♣♣
        5) bin 파일 drag & drop

    +a)
        1) 유용한 기능 : 키워드 클릭 시 클래스 가이드 표시 ♣♣♣
            - 정의 위치, 관련 문서 등 확인 가능
        2) C vs C++
            (1) 특징 ♣♣♣
                - C ⊂ C++
                1] C
                    [1] 설계 개념 : 절차적 ♣♣♣♣♣♣
                        - function driven
                        - top down
                    [2] 캡슐화 : data와 function 분리 ♣♣♣
                    [3] 정보 은닉 : x
                    [4] 오버로딩 : x
                    [5] 상속 : x
                    [6] 키워드 수 : 32 ♣♣♣
                    [7] 이름 공간 : x ♣♣♣
                    [8] 메모리 할당 : malloc(), free() ♣♣♣
                    [9] 참조 변수 : x ♣♣♣
                    [10] 예외 처리 : x ♣♣♣
                    [11] 문자, 불 자료형 : x ♣♣♣
                    [12] 매개변수의 기본값 : x ♣♣♣
                    [13] 인라인 함수 : x ♣♣♣
                2] C++
                    [1] 설계 개념 : 객체지향적 ♣♣♣♣♣♣
                        - object driven
                        - bottom up
                    [2] 캡슐화 : data와 function 통합 ♣♣♣
                    [3] 정보 은닉 : 지원
                    [4] 오버로딩 : function, operator ♣♣♣
                    [5] 상속 : Class
                    [6] 키워드 수 : 52 ♣♣♣
                    [7] 이름 공간 : 지원 ♣♣♣
                    [8] 메모리 할당 : new, delete operator ♣♣♣
                    [9] 참조 변수 : 참조자 & ♣♣♣
                    [10] 예외 처리 : try ~ catch 블록 ♣♣♣
                    [11] 문자, 불 자료형 : Bool, String ♣♣♣
                    [12] 매개변수의 기본값 : 지원 ♣♣♣
                    [13] 인라인 함수 : 지원 ♣♣♣
        3) 객체지향 OPP
            (1) 개념
                1] 클래스 : 설계도
                    - 만드는 기준o. 실체x
                    ex. 자동차 설계도
                2] 객체 : 설계도로 만들어진 상품
                    - 실체o
                    ex. 자동차
            (2) 특징 ♣♣♣
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
            (3) 사용 ♣♣♣
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
        1) 생성자 : DigitalOut 객체명(Pin_name[, value])
            - Pin_name // PinName
            - value : 초기 출력값 (0 or 1) // int ♣♣♣
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
            (1) 사용 ♣♣♣
                1] 난수 seed 설정 : srand(time(NULL))
                    - time : 1970.1.1부터 지금까지 초수 반환
                2] 0 ~ RAND_MAX 난수 발생 : rand()
            
        1) 정수 자료형
            (1) 특징
                - 플랫폼간 호환
                - 바이트 크기 표현 명확, 간결
            (2) 종류
                1] int#_t
                    [1] char(1byte) = int8_t ♣♣♣
                    [2] short int(2byte) = int16_t ♣♣♣
                    [3] int(4byte) = int32_t
                    [4] long(4byte) = int32_t
                    [5] long long(8byte) = int64_t
                2] uint#_t
                    [1] char(1byte) = uint8_t ♣♣♣
                    [2] short int(2byte) = uint16_t ♣♣♣
                    [3] int(4byte) = uint32_t
                    [4] long(4byte) = uint32_t
                    [5] long long(8byte) = uint64_t
            
        2) DigitalOut.h 뜯어보기
            - 강의자료 참고 ♣♣♣♣♣♣
        
        3) HAL(Hardware Abstract Layer) : 추상화 계층 ♣♣♣
            (1) 특징
                - 하드웨어와 무관한 중립 추상 계층
                - 상위 레이어에서 레지스터 등 하드웨어의 구체적 사항 몰라도 사용 가능
                - 포함 관계 : Registers ⊂ HAL ⊂ Mbed


● DigitalIn
    1. DigitalIn Class
        1) 생성자 : DigitalIn 객체명(Pin_name[, Pin_mode])
            - Pin_name // PinName
            - Pin_mode // PinMode ♣♣♣
                PullUp
                PullDown
                PullNone
                OpenDrain
        2) 입력모드 설정 : 객체명.mode(Pin_mode)
            - Pin_mode // PinMode ♣♣♣
                PullUp
                PullDown
                PullNone
                OpenDrain
        3) 읽기
            (1) 메소드 : int v = 객체명.read()
            (2) 연산자 오버로딩 : 특정 변수 = 객체명
            
    +a) 
        1) 스위치 회로
            (1) PullUp : 내부 회로 이용. 기본 High / 누르면 Low
            (2) PullDown : 내부 회로 이용. 기본 Low / 누르면 High
            (3) PullNone : 외부 회로 이용. 기본 Low / 누르면 High ♣♣♣
            
        2) Mbed Serial 입출력
            (1) 종류
                1] 아두이노 시리얼 모니터
                2] Tera term ♣♣♣
                3] Cool term
            (2) Serial 입출력
                1] 출력 : printf()
                2] 입력 : scanf()

        3) C++
            (1) C++ IDE
                1] Visual Studio
                    - 무거움
                    - 모든 것이 설치되어있음 ♣♣♣
                2] Visual Studio Code
                    - 가벼움
                    - 까다로운 설정 ♣♣♣
                3] Dev C++
                    - 가벼움. Simples
                    - 컴파일러 별도 설치 ♣♣♣

            (2) 참조자(reference variable) ♣♣♣♣♣♣ 강의자료 참조
                1] 개념 : 어떤 변수의 별명 만들기
                2] 사용 : 자료형& 별명 = 변수명
                3] 특징
                    [1] 원래 변수와 주소 동일
                    [2] 포인터 : 주소 저장 / 참조자 : 주소 동일
                    [3] 주의 사항
                        - 선언 시 초기화 필수
                        - NULL 참조자는 없다
                    [4] 활용 : 포인터처럼 함수 매개변수로 반환값 받을 때 사용 가능


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
        2) 주기 설정 ♣♣♣
            (1) 초 : 객체명.period(sec) // float sec
            (2) 밀리초 : 객체명.period_ms(ms) // int ms
            (3) 마이크로초 : 객체명.period_us(us) // int us
        2) 펄스폭 설정 ♣♣♣
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
        1) RC 서보모터 ♣♣♣
            (1) 특징
                - 구성 : DC 모터, 감속기, POT
                - 각도 제어 feedback : 각도 명령 -> 제어기 -> 서보모터 -> 현재 각도 feedback
            (2) 명령 식
                수식 : pulse_width[ms] = (2.4-0.6)/180 * ang + 0.6
                - period : 10 ~ 20ms
                - 0도 : 펄스폭 600us
                - 180도 : 펄스폭 2400us
        2) map 함수 : 선형 mapping 사용자 정의 함수 ♣♣♣
            T map(T x, T in_min, T in_max, T out_min, T out_max)
        3) C++의 template ♣♣♣
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
            (1) ADC : 아날로그 입력 -> 디지털 데이터 변환
            (2) 샘플링 주기 : 아날로그 전압 읽는 주기 ♣♣♣
            (3) 양자화 : 샘플된 전압을 일시적으로 아날로그 전압을 유지 -> 유한한 비트 수의 디지털 데이터로 변환 ♣♣♣
        2) 분해능
            (1) 분해능 : 출력 데이터 1비트 변화시키는 입력 전압의 변화 ♣♣♣
            (1) 분해능 = V_span / (2^n)
                - V_span : 측정 스팬 전압
                - n : n비트로 변환
            (2) nucleo 보드 분해능 ♣♣♣
                - 12bit ADC : 0 ~ 3.3V 전압 >> 0 ~ 4095 수로 양자화
                - 분해능 : 1bit 변화 >> 3.3/4095 = 0.8mV 변화
    1. AnalogIn Class
        1) 생성자 : AnalogIn 객체명(Pin_name)
        2) 읽기
            1] 메소드 : float v = 객체명.read()
                - return : 아날로그 핀의 현재 ADC값 (0.0 ~ 1.0) // float
            2] 메소드 : uint16_t v = 객체명.read_u16() ♣♣♣
                - return : 아날로그 핀의 현재 ADC값 (0 ~ 65535) // uint16_t
                - 분해능이 12bit but 16bit로 값을 읽음
            3] 연산자 오버로딩 : float v = 객체명
                - return : 아날로그 핀의 현재 ADC값 (0.0 ~ 65535) // float
    
    +a)
        1) 가변저항(POT) ♣♣♣
            (1) 원리
                - 슬라이더
                - 저항체 트랙
                - 전극
        2) DeadZone


● InterruptIn
    0. InterruptIn 개요
        0) Polling & Interrupt
            (0) 비유 : 영화 시청 중 중요 전화 받기
                main문 : 영화 감상
                Interrupt : 전화 통화
            (1) Polling : 영화 시청 중에 계속 전화기 확인 ♣♣♣
                - 영화를 제대로 보기 힘들다
            (2) Interrupt : 영화 편안하게 시청 >> 전화오면 일시정지 후 전화 받음 >> 통화를 마친 후 영화 재개
                - 영화를 제대로 볼 수 있다
                1] 외부 Interrupt ♣♣♣
                    [1] Interrupt 종류
                        1]] Rising Edge
                        2]] Falling Edge
                    [2] Interrupt 함수
                        1]] ISR
                        2]] callback 함수
            - ISR : Interrupt Service Routine ♣♣♣
            - IRQ : Interrupt ReQuest ♣♣♣
    1. Interrupt Class
        1) 생성자 : InterruptIn 객체명(Pin_name, Pin_mode) ♣♣♣ 함수 명령어
            - Pin_name // PinName
            - Pin_mode : 입력핀 내부 Pull회로 지정 // Pinmode ♣♣♣
                PullUp
                PullDown
                PullNone
                PullDefault
        2) 내부 Pull회로 지정 : 객체명.mode(Pin_mode) ♣♣♣ 함수 명령어
            - Pin_mode : 입력핀 내부 Pull회로 지정 // Pinmode
                PullUp
                PullDown
                PullNone
                PullDefault
        3) 콜백 함수 지정
            (1) 일반함수 지정
                1] 상승 에지 : 객체명.rise(&func) ♣♣♣ 함수 명령어
                    - &func : 상승 에지에서 호출될 반환형이 void인 함수 주소 // Callback<void()> func
                2] 하강 에지 : 객체명.fall(&func) ♣♣♣ 함수 명령어
                    - &func : 하강 에지에서 호출될 반환형이 void인 함수 주소 // Callback<void()> func
            (2) 멤버함수 지정
                1] 상승 에지 : 객체명.rise(callback(obj, &func)) ♣♣♣ 함수 명령어
                    - obj : Interrupt 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 절대 주소 // M

                    class Counter(){
                        InterruptIn _but;
                        Counter(PinName p) _but(p);{
                            _but.rise(callback(this, &Counter::ISR));
                        }
                    }
                2] 하강 에지 : 객체명.fall(callback(obj, &func)) ♣♣♣ 함수 명령어
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
        5) Interrupt 활성화 / 비활성화 ♣♣♣
            (1) Interrupt 활성화 : 객체명.enable_irq() // void
            (2) Interrupt 비활성화 : 객체명.disable_irq() // void

    +a)
        1) 멤버 함수 콜백 지정 ♣♣♣
            Class A
                InterruptIn 멤버_객체
                멤버함수 f2()
                    - 주소 : &A::f2

            Class A의_객체
                - 주소 : this

            일반함수 f1()
                - 주소 : &f1
        2) Callback, ISR 주의사항
            (1) 주의사항
                1] 속도
                    [1] 다른 큰 라이브러리 함수 호출 자제 ♣♣♣
                    [2] blocking code 사용 자제
                        - while문 이용한 대기
                        - wait() 함수 이용한 지연
                        - 기타 blocking 함수
                2] 꼬임 ♣♣♣
                    [1] 메모리 할당 malloc, new 사용 자제
                    [2] printf 사용 자제


● Ticker, Timer, TimeOut
    0. Timer 개요
        1) Ticker : 일정 주기 시간. callback 반복 (오래걸리는 블럭을 interrupt ticker나 delay로 처리하면 안된다) ♣♣♣
        2) Timer : 임의 구간 시간 (오래걸리는 블럭을 처리할때는 timer 사용, wait은 while문을 중단시키지만, timer는 while문을 돌린다) ♣♣♣
            - start : 재개 ♣♣♣
            - stop : 정지
            - reset : 초기화 후 재개 ♣♣♣
            - read : 읽기 ♣♣♣
        3) TimeOut : 지정한 시간 후, callback 1회만 실행
    1-1. Ticker Class
        1) 생성자 : Ticker 객체명
        2) 콜백 함수 지정
            (1) 일반 함수 지정
                1] 초 단위 주기 : 객체명.attach(&func, time) // void ♣♣♣ 함수 명령어
                    - &func : 주기적으로 호출될 콜백 함수 절대 주소. 반환형이 void여야함 // Callback<void()>
                    - time : 콜백 함수 호출 주기(초단위) // float
                2] us 단위 주기 : 객체명.attach_us(&func, time_us) // void ♣♣♣ 함수 명령어
                    - &func : 주기적으로 호출될 콜백 함수 절대 주소. 반환형이 void여야함 // Callback<void()>
                    - time_us : 콜백 함수 호출 주기(마이크로 초단위) // us_timestamp_t
            (2) 멤버 함수 지정
                1] 초 단위 주기 : 객체명.attach(Callback(obj, &func), time) // void ♣♣♣ 함수 명령어
                    - obj : Ticker 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 절대 주소 // M
                    - time : 콜백 함수 호출 주기(마이크로 초단위) // us_timestamp_t

                    class myClass{
                        Ticker tic;
                        myClass(){
                            tic.attach(callback(this, &myClass::ISR), time)
                        }
                    }
                2] us 단위 주기 : 객체명.attach_us(Callback(obj, &func), time_us) ♣♣♣ 함수 명령어
                    - obj : Ticker 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 절대 주소 // M
                    - time_us : 콜백 함수 호출 주기(마이크로 초단위) // us_timestamp_t

                    class myClass{
                        Ticker tic;
                        myClass(){
                            tic.attach_us(callback(this, &myClass::ISR), time_us)
                        }
                    }
        3) 콜백 함수 지정 해제 : 객체명.detach() ♣♣♣ 함수 명령어
    1-2. Timer Class
        1) 생성자 : Timer 객체명
        2) 동작
            (1) 측정 재개 : 객체명.start() ♣♣♣ 함수 명령어
            (2) 측정 정지 : 객체명.stop() ♣♣♣ 함수 명령어
            (3) 측정 초기화 후 재개 : 객체명.reset() ♣♣♣ 함수 명령어
        3) 읽기
            (1) 메소드
                1] 초 읽기 : float t = 객체명.read() ♣♣♣ 함수 명령어
                    - return : 현재 시간(초 단위) // float
                2] 밀리초 읽기 : int t = 객체명.read_ms() ♣♣♣ 함수 명령어
                    - return : 현재 시간(밀리초 단위) // int
                3] 마이크로초 읽기 : int t = 객체명.read_us() ♣♣♣ 함수 명령어
                    - return : 현재 시간(마이크로초 단위) // int
            (2) 오버로딩
                1] 초 읽기 : float t = 객체명 ♣♣♣ 함수 명령어
                    - return : 현재 시간(초 단위) // float
            (3) 부분 코드 실행
                - 원하는 블록 전 : start
                - 원하는 블록 후 : stop & read
    1-3. Timeout Class
        1) 생성자 : Timeout 객체명
        2) 콜백함수 지정
            (1) 일반 함수 지정
                1] 초 단위 지정 : 객체명.attach(&func, time) // void ♣♣♣ 함수 명령어
                    - &func : 주기적으로 호출될 콜백 함수 절대 주소. 반황형이 void여야함 // Callback<void()>
                    - time : 콜백 함수 호출될 때까지의 시간(초단위) // float
                2] us 단위 지정 : 객체명.attach_us(&func, time) // void ♣♣♣ 함수 명령어
                    - &func : 주기적으로 호출될 콜백 함수 절대 주소. 반환형이 void여야함 // Callback<void()>
                    - time : 콜백 함수 호출될 때까지의 시간(마이크로 초단위) // us_timestamp_t
            (2) 멤버 함수 지정
                1] 초 단위 지정 : 객체명.attach(Callback(obj, &func), time) // void ♣♣♣ 함수 명령어
                    - obj : Timeout 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 절대 주소 // M
                    - time : 콜백 함수 호출될 때까지의 시간(초단위) // us_timestamp_t

                    Class myClass{
                        Timeout tmo;
                        myClass(){
                            tmo.attach(callback(this, &myClass::ISR), time)
                        }
                    }
                2] us 단위 지정 : 객체명.attach_us(Callback(obj, &func), time_us) // void ♣♣♣ 함수 명령어
                    - obj : Timeout 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 절대 주소 // M
                    - time : 콜백 함수 호출될 때까지의 시간(마이크로 초단위) // us timestamp_t

                    Class myClass{
                        Timeout tmo;
                        myClass(){
                            tmo.attach(callback(this, &myClass::ISR), time_us)
                        }
                    }
        3) 콜백함수 지정 해제 : 객체명.detach() ♣♣♣ 함수 명령어
    3. Debouncing
        0) Debouncing 개요
            - 스위치 : 탄성력이 접점에 작용
            - Bouncing 현상 : 스위치 버튼 누를 때, 뗄 때 일정 시간동안 Bouncing 현상 발생
            

● UART
    0. UART 통신 개요
        1) 특징
            - 양방향 통신 ♣♣♣
            - 비동기(Asynchronous) 통신 : 동기 신호x. Baud Rate 의존
        2) UART 전송 프레임 정의 : 보드 간 UART 설정 같아야한다
            (1) BaudRate : 1초당 통신 bit 수(bps)
                - 2400의 배수
            (2) 비트 폭 : 1/BaudRate ♣♣♣
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
            (1) Serial 생성 : Serial 객체명(Tx_Pin_name, Rx_Pin_name, baud rate) // RawSerial로 안하니까 오류 뜨는데??? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣  ♣♣♣ 명령어
                - Tx_Pin_name : Tx 핀 이름 // PinName
                - Rx_Pin_name : Rx 핀 이름 // PinName
                - buad rate : 통신 속도 // int
                    default : 9600
        2) Serial 설정 ♣♣♣
            (1) 통신 속도 : 객체명.baud(buad rate) // void
                - buad rate : 통신 속도 // int
            (2) 전송 프레임 : 객체명.format(bits, SerialBase::prty, stopb)
                - bits : 데이터 비트 수 // int
                    default : 8
                    데이터 비트수 설정
                - prty : 패리티 비트 설정 // Parity
                    default : None
                    없음 : None
                    홀수 : Odd ♣♣♣
                    짝수 : Even ♣♣♣
                - stopb : 정지 비트 수 // int ♣♣♣
                    default : 1
                    1비트 : 1
                    2비트 : 2
        3) 콜백 함수 지정
            (1) 일반 함수 지정 : 객체명.attach(&func, type) // void ♣♣♣
                - &func : 인터럽트 발생 시 호출될 함수 절대 주소 // Callback<void()>
                - type : 인터럽트 종류 // IrqType ♣♣♣
                    default : RxIrq
                    수신 : RxIrq
                    발신 : TxIrq
            (2) 멤버 함수 지정 : 객체명.attach(Callback(obj, &func), type) // void ♣♣♣
                - obj : Serial 객체가 포함된 객체의 주소
                - &func : 인터럽트 발생 시 호출될 함수 절대 주소
                - type : 인터럽트 종류 // IrqType ♣♣♣
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
                - return : 통신 버퍼에 여유 있으면 true, 아니면 false ♣♣♣

                if(pc.writable())
                    pc.putc('\n');
            (2) 읽기 가능 : 객체명.readable() // bool
                - return : 수신 버퍼에 데이터 있으면 true, 아니면 false ♣♣♣
                - if(객체명.readable == true)를 사용하지않으면 getc 사용시 수신버퍼가 찰때까지 계속 기다린다 ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
                
                if(pc.readable())
                    char c = pc.getc();
        5) 쓰기 & 읽기
            (1) 쓰기 : 객체명.putc(c) // int
                - c : 전송할 1바이트 데이터 // int
                - return : 성공시 c, 실패시 -1 ♣♣♣
            (1) 쓰기 : 객체명.printf(~~~) ♣♣♣
            (2) 읽기 : 객체명.getc() // int
                - return : 수신할 1바이트 데이터 ♣♣♣
            (2) 읽기 : 객체명.scanf(~~~) ♣♣♣
    
    +a)
        1) RawSerial ♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣
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


● I2C
    0. I2C 개요
        1) 특징
            (1) 회로
                1] 2개 선(SDA, SCL)
                    - SDA, SCL line : 2kΩ ~ 10kΩ로 PullUp
                        SDA : Serial Data
                        SCL : Serial Clock
                    - master, slave 모두 open drain 회로로 0으로 만들 수 있음 ♣♣♣
            (2) 통신 방식
                1] master-slave
                    - master가 각 slave에 고유 주소 부여
                    - 2개 선으로 복수개 장치와 통신 가능
                2] 동기 통신 : clock 신호에 동기
                3] 반이중 양방향 통신
                    - 보내기, 받기 동시 불가 but 각각 따로는 가능
                    - 1:N 반이중 양방향 통신
                    ex. 무전기
        2) 작동 원리
            (0) Basic
                1] clock 신호 : master만 출력 가능
                2] SDA : SCL이 low인 시점만 변경 가능
                    - 예외 : 시작 or 정지 조건
                3] 데이터 전송 : 항상 2개 데이터 전송
                    [1] master 쓰기
                        주소 : master -> slave
                        데이터 : master -> slave
                    [2] master 읽기
                        주소 : master -> slave
                        데이터 : slave -> master
            (1) 전송 시작 or 정지
                1] 시작 조건
                    [1] 시작 조건 이전
                        - slave : 대기
                    [2] 시작 조건
                        - master : SCL를 high로 유지, SDA를 low로 내림
                    [3] 시작 조건 이후
                        - slave : 시작 조건 감지 -> address 읽기
                2] 데이터 변경 가능 시점
                    [1] 데이터 변경 가능 : SCL = low인 경우
                    [2] 데이터 변경 불가 : SCL = high인 경우
                3] 정지 조건
                    [1] 정지 조건
                        - master : SCL를 high로 유지, SDA를 high로 올림
            (2) 쓰기 Master -> Slave
                1] 시작 조건 생성 of master
                2] 주소/데이터 송신 of master
                    - address : A6~A0(7bit)
                    - R/W : 0(마지막 1bit)
                        R : high
                        W : low
                3] 주소/데이터 수신 여부 of slave
                    - 수신 성공 : ACK = 0
                    - 수신 실패 : NACK = 1
            (3) 읽기 Slave -> Master
                1] 시작 조건 생성 of master
                2] 주소 송신 of master
                    - address : A6~A0(7bit)
                    - R/W : 1(마지막 1bit)
                        R : high
                        W : low
                3] 주소 수신 여부 of slave
                    - 수신 성공 : ACK = 0
                    - 수신 실패 : NACK = 1
                4] 정지 조건 생성 of master
                    - 데이터 수신 완료 시, NACK의 의미로 SDA를 high(1)로 만듦 ♣♣♣
    1. I2C Class
        1) 생성자 : I2C 객체명(SDA_Pin_name, SCL_Pin_name) // I2C
            - SDA_Pin_name : I2C data 핀이름 // PinName
            - SCL_Pin_name : I2C clock 핀이름 // PinName
        2) 주파수 설정 : 객체명.frequency(hz) // void
            - hz : Hz단위의 I2C clock 주파수 // int
                default : 40kHz
        3) 쓰기 & 읽기
            (1) 여러 바이트 쓰기 : 객체명.write(address, data, length, repeat)
                - address : slave device의 I2C 주소 // int
                    쓰기 : 마지막 byte 1로 수동 지정
                - data : 전송할 char 배열 // const char*
                - length : 전송 byte 수 // int
                - repeat : 반복 여부 // bool
                    default : false
                    반복 x. Stop o : false
                    반복 o. Stop x : true
                - return : slave 수신 여부
                    성공(ACK) : 0
                    실패(NACK) : 1
            (2) 여러 바이트 읽기 : 객체명.read(address, data, length, repeat)
                - address : slave device의 I2C 주소 // int
                    읽기 : 마지막 byte 0으로 수동 지정
                - data : 수신 데이터 저장할 char 배열 // const char*
                - length : 수신 byte 수 // int
                - repeat : 반복 여부 // bool
                    default : false
                    반복 x. Stop o : false
                    반복 o. Stop x : true
                - return : slave 수신 여부
                    성공(ACK) : 0
                    실패(NACK) : 1
    2. I2C 모듈
        1) RTC 모듈 개요
            (1) 특징
                1] 용도 : 정확한 현재 시간이 필요한 경우
                    ex. 현재 시간에 따른 미세 설정 컨트롤
                    ex. 시간을 포함한 측정 데이터 전송
                2] 배터리
                    - 보존용 리튬 배터리 : 전원 제거 시 시간 유지
                    - 저전력 회로 : 몇년 동안 배터리 사용 가능
            (2) 제품
                1] 제품명 : PCF8563
                    - NXP사 real time clock
                2] 사용법
                    - 레지스터 table 참조
                        BCD 데이터 사용 in 레지스터
                        레지스터에 맞는 마스크 사용
                    - I2C 주소 : 0xA2
        2) OLED 모듈 개요
            (1) 특징
                1] 용도 : 디스플레이
                2] 구조 : 유기발광 다이오드의 매트릭스 구조
            (2) 제품
                1] 제품명 : SSD1306
                    - Size : 최대 128x64
                    - 통신 : I2C, SPI 지원
                    - 라이브러리 : 방대한 기능 지원하므로 직접 제작보단, 라이브러리 사용
                2] Adafruit_SSD1306_I2c Class
                    [0] 라이브러리 다운
                        import -> I2C_SSD1306_Helloworld
                    [1] 생성자 : Adafruit_SSD1306_I2c 객체명(i2c, rst_pin, i2c_adr, height, width)
                        - i2c : OLED 모듈 연결된 I2C 객체 // I2C
                        - rst_pin : mbed보드의 모듈 reset용 Pin // PinName
                        - i2c_adr : OLED 모듈 I2C 주소 // int
                        - height : 디스플레이 높이 // int
                        - width : 디스플레이 폭 // int
                    [2] 디스플레이 초기화 : 객체명.begin()
                    [3] 디스플레이 표시 of 버퍼
                        객체명.printf(~~~)
                        객체명.display()
                    [4] 텍스트 사이즈 변경
                        Adafruit_GFX_config.h에 있는 "GFX_SIZABLE_TEXT"의 주석 해제

                        객체명.setTextSize(size)
                            - size : 텍스트 사이즈 // uint8_t
                    [5] 그리기
                        Adafruit_GFX_config.h에 있는 "GFX_WANT_ABSTRACTS"의 주석 해제
                        1]] 선 그리기 : 객체명.drawLine(x0, y0, x1, y1, color)
                            - x0, y0 : 시작점 좌표 // int16_t
                            - x1, y1 : 끝점 좌표 // int16_t
                            - color : 색 // uint16_t
                                0 : 흰색
                                1 : 배경색(파란색)
                        2]] 직사각형 외곽선 그리기 : 객체명.drawRect(x, y, w, h, color)
                            - x, y : 직사각형 좌측 상단 좌표 // int16_t
                            - w, h : 직사각형 폭, 넓이 // int16_t
                            - color : 색 // uint16_t
                                0 : 흰색
                                1 : 배경색(파란색)
                        3]] 꽉찬 직사각형 그리기 : 객체명.fillRect(x, y, w, h, color)
                            - x, y : 직사각형 좌측 상단 좌표 // int16_t
                            - w, h : 직사각형 폭, 넓이 // int16_t
                            - color : 색 // uint16_t
                                0 : 흰색
                                1 : 배경색(파란색)
                        4]] 원 그리기 : 객체명.drawCircle(x, y, r, color)
                            - x, y : 원 중심 좌표 // int16_t
                            - r : 원 반경 // int16_t
                            - color : 색 // uint16_t
                                0 : 흰색
                                1 : 배경색(파란색)
    +a)
        1) 제품별 고유 I2C 주소
            (1) 8비트 주소 제공
                - 하위 R/W비트 포함
                - 읽기/쓰기 주소 별도 제공
            (2) 7비트 주소 제공
                - 하위 R/W비트 미포함
        2) BCD <-> 10진수
            16진수의 4비트씩 자른 단위에서 0~9만 사용(1바이트 = 최대 99까지 표현 가능)
            ex. 39(H) -> 57(D)
            ex. 39(BCD) -> 39(D)


● SPI
    0. SPI 개요
        1) 특징
            (1) 회로
                1] 4개 선(SS, SCLK, MOSI, MISO)
                    SS : Serial Select
                    SCLK : Serial Clock
                    MOSI : Master Output, Slave Input
                    MISO : Master Input, Slave Output
                    - 장치 개수 증가 >> SS선 증가
            (2) 통신 방식
                1] master-slave
                    - master가 각 slave에 고유 주소 부여x
                    - SS핀 활용해서 장치 선택o
                    - 4개 이상 선으로 복수개 장치와 통신 가능
                2] 동기 통신 : clock 신호에 동기
                    - I2C에 비해 훨씬 빠른 속도(최대 10Mbps)
                3] 전이중 양방향 통신
                    - 보내기, 받기 동시 가능
                    - 1:N 전이중 양방향 통신
        2) 작동 원리
            (1) SPI 모드 설정
                1] clock 극성, 데이터 읽기 위상 결정
                    [1] CPOL : Clock 시작 상태 지정
                        - 0 : clock이 0에서 시작
                        - 1 : clock이 1에서 시작
                    [2] CPHA : 데이터 읽는 엣지 지점 지정
                        - 0 : CPOL 지정 상태의 반대가 될 때
                        - 1 : CPOL 지정 상태가 될 때

                        1]] SPI_MODE0 : CPOL 0 / CPHA 0 / Data Capture Rising
                        2]] SPI_MODE1 : CPOL 0 / CPHA 1 / Data Capture Falling
                        3]] SPI_MODE2 : CPOL 1 / CPHA 0 / Data Capture Falling
                        4]] SPI_MODE3 : CPOL 1 / CPHA 1 / Data Capture Rising
                2] 데이터 전송 bit 순서
                    [1] MSBFIRST : MSB 먼저 보냄
                    [2] LSBFIRST : LSB 먼저 보냄
                    - mbed : MSBFIRST 사용
            (2) 쓰기 Master -> Slave
                1] Slave 선택 of master
                    - 해당 Slave SS핀 Low로 내림
                2] 쓰기모드 지정 of master
                    - 해당 Slave의 레지스터 주소 전송
                    - 최상위 R/W비트를 0으로 내림
                3] 데이터 전송 of master
                    - master가 레지스터에 데이터 전송
                4] Slave 선택 해제 of master
                    - 해당 Slave SS핀 High로 올림
            (3) 읽기 Slave -> Master
                1] Slave 선택 of master
                    - 해당 Slave SS핀 Low로 내림
                2] 읽기모드 지정 of master
                    - Slave의 원하는 레지스터 주소 전송
                    - 최상위 R/W비트를 1로 올림
                3] 데이터 전송 of Slave
                    - Master가 Slave에 아무 데이터나 보냄
                    - Slave가 레지스터에 데이터 전송
                4] Slave 선택 해제 of master
                    - 해당 Slave SS핀 High로 올림
    1. SPI Class
        1) 생성자 : SPI 객체명(mosi_pin, miso_pin, sclk_pin, ssel_pin) // SPI
            - mosi_pin : master output 핀 // PinName
            - miso_pin : master input 핀 // PinName
            - sclk_pin : clock 핀 // PinName
            - ssel_pin : slave select 핀 // PinName
        2) 설정
            (1) format 설정 : 객체명.format(bits, mode) // void
                - bits : 데이터 bit수 // int
                    bit : 6 ~ 15
                    default : 8
                - mode : SPI의 모드 // int
                    모드 : 0 ~ 3
            (2) clock 주파수 설정 : 객체명.frequency(hz) // void
                - hz : SPI clock 주파수 설정
                    default : 1MHz // int
        3) 쓰기 & 읽기
            - 레지스터 주소에 따라 읽기/쓰기 결정
            (1) 쓰기 : 객체명.write(data)
                - data : 전송하고자 하는 데이터 // int
            (2) 읽기 : int data = 객체명.write(아무거나)
                - 아무거나 : master가 slave에 아무 데이터나 넣으면 slave의 응답 반환
                - return : slave의 응답
        4) SPI 사용 순서
            (1) 객체 생성
                1] SPI 객체 생성 : SPI spi(MOSI, MISO, SCK);
                2] SS용 객체 생성 : DigitalOut ss(D6);
            (2) SPI 설정
                1] Slave 선택 : ss = 1;
                2] bit수, SPI모드 설정 : spi.format(bits, mode);
                3] clock 주파수 설정 : spi.frequency(hz);
            (3) slave 선택 : ss = 0;
            (4) 주소 쓰기 : spi.write(addr);
                - addr : 레지스터 주소. 상위비트는 강제로 1로 만들어야함
            (5) 데이터 읽기 : data = spi.write(아무거나);
            (6) slave 선택 해제 : ss = 1;
    2. 실습 센서
        1) MPU9250
            (1) 특징
                1] 구조
                    [1] 관성센서(IMU) : 6축
                        1]] 가속도 센서 : 3축(x, y, z)
                        2]] 각속도 센서 : 3축(x, y, z)
                        - roll, pitch 측정
                    [2] 지자기 센서 : 3축(x, y, z)
                        - yaw 측정
                2] 인터페이스
                    - I2C, SPI 가능
                3] 자세 추정
                    가속도 센서 -> a_x, a_y, a_z ->
                    자이로 센서 -> w_x, w_y, w_z -> 센서 융합 -> θ_x, θ_y, θ_z(roll, pitch, yaw) -> Filter
                    지자기 센서 -> m_x, m_y, m_z -> 
            (2) 활용
                1] 비행기 자세 제어
                    - 이동좌표계 x, y, z 각각 회전각
                    - roll, pitch, yaw


● Bus
    0. BusOut, BusIn 개요
        1) 개념 : 여러 개 핀 그룹화. 한꺼번에 입력 or 출력
    1. BusOut Class
        1) 생성자 : BusOut 객체명(Pin_name0, Pin_name1, ..., Pin_name15) ♣♣♣ 명령어
            - Pin_name : 버스로 사용하려는 핀 ♣♣♣
                Pin_name0 : 최하위 비트 // PinName
                Pin_name15 : 최상위 비트 // PinName
                생략 : No Connection
        2) 쓰기 ♣♣♣
            (1) 메소드 : 객체명.write(value)
                - value : 출력값(0~65535) // int
            (2) 연산자 오버로딩 : 객체명 = value
                - value : 출력값(0~65535) // int
        3) 읽기 ♣♣♣
            (1) 메소드 : int value = 객체명.read()
                - return : 출력값(0~65535) // int
            (2) 연산자 오버로딩 : int value = 객체명
                - return : 출력값(0~65535) // int
        4) 비트 단위 개별핀 접근 쓰기 : 객체명[idx] = value ♣♣♣ 명령어
            - idx : bus 비트 번호(0~15)
        5) 비트 단위 개별핀 접근 읽기 : int value = 객체명[idx] ♣♣♣ 명령어
            - idx : bus 비트 번호(0~15)
    2. BusIn Class
        1) 생성자 : BusIn 객체명(Pin_name0, Pin_name1, ..., Pin_name15) ♣♣♣ 명령어
            - Pin_name : 버스로 사용하려는 핀 ♣♣♣
                Pin_name0 : 최하위 비트 // PinName
                Pin_name15 : 최상위 비트 // PinName
                생략 : No Connection
        2) 입력모드 설정 : 객체명.mode(Pin_mode) ♣♣♣
            - Pin_mode : 입력핀 내부 Pull회로 지정 // Pinmode
                PullUp
                PullDown
                PullNone
        3) 읽기 ♣♣♣
            (1) 메소드 : int value = 객체명.read()
                - return : 출력값(0~65535) // int
            (2) 연산자 오버로딩 : int value = 객체명
                - return : 출력값(0~65535) // int
        4) 비트 단위 개별핀 접근 읽기 : int value = 객체명[idx]
            - idx : bus 비트 번호(0~15)
    +a)
        1) RGB LED ♣♣♣
            (1) 공통 음극(Common Cathode)
                - MCU -> GND : MCU가 high되면 켜짐
            (2) 공통 양극(Common Anode)
                - VCC -> MCU : MCU가 low되면 켜짐
        2) 7segment = Flexible Numeric Display ♣♣♣
            (1) 공통 음극(Common Cathode)
                - MCU -> GND : MCU가 high되면 켜짐
            (2) 공통 양극(Common Anode)
                - VCC -> MCU : MCU가 low되면 켜짐


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


● 초음파 거리 센서
    0. 초음파 센서 개요
        1) 초음파(Ultrasonic)
            (1) 개념 : 가청 주파수보다 높은 20kHz 이상 고음
            (2) 활용 : 거리측정 센서, 메디컬 영상, 비파괴 검사 등
            (3) 자연계 활용 : 돌고래, 박쥐의 방향, 거리 감지
        2) 초음파 거리 센서
            (1) 개념 : 초음파 발생 -> 체공 시간 = 반향(echo)까지의 시간 측정 -> 거리 계산
                1] 소리 체공시간 = 소리 왕복시간
                    dt = t_R - t_r

                    t_R : 수신 시간
                    t_r : 송신 시간
                    V_s : 음속(343.5m/s in 20도)
                2] 소리 왕복거리
                    2d = V_s x dt
                3] 측정 물체까지 거리
                    d = V_s x dt / 2
            (2) 활용 : 수중 거리 측정, 자동차 장애물 감지, 공장 자동화 등
            (3) 장점 : 열악한 환경 측정(수중, 연기, 분진, 안개, 어두운 밤 등) 가능 better than 광학식 거리 측정 센서
            (4) 실습용 센서 - HC-SR04
                1] 장점
                    - 쉬운 사용
                    - 싼 가격
                2] 구성
                    - 송신 스피커
                    - 수신 스피커
                3] 작동 방법
                    [1] [Trigger Pin 10us 동안 high 유지 -> low 되돌림] = 초음파 발사 = echo Pin high 유지
                    [2] 반향 돌아오면 echo Pin low 되돌림
                        - 반향 돌아오기 전까지 초음파 센서 blocking

● CircularBuffer
    0. CircularBuffer 개요
        1) 특징
            (1) 동의어 : CircularBuffer = RingBuffer
            (2) 도입 전 : 데이터 생산자 vs 데이터 소비자 처리 속도 차이 -> overrun -> 데이터 소실
            (3) 도입 후 : 일정 분량 데이터 저장 -> 데이터 소실 없이 완충
            (4) 구조 : 적당한 자료형의 1차원 배열
        2) 구조
            (1) Push : 데이터 넣기
                head에 데이터 저장 -> 저장 후 head 1 증가
            (2) Pop : 데이터 빼기
                tail에서 데이터 읽기 -> 읽기 후 tail 1 증가
            (3) 순환 구조
                head, tail 모두 마지막 원소 인덱스 도달 시, 인덱스 0으로 변화
        3) 데이터 속도 & OverRun
            (1) 소비속도 > 생산속도
                - 항상 : 버퍼에 데이터 거의 없음
            (2) 소비속도 < 생산속도
                - 일정 시간 이하 : 버퍼링
                - 일정 시간 이후 : OverRun
    1. CircularBuffer Class
        1) 생성자 : CircularBuffer<buf_typ, buf_sz, cnt_typ> 객체명 // template
            - buf_typ : 버퍼 데이터 자료형 // typename
            - buf_sz : 버퍼 크기 // uint32_t
            - cnt_typ : 유효한 데이터 개수 반환하는 size 멤버 함수 자료형 // typename
        2) 데이터 관리
            (1) 데이터 넣기 : 객체명.push(data) // void
                - data : push할 데이터 // const T
            (2) 데이터 빼기 : 객체명.pop(data) // bool
                - data : pop할 데이터 // const T
                - return : 성공 여부
                    성공 : true
                    실패 : false
            (3) 데이터 값 읽기 : 객체명.peek(data) // bool
                - data : peek할 데이터 // const T
                - return : 성공 여부
                    성공 : true
                    실패 : false
        3) 상태 확인
            (1) empty() : if(객체명.empty()) // bool
                - return : 버퍼 empty 여부
                    비어있는 버퍼 : true
                    비어있지 않은 버퍼 : false
            (2) full() : if(객체명.full()) // bool
                - return : 버퍼 full 여부
                    꽉차있는 버퍼 : true
                    꽉차있지 않은 버퍼 : false
            (3) 버퍼에 유효한 데이터 개수 : int 변수명 = 객체명.size() // CounterType
                - return : 버퍼에 저장된 유효한 데이터 개수


● StepMotor
    0. StepMotor 개요
        1) 특징
            (1) 장점 : 쉽고 정밀한 위치, 속도 제어
            (2) 활용 : 프린터, 스캐너, 복사기 등 사무기기, 산업용 기기
            (3) 방식 // ♣♣
                1] PM 방식
                2] Variable Reluctance 방식
                3] 하이브리드 방식
            (4) 코일 수 : 2상, 3상, 5상 등
        2) 구조
            (1) 2상 StepMotor
                1] 회전 : A, B코일의 전류 방향 조합 -> 1step 단위 회전
                2] 제어 : 2개 코일 각각 H-bridge 회로 제어
                3] Step 각
                    - 산업용 하이브리드 : 0.9도(400step) or 1.8도(200step)
                4] step 종류
                    [1] Full Step : Step각 = 90도
                        1]] 구동 순서
                            A -> B -> A' -> B' -> 반복
                    [2] Half Step : Step각 = 45도
                        1]] 구동 순서
                            A -> AB -> B -> BA' -> A' -> A'B' -> B' -> B'A
                        2]] 토크 리플
                            [[1]] 발생 이유 : 각 코일 사이에 회전자 위치 시, 양 코일이 반응하여 토크 크기 증가
                            [[2]] 해결 방안 : 토크 크기 일정화를 위해 전류를 더 약하게 흘려, 자기력의 합력 일정하게 맞춤
                    [3] Micro Step
                        1]] 구동 순서 : Step각 = 잘게 쪼갬
                            Half Step 이상으로 쪼개서 sin파처럼 움직임
                        2]] 토크 리플
                            [[1]] 발생 이유 : 각 코일 사이에 회전자 위치 시, 양 코일이 반응하여 토크 크기 증가
                            [[2]] 해결 방안 : 토크 크기 일정화를 위해 전류를 더 약하게 흘려, 자기력의 합력 일정하게 맞춤
        3) 실습용 모터 드라이버 - DRV8825
            (1) 특징
                1] Logic 전압 : 2.5V ~ 5.25V
                1] 최대 전류 : 2.2A

                2] 모터 구동 전압 : 8.2V ~ 45V
                2] 최대 1/32 MicroStep 지원
            (2) 핀
                1] Pulse핀 : 1pulse = 1MicroStep
                1] Direction핀 : 모터 회전 방향

                2] 제어핀
                    - 교수님 설명x. 이것도 해야되나??? ♣♣♣♣♣♣♣♣♣♣♣♣♣♣
                3] MicroStep핀
                    MS1 MS2 MS3 resolution
                     0   0   0      1
                     1   0   0      1/2
                     0   1   0      1/4
                     1   1   0      1/8
                     0   0   1      1/16
                     그 외          1/32


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


● DC Motor Drive    // 마이컨과 겹침. 강의자료 참조 ♣♣♣
    1) H-bridge 회로
        (1) 원리
        (2) 종류
        (3) 제품
*/