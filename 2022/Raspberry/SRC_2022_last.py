import time
import cv2
import numpy as np
from collections import deque
import serial

# 시리얼 설정
ser = serial.Serial(                # serial 객체
    port = '/dev/ttyAMA1',          # serial통신 포트
    baudrate = 115200,              # serial통신 속도
    parity = serial.PARITY_EVEN,    # 패리티 비트 설정
    stopbits = serial.STOPBITS_ONE, # 스톱 비트 설정
    bytesize = serial.EIGHTBITS,    # 데이터 비트수
    timeout = 1                     # 타임 아웃 설정
)

# 카메라 설정, 화면 사이즈, hsv
# cap = cv2.VideoCapture(cv2.CAP_DSHOW + 1)
cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
#cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('M', 'J', 'P', 'G'))
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('Y', '1', '6', ' '))
cap.set(cv2.CAP_PROP_CONVERT_RGB, 0)

W_View_size = 400
H_View_size = 300

#green
#동방용

lower_green_t = (56,100,50)
upper_green_t = (68,255,255)

lower_green = lower_green_t
upper_green = upper_green_t

ad_thres_Hue = 3
ad_thres_Sat = 5
ad_thres_Val = 5

adjust_lower_green = (lower_green_t[0]-(ad_thres_Hue+2),lower_green_t[1]-ad_thres_Sat,lower_green_t[2]-ad_thres_Val)
adjust_upper_green = (upper_green_t[0]+ad_thres_Hue,upper_green_t[1],upper_green_t[2])

lower_blue = (107, 150, 110)
upper_blue = (120, 255, 255)

lower_red1 = (172, 90, 160)
upper_red1 = (179, 255, 255)

lower_red2 = (0, 70, 160)#90
upper_red2 = (6, 255, 255)

# 원통 크기 기준, 통신 데이터 리스트
green_small = W_View_size * H_View_size * 0.0009#108
#green_middle = W_View_size * H_View_size * 0.00167#200
green_middle = W_View_size * H_View_size * 0.00167#200
green_big = W_View_size * H_View_size * 0.00334 #400.8
green_Vbig = W_View_size * H_View_size * 0.01 #1200
green_Fbig = W_View_size * H_View_size * 0.0150 #1800
ras_res= ""

FPS=list()

hsv_adjust_mode = 0

count_adjust_mode = 0

count = 0

Filter_Area = deque([0, 0, 0, 0, 0])

if not cap.isOpened():
    print('ERROR! Unable to open camara')
    
'''
cap.set(cv2.CAP_PROP_FRAME_WIDTH,320)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT,240)
'''

kernel = np.ones((3, 3), np.uint8)

rx,rw =0,0
    
def Masking(img_hsv,lower,upper):
    
    img_mask = cv2.inRange(img_hsv, lower, upper)
    #모폴로지
    img_mask = cv2.morphologyEx(img_mask, cv2.MORPH_OPEN, kernel)
    img_mask = cv2.morphologyEx(img_mask, cv2.MORPH_CLOSE, kernel)
    return img_mask

def get_contour_green(contours):
    max_contour = None
    max_area = -1
    for contour in contours: # 컨투어 제일 넓이 큰거 하나만 구하는 과정
        area = cv2.contourArea(contour)
        #print(area)
        if cv2.contourArea(contour) < 50:  #  너무 작으면 무시(노이즈제거)
            continue
        if area>max_area:
            max_area = area
            max_contour = contour
    return max_area,max_contour

def get_contour_RnB(contours):
    max_contour = None
    second_contour = None
    max_area = -1
    second_area = -1
    for contour in contours: # 컨투어 제일 넓이 큰거 하나만 구하는 과정
        area = cv2.contourArea(contour)
        #print(area)
        if cv2.contourArea(contour) > 400:  #  너무 작으면 무시(노이즈제거)
            if area>max_area:
                second_area=max_area
                second_contour=max_contour
                max_area = area
                max_contour = contour
            elif area>second_area:
                second_area = area
                second_contour = contour
    return max_area,max_contour, second_area,second_contour

def get_contour_B(contours):
    max_contour = None
    second_contour = None
    max_area = -1
    second_area = -1
    for contour in contours: # 컨투어 제일 넓이 큰거 하나만 구하는 과정
        area = cv2.contourArea(contour)
        #print(area)
        if cv2.contourArea(contour) > 500:  #  너무 작으면 무시(노이즈제거)
            if area>max_area:
                second_area=max_area
                second_contour=max_contour
                max_area = area
                max_contour = contour
            elif area>second_area:
                second_area = area
                second_contour = contour
    return max_area,max_contour, second_area,second_contour

