#ifndef __QUAD_NUM_H__
#define __QUAD_NUM_H__

#include "Arduino.h"
#include "math.h"

#define my_pow(a) ((a) * (a))
#define halfT 0.010f // 采样周期的一半，用于求解四元数微分方程时计算角增量
// #define halfT 0.0005f // 采样周期的一半，用于求解四元数微分方程时计算角增量
#define IMUConverter 0.000532f

typedef struct
{
    float lpf_1;
    float out;
} _lf_t;

typedef struct
{
    float x;
    float y;
    float z;
} _xyz_f_st;

typedef struct
{
    float w; //q0;
    float x; //q1;
    float y; //q2;
    float z; //q3;

    _xyz_f_st x_vec;
    _xyz_f_st y_vec;
    _xyz_f_st z_vec;

    _xyz_f_st a_acc;
    _xyz_f_st w_acc;

    float rol;
    float pit;
    float yaw;
    float inter_rol;
    float inter_pit;
    float inter_yaw;
} _imu_st;

class quad_num
{
    public:
    _lf_t err_lf_x, err_lf_y, err_lf_z;
    _xyz_f_st vec_err_i;
    float Accel_magnitude;
    float kp = 1, ki = 0;

    quad_num() {}
    ~quad_num() {}

    void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, _imu_st *imu)
    {
        float q0q1, q0q2, q1q1, q1q3, q2q2, q2q3, q3q3, q1q2, q0q3;
        float w_q, x_q, y_q, z_q;
        float acc_length, q_length;
        _xyz_f_st acc_norm;
        _xyz_f_st vec_err;
        _xyz_f_st d_angle;

        if (millis() < 10000)
        {
            kp = 10;
            ki = 1;
        }
        else
        {
            kp = 1.0;
            ki = 0;
        }

        w_q = imu->w;
        x_q = imu->x;
        y_q = imu->y;
        z_q = imu->z;

        //		q0q0 = w_q * w_q;
        q0q1 = w_q * x_q;
        q0q2 = w_q * y_q; //pitch
        q1q1 = x_q * x_q;
        q1q3 = x_q * z_q; //pitch
        q2q2 = y_q * y_q;
        q2q3 = y_q * z_q;
        q3q3 = z_q * z_q;
        q1q2 = x_q * y_q;
        q0q3 = w_q * z_q;

        Accel_magnitude = sqrt(ax * ax + ay * ay + az * az);
        Accel_magnitude = Accel_magnitude / 16384.0f; // Scale to gravity.

        // 加速度计的读数，单位化。
        acc_length = sqrt(my_pow(ax) + my_pow(ay) + my_pow(az));
        acc_norm.x = ax / acc_length;
        acc_norm.y = ay / acc_length;
        acc_norm.z = az / acc_length;

        // 载体坐标下的x方向向量，单位化。
        imu->x_vec.x = 1 - (2 * q2q2 + 2 * q3q3);
        imu->x_vec.y = 2 * q1q2 - 2 * q0q3;
        imu->x_vec.z = 2 * q1q3 + 2 * q0q2;

        // 载体坐标下的y方向向量，单位化。
        imu->y_vec.x = 2 * q1q2 + 2 * q0q3;
        imu->y_vec.y = 1 - (2 * q1q1 + 2 * q3q3);
        imu->y_vec.z = 2 * q2q3 - 2 * q0q1;

        // 载体坐标下的z方向向量（等效重力向量、重力加速度向量），单位化。
        imu->z_vec.x = 2 * q1q3 - 2 * q0q2;
        imu->z_vec.y = 2 * q2q3 + 2 * q0q1;
        imu->z_vec.z = 1 - (2 * q1q1 + 2 * q2q2);

        // 计算载体坐标下的运动加速度。(与姿态解算无关)
        imu->a_acc.x = ax - 9800 * imu->z_vec.x;
        imu->a_acc.y = ay - 9800 * imu->z_vec.y;
        imu->a_acc.z = az - 9800 * imu->z_vec.z;

        // 计算世界坐标下的运动加速度。(与姿态解算无关)
        imu->w_acc.x = imu->x_vec.x * imu->a_acc.x + imu->x_vec.y * imu->a_acc.y + imu->x_vec.z * imu->a_acc.z;
        imu->w_acc.y = imu->y_vec.x * imu->a_acc.x + imu->y_vec.y * imu->a_acc.y + imu->y_vec.z * imu->a_acc.z;
        imu->w_acc.z = imu->z_vec.x * imu->a_acc.x + imu->z_vec.y * imu->a_acc.y + imu->z_vec.z * imu->a_acc.z;
        // 测量值与等效重力向量的叉积（计算向量误差）。
        vec_err.x = (acc_norm.y * imu->z_vec.z - imu->z_vec.y * acc_norm.z);  // * Accel_magnitude;
        vec_err.y = -(acc_norm.x * imu->z_vec.z - imu->z_vec.x * acc_norm.z); // * Accel_magnitude;
        vec_err.z = -(acc_norm.y * imu->z_vec.x - imu->z_vec.y * acc_norm.x); // * Accel_magnitude;

        err_lf_x.out = vec_err.x;
        err_lf_y.out = vec_err.y;
        err_lf_z.out = vec_err.z;

        //误差积分
        vec_err_i.x += err_lf_x.out * halfT * 2 * ki;
        vec_err_i.y += err_lf_y.out * halfT * 2 * ki;
        vec_err_i.z += err_lf_z.out * halfT * 2 * ki;

        // 构造增量旋转（含融合纠正）。
        d_angle.x = (gx + (err_lf_x.out + vec_err_i.x) * kp) * halfT * 2 / 2;
        d_angle.y = (gy + (err_lf_y.out + vec_err_i.y) * kp) * halfT * 2 / 2;
        d_angle.z = (gz + (err_lf_z.out + vec_err_i.z) * kp) * halfT * 2 / 2;

        // 计算姿态。
        imu->w = w_q - x_q * d_angle.x - y_q * d_angle.y - z_q * d_angle.z;
        imu->x = w_q * d_angle.x + x_q + y_q * d_angle.z - z_q * d_angle.y;
        imu->y = w_q * d_angle.y - x_q * d_angle.z + y_q + z_q * d_angle.x;
        imu->z = w_q * d_angle.z + x_q * d_angle.y - y_q * d_angle.x + z_q;

        q_length = sqrt(imu->w * imu->w + imu->x * imu->x + imu->y * imu->y + imu->z * imu->z);
        imu->w /= q_length;
        imu->x /= q_length;
        imu->y /= q_length;
        imu->z /= q_length;

        imu->pit = asin(2 * q1q3 - 2 * q0q2) * 57.30f;
        imu->rol = atan2(2 * q2q3 + 2 * q0q1, -2 * q1q1 - 2 * q2q2 + 1) * 57.30f;
        imu->yaw = -atan2(2 * q1q2 + 2 * q0q3, -2 * q2q2 - 2 * q3q3 + 1) * 57.30f;
    }
};

#endif