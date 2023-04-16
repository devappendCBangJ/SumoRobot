# state 정의
# 빨간색 영역이 왼쪽에 위치    : 'L'
# 빨간색 영역이 오른쪽에 위치  : 'R'
# 빨간색 영역이 양쪽에 위치    : 'M'
# 파란색 영역이 왼쪽에 위치    : 'L'
# 파란색 영역이 오른쪽에 위치  : 'R'
# 파란색 영역이 양쪽에 위치    : 'M'
# 로봇이 전방 좌측 방향에 위치 : 'l'
# 로봇이 전방 우측 방향에 위치 : 'r'
# 로봇이 전방 중앙에 위치     : 'm'
# 로봇이 없음               : 'n'
# *전략에 따라 추가 요망*

# 이미지 크기 조정
# cv2.resize(이미지 변수, None or (가로방향 픽셀수, 세로방향 픽셀수), fx=x축척, fy=y축척, interpolation=cv2.INTER_AREA)
    # - 이미지 확대 : cv2.INTER_CUBIC or cv2.INTER_LINEAR
    # - 이미지 축소 : cv2.INTER_AREA

# 도형 그리기
# - 원 도형 : cv2.circle(이미지 파일, (원 x좌표, 원 y좌표), 원 반지름 mm, (b,g,r), 두께 mm)
# - 사각형 도형 : cv2.rectangle(이미지 파일, (시작 x좌표, 시작 y좌표), (종료 x좌표, 종료 y좌표), (b,g,r), 두께 mm)

# hsv 행렬 씌우기
# 특정 범위 hsv 행렬 원소 검출 후 씌우기 : cv2.inRange(입력 행렬, 하한 값 행렬, 상한 값 행렬)

# 행렬 만들기
# 1로 채워진 배열 반환 : np.ones((행 개수, 열 개수), np.uint8)

# 모폴로지(형태학)
    # - 적용 가능 이미지 : binary 이미지
    # - 0과 1로 구성된 커널 필요
    # - 종류
        # 1) 침식 : 이미지 깎음 연산 (어두운 노이즈 제거)
            # - 1인 이미지 픽셀을 커널 중심부가 모두 훑은 후, 완전히 겹치면 1 그대로 / 완전히 겹치지 않으면 0으로 변경
        # 2) 팽창 : 이미지 확장 연산 (밝은 노이즈 제거)
            # - 1인 이미지 픽셀을 커널 중심부가 모두 훑은 후, 완전히 겹치지 않으면 1로 변환
        # 3) 열림 : 침식 + 팽창 (굵기 변함 x. 밝은 노이즈 제거)
        # 4) 닫힘 : 팽창 + 침식 (굵기 변함 x. 어두운 노이즈 제거)
        # 5) 그레디언트 : 팽창 - 침식 (경계 검출)
        # 6) 탑햇 : 원본 - 열림 (매우 밝은 영역 강조)
        # 7) 블랙햇 : 닫힘 - 원본 (매우 어두운 영역 강조)

# 구조화 요소 커널 생성 : 구조화 요소 커널 = cv2.getStructuringElement(커널 모양, (커널 x 크기, 커널 y 크기), 기준점)
    # - 커널 모양 사각형 : cv2.MORPH_RECT
    # - 커널 모양 타원형 : cv2.MORPH_ELLIPSE

# 침식 연산 : cv2.erode(입력 영상, 구조화 요소 커널)
# 팽창 연산 : cv2.dilate(입력 영상, 구조화 요소 커널)
# 열림 연산 : cv2.morphologyEx(입력 영상, cv2.MORPH_OPEN, 구조화 요소 커널)
# 닫힘 연산 : cv2.morphologyEx(입력 영상, cv2.MORPH_CLOSE, 구조화 요소 커널)
# 그레디언트 연산 : cv2.morphologyEx(입력 영상, cv2.MORPH_GRADIENT, 구조화 요소 커널)
# 탑햇 연산 : cv2.morphologyEx(입력 영상, cv2.MORPH_TOPHAT, 구조화 요소 커널)
# 블랙햇 연산 : cv2.morphologyEx(입력 영상, cv2.MORPH_BLACK, 구조화 요소 커널)

# 연결된 객체 단위 분석 : 객체를 분할해서 특징 분석
    # - 적용 가능 이미지 : binary 이미지
    # - 용도 : 객체 위치, 크기 정보, ROI 추출, 모양 분석
    # - 종류
        # 1) 레이블링 : 객체 구역을 영역 단위로 분석
            # - 객체 구분 : 0 >> 배경 / 1이상 >> 객체
            # 같은 객체에 번호가 지정된 정수형 행렬을 레이블 맵이라 부름
            # 서로 연결되어있으면 같은 번호 저장
            
            # a. 4-이웃 연결 관계 : + 모양 연결
            # b. 8-이웃 연결 관계 : 3x3 모양 연결
        # 2) 외곽선 검출

