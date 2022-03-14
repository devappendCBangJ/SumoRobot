/*
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


● Ticker, Timer, TimeOut
    0. Timer 개요
        1) Ticker : 일정 주기 시간. callback 반복
        2) Timer : 임의 구간 시간
            - start : 기동
            - stop : 정지
            - reset : 0
            - read : 읽기.
        3) TimeOut : 지정한 시간 후, 1회 callback 측정
    1. Timer Class
        1) 생성자 : Ticker 객체명
        2) 콜백 함수 지정
            (1) 일반 함수 지정
                1] 초 단위 주기 : 객체명.attach(&func, time)
                    - &func : 주기적으로 호출될 콜백 함수 주소. 반환형이 void여야함 // Callback<void()>
                    - time : 콜백 함수 호출 주기(초단위) // float
                2] us 단위 주기 : 객체명.attach_us(&func, time_us)
                    - &func : 주기적으로 호출될 콜백 함수 주소. 반환형이 void여야함 // Callback<void()>
                    - time_us : 콜백 함수 호출 주기(마이크로 초단위) // us_timestamp_t
            (2) 멤버 함수 지정
                1] 초 단위 주기 : 객체명.attach(Callback(obj, &func), time)
                    - obj : Ticker 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 주소 // M
                    - time : 콜백 함수 호출 주기(마이크로 초단위) // us_timestamp_t

                    class myClass{
                        Ticker tic;
                        myClass(){
                            tic.attach(callback(this, &myClass::ISR), time)
                        }
                    }
                2] us 단위 주기 : 객체명.attach(Callback(obj, &func), time_us)
                    - obj : Ticker 객체가 포함된 객체의 주소 // T
                    - &func : 호출될 멤버 함수의 주소 // M
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
                - &func : 인터럽트 발생 시 호출될 함수 주소 // Callback<void()>
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
                <string> 명령어 사용 가능
            - RawSerial : 오류 발생x
                printf 정의x
                scanf 정의x
                <string> 명령어 사용 불가
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
                - &task : 실행시키고자 하는 함수 주소
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
                - &task : 실행시키고자 하는 함수 주소
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

*/