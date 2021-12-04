#ifndef __KALMAN_H__
#define __KALMAN_H__
#include "common_macro.h"
#include "rtthread.h"
using namespace rtthread;

class kalman_filter
{
public:
    float Gyro_x = 0;
    float Angle = 0;
    // int flag_zero=0;

    ///////////////////////////////////////////////
    float Q_angle = 0.1, Q_gyro = 1, R_angle = 0.001;
    //float Q_angle=0.50, Q_gyro=0.070, R_angle=4;
    //float Q_angle=0.2, Q_gyro=0.1, R_angle=0.3;
    //float Q_angle=0.02, Q_gyro=0.01, R_angle=0.03;
    //float Q_angle=0.001, Q_gyro=0.1, R_angle=10;
    // float Q_angle = 0.0001, Q_gyro = 0.0001, R_angle = 10;

    float Dt = 0.001;
    float C_0 = 1.0;
    float Q_bias = 0, Angle_err = 0;
    float PCt_0 = 0.0, PCt_1 = 0, E = 0.0;
    float K_0 = 0.0, K_1 = 0.0, t_0 = 0.0, t_1 = 0.0;
    float Pdot[4] = {0, 0, 0, 0};
    float PP[2][2] = {{1.0, 0}, {0, 1.0}};
    ///////////////////////////////////////////////
    float gyro_z_Q = 50;
    float gyro_z_R = 100;
    float filtered_gyro_z = 0;
    ///////////////////////////////////////////////
    float ACC_Z_Q = 10;
    float ACC_Z_R = 200;
    float filtered_acc_z = 0;
    ///////////////////////////////////////////////

    kalman_filter() {}

    void filter(float Gyro, float Accel)
    {
        Angle += (Gyro - Q_bias) * Dt;

        Pdot[0] = Q_angle - PP[0][1] - PP[1][0];

        Pdot[1] = -PP[1][1];
        Pdot[2] = -PP[1][1];
        Pdot[3] = Q_gyro;

        PP[0][0] += Pdot[0] * Dt;
        PP[0][1] += Pdot[1] * Dt;
        PP[1][0] += Pdot[2] * Dt;
        PP[1][1] += Pdot[3] * Dt;

        Angle_err = Accel - Angle;

        PCt_0 = C_0 * PP[0][0];
        PCt_1 = C_0 * PP[1][0];

        E = R_angle + C_0 * PCt_0;

        K_0 = PCt_0 / E;
        K_1 = PCt_1 / E;

        t_0 = PCt_0;
        t_1 = C_0 * PP[0][1];

        PP[0][0] -= K_0 * t_0;
        PP[0][1] -= K_0 * t_1;
        PP[1][0] -= K_1 * t_0;
        PP[1][1] -= K_1 * t_1;

        Angle += K_0 * Angle_err;
        Q_bias += K_1 * Angle_err;
        Gyro_x = Gyro - Q_bias;
    }

    float gyro_z_filter(float signal)
    {
        static float nowdata_p = 20;
        float nowdata;
        float kg = 0.1;
        /*
        Q:过程噪声,Q增大,动态响应变快，收敛稳定性变坏
        R:测量噪声,R增大，动态响应变慢，收敛稳定性变好
	    */
        /*预测*/
        nowdata = filtered_gyro_z;
        /*协方差*/
        nowdata_p = nowdata_p + gyro_z_Q;
        /*卡尔曼增益*/
        kg = nowdata_p / (nowdata_p + gyro_z_R);
        /*最优解*/
        filtered_gyro_z = nowdata + kg * (signal - nowdata);
        /*更新协方差*/
        nowdata_p = (1 - kg) * nowdata_p;
        return filtered_gyro_z;
    }

    float acc_z_filter(float signal)
    {
        static float nowdata_p = 20;
        float nowdata;
        float kg = 0.1;

        /*预测*/
        nowdata = filtered_acc_z;
        /*协方差*/
        nowdata_p = nowdata_p + ACC_Z_Q;
        /*卡尔曼增益*/
        kg = nowdata_p / (nowdata_p + ACC_Z_R);
        /*最优解*/
        filtered_acc_z = nowdata + kg * (signal - nowdata);
        /*更新协方差*/
        nowdata_p = (1 - kg) * nowdata_p;
        return filtered_acc_z;
    }
};

#endif
