#include "mbed.h"
#include "MPU9250.h"

MPU9250 mpu9250;

// main() runs in its own thread in the OS

Timer timer;
float a = 0;

int main()
{
    mpu9250.resetMPU9250(); // Reset registers to default in preparation for device calibration
    mpu9250.MPU9250SelfTest(SelfTest); // Start by performing self test and reporting values 
    mpu9250.calibrateMPU9250(gyroBias, accelBias); // Calibrate gyro and accelerometers, load biases in bias registers  
    // //ThisThread::sleep_for(100);
    mpu9250.initMPU9250(); 

    // //
    // //whoami = mpu9250.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    // //pc.printf("I AM 0x%x\t", whoami); pc.printf("I SHOULD BE 0x48\n\r");

    // mpu9250.getAres(); // Get accelerometer sensitivity +-2g 4g 8g
    // mpu9250.getGres(); // Get gyro sensitivity      250  500   1000 


    // while (true) {
    //         timer.start();
    //         mpu9250.get_data();ar
    //         pc.printf("%.1f %.1f \n",roll_p,pitch_p);
    //         ///타이머 시간 3m/s 측정
    //         while(true)
    //             {
    //                 if((a = timer.read_ms()) >= deltat)
    //                     break;
    //             }
    //         timer.reset();
    //         timer.stop();
    // }
}

