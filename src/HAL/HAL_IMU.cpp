#include "HAL/HAL.h"
// #include "MPU9250.h"
// #include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "App/Accounts/Account_Master.h"
#include <stdlib.h>
#include "MPU6050.h"

// MPU6050 mpu;//mpu(0x69);
// bool dmpReady = false;
// uint8_t fifoBuffer[64]; 
// uint8_t mpuIntStatus;  
// uint8_t devStatus;     
// uint16_t packetSize;   
// uint16_t fifoCount;    

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

void HAL::IMU_Init()
{
    // Wire.begin();
    // Wire.setClock(400000); 
    // mpu.initialize();
    // Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    // devStatus = mpu.dmpInitialize();
    // mpu.setXGyroOffset(220);
    // mpu.setYGyroOffset(76);
    // mpu.setZGyroOffset(-85);
    // mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // if (devStatus == 0) {
    //     // Calibration Time: generate offsets and calibrate our MPU6050
    //     mpu.CalibrateAccel(6);
    //     mpu.CalibrateGyro(6);
    //     mpu.PrintActiveOffsets();
    //     // turn on the DMP, now that it's ready
    //     Serial.println(F("Enabling DMP..."));
    //     mpu.setDMPEnabled(true);

    //     // enable Arduino interrupt detection
    //     Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    //     Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    //     Serial.println(F(")..."));
    //     attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    //     mpuIntStatus = mpu.getIntStatus();

    //     // set our DMP Ready flag so the main loop() function knows it's okay to use it
    //     Serial.println(F("DMP ready! Waiting for first interrupt..."));
    //     dmpReady = true;

    //     // get expected DMP packet size for later comparison
    //     packetSize = mpu.dmpGetFIFOPacketSize();
    // } else {
    //     // ERROR!
    //     // 1 = initial memory load failed
    //     // 2 = DMP configuration updates failed
    //     // (if it's going to break, usually the code will be 1)
    //     Serial.print(F("DMP Initialization failed (code "));
    //     Serial.print(devStatus);
    //     Serial.println(F(")"));
    // }
    accelgyro.initialize();
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

}

void HAL::IMU_Update()
{
    IMU_Info_t imuInfo;
    // imuInfo.ax = rand() % 1000 - 500;
    // imuInfo.ay = rand() % 1000 - 500;
    // imuInfo.az = rand() % 1000 - 500;
    // imuInfo.gx = rand() % 1000 - 500;
    // imuInfo.gy = rand() % 1000 - 500;
    // imuInfo.gz = rand() % 1000 - 500;
    // imuInfo.mx = rand() % 1000 - 500;
    // imuInfo.my = rand() % 1000 - 500;
    // imuInfo.mz = rand() % 1000 - 500;
    // imuInfo.roll= rand() % 1000 - 500;
    // imuInfo.yaw =rand() % 1000 - 500;
    // imuInfo.pitch=rand() % 1000 - 500;

//    accelgyro.getMotion6(
//            &imuInfo.ax,
//            &imuInfo.ay,
//            &imuInfo.az,
//            &imuInfo.gx,
//            &imuInfo.gy,
//            &imuInfo.gz
//    );
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    imuInfo.ax = ax;
    imuInfo.ay = ay;
    imuInfo.az = az;
    imuInfo.gx = gx;
    imuInfo.gy = gy;
    imuInfo.gz = gz;
    imuInfo.mx = 0;
    imuInfo.my = 0;
    imuInfo.mz = 0;
    imuInfo.roll= 0;
    imuInfo.yaw =0;
    imuInfo.pitch=0;

    AccountSystem::IMU_Commit(&imuInfo);

    // if (!dmpReady) return;
    // if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    //      mpu.dmpGetQuaternion(&q, fifoBuffer);
    //         mpu.dmpGetGravity(&gravity, &q);
    //         mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    //         Serial.print("ypr\t");
    //         Serial.print(ypr[0] * 180/M_PI);
    //         Serial.print("\t");
    //         Serial.print(ypr[1] * 180/M_PI);
    //         Serial.print("\t");
    //         Serial.println(ypr[2] * 180/M_PI); 
    //     }
}