# 연결된 객체 단위 분석
# 레이블 맵 형성 : cv2.connectedComponents(8비트 1채널 입력 영상)
# 객체 정보 반환 + 레이블 맵 형성 : cv2.connectedComponentsWithStats(8비트 1채널 입력 영상) = retval, labels, stats, centroids
    # - 반환 : retval, labels, stats, centroids
    # - retval : 객체 수 + 1 (배경 포함)
    # - labels : 객체별 번호 레이블 맵
    # - stats : N행 5열
        # N행 : 객체 수 + 1 (배경 포함)
        # 5열 : x, y, width, height, area (좌측 상단 좌표, 면적, 픽셀 수)
    # - centroids : N행 2열
        # N행 : 객체 수 + 1 (배경 포함)
        # 2열 : x, y 무게 중심 좌표 픽셀

# enumerate() 함수 : 인덱스와 원소로 이루어진 튜플 생성
    # for i in enumerate(['A', 'B', 'C'], start = 0)
        # print(i)
    # 결과 :
    # (0, 'A')
    # (1, 'B')
    # (2, 'C')

    # for i, letter in enumerate(['A', 'B', 'C'], start = 109)
        # print(i, letter)
    # 결과 :
    # 109 A
    # 110 B
    # 111 C

# 배열 데이터 비교
# 조건에 맞는 배열 데이터 True or False 출력 : np.any(조건)
# 조건에 완전히 맞는 배열 데이터 True or False 출력 : np.all(조건)
# shape이 같은 배열간 비교 >> 요소 각각 True or False 출력

# 모듈 불러오기
import cv2
import numpy as np
import serial
import time

# 시리얼 설정
ser = serial.Serial(port='/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_0670FF525157878367085346-if02', baudrate=115200,
                    timeout=0)

# 카메라 가로, 세로, 면적(노이즈, 배틀) 설정
width = 320
height = 240
erase_noise_area = (width * height) / 300
battle_mode_area = 1400

# mask 씌운 이미지로 변환 (hsv 카메라 변수, hue, threshold)
def masking(front_Camera_hsv_def, hue, threshold):
    # hue 범위 설정
    if hue < 10:
        lower1 = np.array([hue - 10 + 180, threshold, threshold])
        upper1 = np.array([180, 255, 255])
        lower2 = np.array([0, threshold, threshold])
        upper2 = np.array([hue, 255, 255])
        lower3 = np.array([hue, threshold, threshold])
        upper3 = np.array([hue + 10, 255, 255])
    elif hue > 170:
        lower1 = np.array([hue, threshold, threshold])
        upper1 = np.array([180, 255, 255])
        lower2 = np.array([0, threshold, threshold])
        upper2 = np.array([hue + 10 - 180, 255, 255])
        lower3 = np.array([hue - 10, threshold, threshold])
        upper3 = np.array([hue, 255, 255])
    else:
        lower1 = np.array([hue, threshold, threshold])
        upper1 = np.array([hue + 10, 255, 255])
        lower2 = np.array([hue - 10, threshold, threshold])
        upper2 = np.array([hue, 255, 255])
        lower3 = np.array([hue - 10, threshold, threshold])
        upper3 = np.array([hue, 255, 255])

    # hsv 범위 이용 마스킹
    front_Camera_mask_def1 = cv2.inRange(front_Camera_hsv_def, lower1, upper1)
    front_Camera_mask_def2 = cv2.inRange(front_Camera_hsv_def, lower2, upper2)
    front_Camera_mask_def3 = cv2.inRange(front_Camera_hsv_def, lower3, upper3)
    front_Camera_mask_def = front_Camera_mask_def1 | front_Camera_mask_def2 | front_Camera_mask_def3

    # 노이즈 제거 - 모폴로지 적용
    kernel = np.ones((5, 5), np.uint8) # 5x5로 한 이유???
    front_Camera_mask_def = cv2.morphologyEx(front_Camera_mask_def, cv2.MORPH_OPEN, kernel) # 흰색 작은 영역 제거
    front_Camera_mask_def = cv2.morphologyEx(front_Camera_mask_def, cv2.MORPH_CLOSE, kernel) # 검은 구멍 메우기

    # 마스크 이미지 반환
    return front_Camera_mask_def

