#View > Tool Windows > Terminal에서 Terminal창 >> pip install opencv-python 입력

# <카메라 불러오기 함수>
# 0. 영상처리 모듈 : import cv2
# 1. 카메라 할당 : 카메라 변수 = cv2.VideoCapture(cv2.CAP_DSHOW + 숫자) #0부터 정수로 표현. 카메라 대수만큼 0부터 할당
# 1. 동영상 저장 할당 : 저장 변수 = cv2.VideoWriter("파일 위치/파일명", cv2.VideoWriter_fourcc(*'XVID'), fps 수, (영상 화면 가로 픽셀 수, 영상 화면 세로 픽셀 수)) #XVID : 코덱
# 2. 카메라 읽기 : ret,컬러 이미지 변수 = 카메라 변수.read()
# 2-1. 카메라 읽고 gray 변환 : 그레이 이미지 변수 = cv2.cvtColor(컬러 이미지 변수, cv2.COLOR_BGR2GRAY)
# 3. 윈도우 생성 : cv2.namedWindow('윈도우 식별자') # 여러 창 띄울 경우 서로 다른 윈도우 식별자 사용 # 생략 가능
# 4. 이미지 띄움 : cv2.imshow('윈도우 식별자', 컬러 이미지 변수 or 그레이 이미지 변수) # 여러 창 띄울 경우 서로 다른 윈도우 식별자 사용
# 5. 카메라에서 읽는 동영상 새롭게 저장 : 저장 변수.write(컬러 이미지 변수)
# 6. 키보드 입력 대기 : cv2.waitKey(초)
# 7. 자원 해제 of 카메라 읽기 : 카메라 변수.release()
# 7. 자원 해제 of 이미지 저장 : 저장 변수.release()
# 7. 자원 해제 of 윈도우  : cv2.destroyAllWindows()

# 키보드 입력 대기 : cv2.waitKey(초)

# while문 : 카메라 이미지 연속으로 호출 >> 영상처럼 표현

# 캡쳐 되지 않는 경우 루프 재시작
    # if ret == False:
        #continue

# ESC 누를 시 루프 탈출
    # if cv2.waitKey(1) & 0xFF == 27:
    #     break

import cv2
import time
import threading

# 카메라 할당
cap0 = cv2.VideoCapture(cv2.CAP_DSHOW + 0)
cap1 = cv2.VideoCapture(cv2.CAP_DSHOW + 1)

d=cap1
count = 0

# 비디오 저장
fourcc = cv2.VideoWriter_fourcc(*'XVID')
writer = cv2.VideoWriter('camera.avi', fourcc, 30, (640, 480))

def isopen():
    while(True):
        if(cap1.isOpened()==True):
            raise

t1 = threading.Thread(target=isopen)
t1.daemon = True
t1.start


# 카메라 읽기 & 출력
while(True):
    try:
        count += 1
        print(count)
        ret, img_color = d.read()

        # ret,img_color = cap.read()
        img_gray = cv2.cvtColor(img_color, cv2.COLOR_BGR2GRAY)

        # if ret1 == False:  # 캡쳐 되지 않는 경우 루프 재시작
        #     continue

        cv2.imshow("color", img_color)
        cv2.imshow("gray", img_gray)

        # writer.write(img_color)

        if cv2.waitKey(1) & 0xFF == 27:  # ESC 누를 시 루프 탈출
            break

        if(count%30 == 0):
            raise Exception("에러에러에러!!")
    except:
        if(d==cap1):
            cap1.release()
            cap0 = cv2.VideoCapture(cv2.CAP_DSHOW + 0)
            # cap1 = cv2.VideoCapture(cv2.CAP_DSHOW + 1)
            d=cap0
        elif(d==cap0):
            cap0.release()
            # cap0 = cv2.VideoCapture(cv2.CAP_DSHOW + 0)
            cap1 = cv2.VideoCapture(cv2.CAP_DSHOW + 1)
            d=cap1

    if cv2.waitKey(1) & 0xFF == 27:  # ESC 누를 시 루프 탈출
        break

#자원 해제
cap0.release()
cap1.release()
writer.release()
cv2.destroyAllWindows()

# 최적화 시킨 코드
# import cv2
# 
# # 카메라 할당
# cap0 = cv2.VideoCapture(cv2.CAP_DSHOW + 0)
# cap1 = cv2.VideoCapture(cv2.CAP_DSHOW + 1)
# 
# d=cap1
# 
# # 카메라 읽기 & 출력
# while(True):
#     try:
#         ret, img_color = d.read()
# 
#         img_gray = cv2.cvtColor(img_color, cv2.COLOR_BGR2GRAY)
# 
#         cv2.imshow("color", img_color)
#         cv2.imshow("gray", img_gray)
# 
#         if cv2.waitKey(1) & 0xFF == 27:  # ESC 누를 시 루프 탈출
#             break
# 
#     except:
#         if(d==cap1):
#             cap1.release()
#             cap0 = cv2.VideoCapture(cv2.CAP_DSHOW + 0)
#             # cap1 = cv2.VideoCapture(cv2.CAP_DSHOW + 1)
#             d=cap0
#         elif(d==cap0):
#             cap0.release()
#             # cap0 = cv2.VideoCapture(cv2.CAP_DSHOW + 0)
#             cap1 = cv2.VideoCapture(cv2.CAP_DSHOW + 1)
#             d=cap1
# 
#     if cv2.waitKey(1) & 0xFF == 27:  # ESC 누를 시 루프 탈출
#         break
# 
# #자원 해제
# cap0.release()
# cap1.release()
# cv2.destroyAllWindows()