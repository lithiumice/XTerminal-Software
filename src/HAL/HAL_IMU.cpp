#include "HAL/HAL_IMU.h"
#include "App/Accounts/Account_Master.h"
#include <stdlib.h>
#include "MPU6050.h"
#include <MadgwickAHRS.h>

MPU6050 mpu;
Madgwick filter;
int16_t ax, ay, az;
int16_t gx, gy, gz;
HAL::IMU_Info_t imuInfo;


ACTIVE_TYPE mpu_action = ACTIVE_TYPE_MAX;
uint8_t mpu_act_valid = 0;
// quad_num quad_n;
// _imu_st imu_data = {1, 0, 0, 0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0, 0};
// float acc_x_filter = 0;
// float acc_y_filter = 0;
// float acc_z_filter = 0;
// float gyro_x_filter = 0;
// float gyro_z_filter = 0;
// float gyro_y_filter = 0;

float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}

void mpu_action_judge()
{
    if (!mpu_act_valid)
    {
        if (imuInfo.ay > 0.244)//4000
        {
            mpu_act_valid = 1;
            mpu_action = TURN_LEFT;
        }
        else if (imuInfo.ay < - 0.244)
        {
            mpu_act_valid = 1;
            mpu_action = TURN_RIGHT;
        }
        else if (imuInfo.ax > 0.3)//0.3 --> 5000
        {
            mpu_act_valid = 1;
            mpu_action = TURN_UP;
            // delay(500);
            // mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
            // if (ax > 5000)
            // {
            //     mpu_act_valid = 1;
            //     mpu_action = FORWORD;
            // }
        }
        else if (imuInfo.ax < -0.3)
        {
            mpu_act_valid = 1;
            mpu_action = TURN_DOWN;
            // delay(500);
            // mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
            // if (ax < -5000)
            // {
            //     mpu_act_valid = 1;
            //     mpu_action = BACKWARD;
            // }
        }
        else
        {
            mpu_act_valid = 0;
            mpu_action = TURN_NONE;
        }
    }
}

// void compute_quad()
// {
//     float acc_x_filter  = imuInfo.ax*1000;
//     float acc_y_filter  = imuInfo.ay*1000;
//     float acc_z_filter  = imuInfo.az*1000;
//     float gyro_x_filter = imuInfo.gx*1000;
//     float gyro_z_filter = imuInfo.gy*1000;
//     float gyro_y_filter = imuInfo.gz*1000;

//     quad_n.IMUupdate(
//         IMUConverter * gyro_x_filter,
//         IMUConverter * gyro_y_filter,
//         IMUConverter * gyro_z_filter,
//         acc_x_filter,
//         acc_y_filter,
//         acc_z_filter,
//         &imu_data);

// }

void HAL::IMU_Init()
{
#ifdef BOARD_WROOM32
#else
    mpu.initialize();
    filter.begin(50);

    Serial.println("Testing device connections...");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // HAL::config_mpu_load();
    // mpu.setXGyroOffset(HAL::config.mpu_config.x_gyro_offset);
    // mpu.setYGyroOffset(HAL::config.mpu_config.y_gyro_offset);
    // mpu.setZGyroOffset(HAL::config.mpu_config.z_gyro_offset);
    // mpu.setXAccelOffset(HAL::config.mpu_config.x_accel_offset);
    // mpu.setYAccelOffset(HAL::config.mpu_config.y_accel_offset);
    // mpu.setZAccelOffset(HAL::config.mpu_config.z_accel_offset);
#endif
}

void HAL::IMU_Calibrate()
{
    mpu.CalibrateAccel(7);
    mpu.CalibrateGyro(7);
    mpu.PrintActiveOffsets();

    HAL::config.mpu_config.x_gyro_offset = mpu.getXGyroOffset();
    HAL::config.mpu_config.y_gyro_offset = mpu.getYGyroOffset();
    HAL::config.mpu_config.z_gyro_offset = mpu.getZGyroOffset();
    HAL::config.mpu_config.x_accel_offset = mpu.getXAccelOffset();
    HAL::config.mpu_config.y_accel_offset = mpu.getYAccelOffset();
    HAL::config.mpu_config.z_accel_offset = mpu.getZAccelOffset();
    HAL::config_mpu_save();

    HAL::TerminalPrintln("mpu calibrate done!");
}

void HAL::IMU_Update()
{
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    ax = convertRawAcceleration(ax);
    ay = convertRawAcceleration(ay);
    az = convertRawAcceleration(az);
    gx = convertRawGyro(gx);
    gy = convertRawGyro(gy);
    gz = convertRawGyro(gz);

    imuInfo.ax = (float)ax;
    imuInfo.ay = (float)ay;
    imuInfo.az = (float)az;
    imuInfo.gx = (float)gx;
    imuInfo.gy = (float)gy;
    imuInfo.gz = (float)gz;
    imuInfo.mx = 0;
    imuInfo.my = 0;
    imuInfo.mz = 0;

    filter.updateIMU(gx, gy, gz, ax, ay, az);

    imuInfo.roll = filter.getRoll();
    imuInfo.pitch = filter.getPitch();
    imuInfo.yaw = filter.getYaw();

    if(imuInfo.roll<0)
        imuInfo.roll += 180;
    else
        imuInfo.roll -= 180;

    mpu_action_judge();

    // compute_quad();
    // imuInfo.roll = imu_data.rol;
    // imuInfo.yaw = imu_data.yaw;
    // imuInfo.pitch = imu_data.pit;
    // if (commit)
    // AccountSystem::IMU_Commit(&imuInfo);
}

/**
 * 
MPU6050 mpu;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];

void setup()
{
  Serial.begin(115200);
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.CalibrateAccel(6);
  mpu.CalibrateGyro(6);
  mpu.PrintActiveOffsets();
  mpu.setDMPEnabled(true);
}

void loop()
{
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
  {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180 / M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180 / M_PI);
    Serial.print("\t");
    Serial.println(ypr[2] * 180 / M_PI);
  }
}
 *
 */