# 영역 위치 판별 (카메라 변수, 카메라 마스크 변수, 색)
def state_decision(front_Camera_def, front_Camera_mask_def, color):
    # 연결된 객체 단위 분석
    number, img_label, stats, centroids = cv2.connectedComponentsWithStats(front_Camera_mask_def)
    count = 0
    
    # 연결된 객체 특징 저장
    for idx, centroid in enumerate(centroids):
        if stats[idx][0] == 0 and stats[idx][1] == 0: # 각 물체 x, y좌표가 0일 때 건너띔
            continue
        if np.any(np.isnan(centroid)): # ???
            continue
        x, y, wid, hei, area = stats[idx] #물체별 stats값 저장
        cx, cy = int(centroid[0]), int(centroid[1]) #물체별 중심좌표 저장

        # 노이즈보다 큰 영역인 경우
        if area > erase_noise_area:
            # print(area) # 확인용 코드

            # 빨강 영역, 파랑 영역의 경우
            if (color == 'red') | (color == 'blue'):
                # 원 도형 그리기
                cv2.circle(front_Camera_def, (cx, cy), 10, (255, 255, 255), 10)

                # 영역 개수와 위치 판별 - 한쪽만 영역인가 / 양쪽 모두 영역인가
                for centroid2 in centroids:
                    if centroid2[0] < width / 3:
                        count -= 1
                    elif centroid2[0] > (width / 3) * 2:
                        count += 1

                if abs(count) == int(number) - 1:
                    confirm = 0
                else:
                    confirm = 1

                # 빨강 파랑 위치 반환 (3등분)
                if ((cx < width / 3) & confirm == 0):
                    state = 'L'
                elif ((cx > (width / 3) * 2) & (confirm == 0)):
                    state = 'R'
                elif ((cx < width / 3) | (cx > (width / 3) * 2) & (confirm == 1)):
                    state = 'M'
                else:
                    state = 'G'
                return state

            # 초록 영역의 경우
            if color == 'green':
                # 원, 사각형 도형 그리기
                cv2.circle(front_Camera_def, (cx, cy), 10, (0, 255, 0), 10)
                cv2.rectangle(front_Camera_def, (x, y), (x + wid, y + hei), (0, 255, 0))

                # 초록 영역 크기 큼 : 배틀 모드 반환
                if area > battle_mode_area:
                    state = 'GO'
                    print('battle mode')
                # 초록 영역 크기 작음 : 초록색 위치 반환 (3등분)
                else:
                    if cx < (width / 8) * 3:
                        state = 'l'
                    elif cx > (width / 8) * 5: # 5아닌가???
                        state = 'r'
                    else:
                        state = 'm'
                return state

# 윈도우창 생성, 카메라 할당
cv2.namedWindow('front_Camera')
cap = cv2.VideoCapture('/dev/v4l/by-id/usb-Xiongmai_web_camera_12345678-video-index0')

while True:
    # 카메라 읽기, 크기 조정, hsv 변환
    ret, front_Camera = cap.read()
    front_Camera = cv2.resize(front_Camera, (width, height), interpolation=cv2.INTER_AREA)
    front_Camera_hsv = cv2.cvtColor(front_Camera, cv2.COLOR_BGR2HSV)

    # hsv 색별 마스크 변환
    front_Camera_red_mask = masking(front_Camera_hsv, 200, 120)
    front_Camera_blue_mask = masking(front_Camera_hsv, 200, 100)
    front_Camera_green_mask = masking(front_Camera_hsv, 60, 30)

    # hsv 색별 state 종합 후 최종 결정
    Green_state = state_decision(front_Camera, front_Camera_green_mask, 'green') # 3번째 green 이 부분은 왜 있는거???

    Final_state = Green_state # 이거 있는 이유???
    # 로봇을 감지한 경우(area 가 battle mode area 이상), 배틀 모드로 바뀌어 Final_state 는 바닥색으로 반환

    # - 초록 영역 크기 작게 감지 >> 초록 영역 위치만 보냄
    if Green_state == 'l' or Green_state == 'r' or Green_state == 'm':
        Final_state = Green_state
    # - 초록 영역 크기 크게 감지 & 영역 감지 >> 빨파 영역 위치만 보냄 >> mbed에서 Green_state == 'GO'일 경우 Red_state와 Blue_state만으로 판단
    elif Green_state == 'GO':
        Red_state = state_decision(front_Camera, front_Camera_red_mask, 'red') # 가까이 있을 때는 이거 연산 할 필요 없지 않나???
        Blue_state = state_decision(front_Camera, front_Camera_blue_mask, 'blue') # 가까이 있을 때는 이거 연산 할 필요 없지 않나???

        # - 빨파 영역 감지
        if Red_state == 'L' or Red_state == 'R' or Red_state == 'M':
            Final_state = Red_state
        elif Blue_state == 'L' or Blue_state == 'R' or Blue_state == 'M':
            Final_state = Blue_state
        # - 빨파 영역 감지 x
        else:
            Final_state = 'G'
    # - 로봇 감지 x
    else:
        Final_state = 'n'

    print(Final_state) # 확인용 코드
    #ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
    # state별 mask 최종 결정
    # - 초록 영역 크기 크게 감지 >> 모든 마스크 보냄
    if Green_state == 'GO':
        front_Camera_mask = front_Camera_red_mask | front_Camera_green_mask | front_Camera_blue_mask
    # - 초록 영역 크기 작게 감지 >> 초록 마스크만 보냄
    else:
        front_Camera_mask = front_Camera_green_mask

    #ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
    # state 인코딩 후 통신
    Trans_state = Final_state.encode('utf-8')
    if ser.writable:
        ser.write(Trans_state)

    # 이미지 띄움
    cv2.imshow('front_Camera', front_Camera)
    cv2.imshow('front_Camera_mask', front_Camera_mask)
    # ESC 누르면 종료
    if cv2.waitKey(1) & 0xFF == 27:
        break

# 자원 해제
cv2.destroyAllWindows()