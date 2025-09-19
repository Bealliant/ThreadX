//
// Created by a2012 on 10/9/2025.
//

#ifndef THREADX_PERIPH_MPU6050_H
#define THREADX_PERIPH_MPU6050_H
#include "stm32f10x.h"

#define mpu6050_addr0 0x00

/**
 * @brief: MPU6050 Register Map
 */
typedef enum {
    mpu6050_sample_rate = 0x19,
    mpu6050_gyro_config = 0x1B,
    mpu6050_accel_config = 0x1C,
    mpu6050_power_mgmt_1 = 0x6B,
    mpu6050_pwr_mgmt_2 = 0x6C,
    mpu6050_usr_ctrl = 0x6A,
    mpu6050_who_am_i = 0x75,
    mpu6050_accel_xout_h = 0x3B,
    mpu6050_accel_xout_l = 0x3C,
    mpu6050_accel_yout_h = 0x3D,
    mpu6050_accel_yout_l = 0x3E,
    mpu6050_accel_zout_h = 0x3F,
    mpu6050_accel_zout_l = 0x40,
    mpu6050_temp_out_h = 0x41,
    mpu6050_temp_out_l = 0x42,
    mpu6050_gyro_xout_h = 0x43,
    mpu6050_gyro_xout_l = 0x44,
    mpu6050_gyro_yout_h = 0x45,
    mpu6050_gyro_yout_l = 0x46,
    mpu6050_gyro_zout_h = 0x47,
    mpu6050_gyro_zout_l = 0x48,
} mpu6050_reg;

#define mpu6050_gyro_250d 0x00
#define mpu6050_gyro_500d 0x08
#define mpu6050_gyro_1000d 0x10
#define mpu6050_gyro_2000d 0x18

#define mpu6050_gyro_range mpu6050_gyro_250d
#if (mpu6050_gyro_range == mpu6050_gyro_250d)
    #define mpu6050_gyro_scale_setting 0x00
    #define mpu6050_gyro_sensitivity 131.0
#elif (mpu6050_gyro_range == mpu6050_gyro_500d)
    #define mpu6050_gyro_scale_setting 0x08
    #define mpu6050_gyro_sensitivity 65.5
#elif (mpu6050_gyro_range == mpmpu6050_gyro_1000d)
    #define mpu6050_gyro_scale_setting 0x10
    #define mpu6050_gyro_sensitivity 32.8
#elif (mpu6050_gyro_range == mpu6050_gyro_2000d)
    #define mpu6050_gyro_scale_setting 0x18
    #define mpu6050_gyro_sensitivity 16.4
#endif


#define mpu6050_accel_2g 0x00
#define mpu6050_accel_4g 0x08
#define mpu6050_accel_8g 0x10
#define mpu6050_accel_16g 0x18

#define mpu6050_accel_range mpu6050_accel_4g

#if (mpu6050_accel_range == mpu6050_accel_2g)
    #define mpu6050_accel_scale_setting 0x00
    #define mpu6050_accel_sensitivity 16384.0
#elif (mpu6050_accel_range == mpu6050_accel_4g)
    #define mpu6050_accel_scale_setting 0x08
    #define mpu6050_accel_sensitivity   8192.0
#elif (mpu6050_accel_range == mpu6050_accel_8g)
    #define mpu6050_accel_scale_setting 0x10
    #define mpu6050_accel_sensitivity   4096.0
#elif (mmpu6050_accel_range == mpu6050_accel_16g)
    #define mpu6050_accel_scale_setting 0x18
    #define mpu6050_accel_sensitivity   2048.0
#endif


#define mpu6050_temp_sensitivity 340.0
#define mpu6050_temp_offset 36.53

typedef enum{
    mpu6050_addr_write = (0b1101000|mpu6050_addr0)<<1,
    mpu6050_addr_read =(0b1101000|mpu6050_addr0)<<1 | 0x01
}mpu6050_address;

typedef struct {
    double temperature;
    double accel_x;
    double accel_y;
    double accel_z;
    double gyro_x;
    double gyro_y;
    double gyro_z;
}mpu6050_info;



void mpu6050_init(void);
void mpu6050_get_data(mpu6050_info* data);
void mpu6050_info_print(mpu6050_info* data);
#endif //THREADX_PERIPH_MPU6050_H