#ang = 90
#start2 = time.time()

while True:
    try:
        start = time.perf_counter()
        '''
        curTime = time.time()
        sec = curTime - prevTime
        fps = 1/sec
        FPS.append(fps)
        count=count+1
        if(count==50):
            print("평균 fps:",np.mean(FPS))
            FPS=[]
            count=0
        '''
        # 카메라 읽기
        ret,frame = cap.read()
        frame = cv2.resize(frame, dsize=(W_View_size, H_View_size), interpolation=cv2.INTER_AREA)
        if not ret:
            continue
        
        # hsv 조정모드
        if hsv_adjust_mode == 0:
            pass
        elif hsv_adjust_mode == 1:
            lower_green = adjust_lower_green
            upper_green = adjust_upper_green
            hsv_adjust_mode = 2
        elif hsv_adjust_mode == 3:
            lower_green = adjust_lower_green
            upper_green = adjust_upper_green
            hsv_adjust_mode = 0
            
        # hsv 변환, 노이즈제거, 마스크형성
        img_hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        bin_image1 = Masking(img_hsv,lower_green,upper_green)#|Masking(img_hsv,lower_green2,upper_green2)
        bin_image2 = Masking(img_hsv,lower_blue,upper_blue)
        bin_image3 = Masking(img_hsv,lower_red1,upper_red1)|Masking(img_hsv,lower_red2,upper_red2)
        
        '''
        roi = bin_image3[120:,:]
        res = np.zeros([120,400],dtype=np.uint8)
        bin_image3 = np.vstack((res,roi))
        '''
        #최종마스킹
        bin_image4 = bin_image1|bin_image2|bin_image3
        
        roi_red = bin_image3[120:,:]
        roi_green = bin_image1[:190,:]
        #print(np.any(roi_green))#하나라도 보이면 true
        if (np.any(roi_green)==False):
            roi_green = bin_image1
            
        #컨투어
        contours_green, _ = cv2.findContours(roi_green, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        contours_red, _ = cv2.findContours(roi_red, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        contours_blue, _ = cv2.findContours(bin_image2, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        #print(len(contours_red))
        #컨투어 영역 제일 넓은 부분만 구하기(빨간색은 2번쨰까지)
        max_areaG, max_contourG = get_contour_green(contours_green)
        max_areaR, max_contourR, second_areaR, second_contourR = get_contour_RnB(contours_red)
        max_areaB, max_contourB, second_areaB, second_contourB = get_contour_B(contours_blue)
        
        gx, gy, gw, gh = cv2.boundingRect(max_contourG)
        
        bx1, by1, bw1, bh1 = cv2.boundingRect(max_contourB)
        bx2, by2, bw2, bh2 = cv2.boundingRect(second_contourB)
        
        rx1, ry1, rw1, rh1 = cv2.boundingRect(max_contourR)
        rx2, ry2, rw2, rh2 = cv2.boundingRect(second_contourR)
        
        centerX, centerY = gx+gw/2, gy+gh/2
        #print(max_areaR, second_areaR)
        ry1=ry1+120
        ry2=ry2+120
        
        #len 세로/가로 비 확인해서 1 보다 작을 경우...
        if (len(contours_green) != 0):
            if gw == 0:
                ratio = -1
            else:
                ratio = gh/gw
        else:
            ratio = -1
        
        if hsv_adjust_mode == 0:
            if len(contours_green) == 0 or (len(contours_green) != 0 and max_areaG < green_middle and ratio < 1):
                hsv_adjust_mode = 1
        elif hsv_adjust_mode == 2:
            if count_adjust_mode < 90:
                count_adjust_mode = count_adjust_mode + 1
            else:
                count_adjust_mode = 0
                hsv_adjust_mode = 3
        #print(hsv_adjust_mode)
                
        #GRectArea=gw*gh
        
        '''
        Filter_Area.appendleft(GRectArea)
        Filter_Area.pop()
        mean_Area=int(sum(Filter_Area)/5)
        '''
        
        
        #빨간색 컨투어가 1개만 잡히면
        if second_areaR == -1:
            rx = rx1
            rw = rw1
        #빨간색 컨투어가 2개 잡히면
        else:
            if rx1 > rx2:
                rx = rx2
            else:
                rx = rx1
            if rx1+rw1 >= rx2+rw2:
                rx_2 = rx1+rw1
            else:
                rx_2 = rx2+rw2   
            rw = rx_2-rx
        
        #파란색 컨투어가 1개만 잡히면
        if second_areaB == -1:
            bx = bx1
            bw = bw1
        #파린색 컨투어가 2개 잡히면
        else:
            if bx1 > bx2:
                bx = bx2
            else:
                bx = bx1
            if bx1+bw1 >= bx2+bw2:
                bx_2 = bx1+bw1
            else:
                bx_2 = bx2+bw2   
            bw = bx_2-bx
        
        
        #print("green: ",max_areaG)
        #print("red: ",max_areaR)
                
        cv2.rectangle(bin_image4, (gx, gy), (gx+gw, gy+gh), (255, 0, 0), 2)
        cv2.rectangle(bin_image4, (rx1, ry1), (rx1+rw1, ry1+rh1), (255, 0, 0), 2)
        cv2.rectangle(bin_image4, (rx2, ry2), (rx2+rw2, ry2+rh2), (255, 0, 0), 2)
        cv2.rectangle(bin_image4, (bx1, by1), (bx1+bw1, by1+bh1), (255, 0, 0), 2)
        cv2.rectangle(bin_image4, (bx2, by2), (bx2+bw2, by2+bh2), (255, 0, 0), 2)
        
        '''
        inside_R = False
        inside_B = False
                
        #빨간색영역 안에 원통 중심좌표 있을 경우
        if gx > rx and gx+gw < rx+rw:
            inside_R = True
        else:
            inside_R = False
         ''' 
        
        
        '''
        if inside:
            print("I")
        else:
            print("O")
        '''
        
        # 결과 연산, 시리얼 통신
        ras_res = ""
        #안 보일 시
        if(max_areaG == -1):
            ras_res = "999"
        else:
            ras_res = ras_res+str(gx)
        
        #print(max_areaG, GRectArea)
        
        if(len(contours_green) != 0): #contours 존재하면
            if(max_areaG < green_small):#멀음
                ras_res += ",1"
            elif(green_small <= max_areaG < green_middle):#중간
                ras_res += ",2"
            elif(green_middle <= max_areaG < green_big):#가까움
                ras_res += ",3"
            elif(green_big <= max_areaG < green_Vbig):#매우가까움
                ras_res += ",4"
            elif(green_Vbig <= max_areaG < green_Fbig):#매우매우가까움
                ras_res += ",5"
            elif(green_Fbig <= max_areaG):#준내게가까움
                ras_res += ",6"
            
                #print(ras_res)
        #elif((centerX!=0 and centerY!=0) and (len(contours) ==0)): 여기서 좀 더 세부적으로 나눌거임.
            #ras_res += "9,9"
        else: #contours 없으면
            ras_res += ",9"
            #print(ras_res)
        
        inside_R = False
        inside_B = False
                
        #빨간색영역 안에 원통 중심좌표 있을 경우
        if gx > rx and gx+gw < rx+rw:
            #if second_areaR != -1:#컨투어가 2개 잡힐 경우
            if (green_Vbig <= max_areaG):
                #print("1")
                inside_R = False
            else:
                inside_R = True
            #else:#컨투어가 1개 일 경우        
            #   inside_R = True
        else:
            #print("2")
            inside_R = False
        
        #파란색 영역 안에 원통 중심 좌표가 있을 경우    
        if gx > bx and gx+gw < bx+bw:
            inside_B = True
        else:
            inside_B = False
        
        
        #원통이 빨간 원 안에 있으면 마지막 자리 0, 아니면 1
        if inside_R:
            ras_res += ",0"
        #elif (inside_R == False) and (inside_B == False):
        #    ras_res += ",1/"
        #elif (inside_R == False) and (inside_B == True):
        #    ras_res += ",2/"
        #else:
        #    ras_res += ",3/"
        else:
            ras_res += ",1"
        #print(ras_res)
        
        if inside_B:
            ras_res += ",0/"
        else:
            ras_res += ",1/"
        '''
        ras_res += str(mean_Area)
        ras_res += "/"
        '''
        #servo_move(ang)
        #print(ras_res)
        
        ser.write(ras_res.encode())


        #cv2.imshow('frame', frame)
        cv2.imshow('dst', bin_image4)
        
        
        if cv2.waitKey(1) & 0xFF == 27:
            break
        
        end = time.perf_counter()
        sec = end-start
        fps = 1/sec
        FPS.append(fps)
        count=count+1
        #if((count%5)==0):
        #    print("size : ", max_areaG, inside_R)
        
        if(count==50):
            print("평균 fps: ", np.mean(FPS), "size : ", max_areaG, inside_R, inside_B)
            #print(ras_res)
            FPS=[]
            count=0
        
        '''
        end2 = time.time()
        print(end-start)
        start = end
        '''  
        #prevTime = curTime
    except:
        cap.release()
        while True:
            cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
            ret,frame = cap.read()
            if ret:
                break
cap.release()
cv2.destroyAllWindows()