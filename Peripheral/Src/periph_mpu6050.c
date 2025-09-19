//
// Created by a2012 on 10/9/2025.
//
#include "periph_mpu6050.h"
#include "bsp_i2c.h"
#include "bsp_delay.h"
#include "stdio.h"

/**
 * @brief: Initialize the mpu6050 device upon power on.
 * @note: the register to set when power on:
 *  1. power_mgmt_1: set to 0x00 to wake up the device
 *  2. sample_rate: set to 0x07 to set the sample rate
 *  3. gyro_config: set to 0x18 to set the gyro full scale range to ±2000°/s
 *      GYRO_CONFIG FS_SEL[1:0] | Full Scale Range
 *      -------------------------|-----------------
 *               00            | ±250°/s
 *               01            | ±500°/s
 *               10            | ±1000°/s
 *               11            | ±2000°/s
 *      -------------------------|-----------------
 *  4. accel_config: set to 0x01 to set the accel full scale range to ±8g
 *      ACCEL_CONFIG AFS_SEL[1:0] | Full Scale Range
 *      -------------------------|-----------------
 *               00            | ±2g
 *               01            | ±4g
 *               10            | ±8g
 *               11            | ±16g
 *      -------------------------|-----------------
 *  5. usr_ctrl: set to 0x00 to disable the i2c master mode
 *  6. pwr_mgmt_2: set to 0x00 to enable all sensors
 * */
void mpu6050_init(){
    Softi2c i2c_initstruct = {
        .i2c_GPIOPort = GPIOB,
        .i2c_scl = GPIO_Pin_6,
        .i2c_sda = GPIO_Pin_7,
        .debug = DISABLE
    };
    bsp_soft_i2c_init(&i2c_initstruct);
    bsp_soft_i2c_byte_write(0x00,0x00);
    bsp_soft_i2c_reg8byte(mpu6050_addr_write,mpu6050_power_mgmt_1,0x00);
    bsp_soft_i2c_reg8byte(mpu6050_addr_write,mpu6050_sample_rate,0x07);
    bsp_soft_i2c_reg8byte(mpu6050_addr_write,mpu6050_gyro_config,mpu6050_gyro_scale_setting);
    bsp_soft_i2c_reg8byte(mpu6050_addr_write,mpu6050_accel_config,mpu6050_accel_scale_setting);
    bsp_soft_i2c_reg8byte(mpu6050_addr_write,mpu6050_usr_ctrl,0x00);
    bsp_soft_i2c_reg8byte(mpu6050_addr_write,mpu6050_pwr_mgmt_2,0x00);
}

/**
    * @brief: to read the current value of the accelerometer, gyroscope and temperature sensor
    * @
 */
void mpu6050_get_data(mpu6050_info* data){
    static uint8_t raw_data[14];
    static int16_t _accel_x, _accel_y, _accel_z, _gyro_x, _gyro_y, _gyro_z;
    static int16_t _temp;
    bsp_soft_i2c_byte_write(mpu6050_addr_write,mpu6050_accel_xout_h);
    Delay_ms(2);
    bsp_soft_i2c_array_read(mpu6050_addr_read,raw_data,14);
    _accel_x = (int16_t)(raw_data[0]<<8 | raw_data[1]);
    _accel_y = (int16_t)(raw_data[2]<<8 | raw_data[3]);
    _accel_z = (int16_t)(raw_data[4]<<8 | raw_data[5]);
    _temp    = (int16_t)(raw_data[6]<<8 | raw_data[7]);
    _gyro_x  = (int16_t)(raw_data[8]<<8 | raw_data[9]);
    _gyro_y  = (int16_t)(raw_data[10]<<8 | raw_data[11]);
    _gyro_z  = (int16_t)(raw_data[12]<<8 | raw_data[13]);
    data->accel_x = (double)_accel_x/mpu6050_accel_sensitivity;
    data->accel_y = (double)_accel_y/mpu6050_accel_sensitivity;
    data->accel_z = (double)_accel_z/mpu6050_accel_sensitivity;
    data->temperature = (double)_temp/mpu6050_temp_sensitivity + mpu6050_temp_offset;
    data->gyro_x = (double)_gyro_x/mpu6050_gyro_sensitivity;
    data->gyro_y = (double)_gyro_y/mpu6050_gyro_sensitivity;
    data->gyro_z = (double)_gyro_z/mpu6050_gyro_sensitivity;
}

//TODO: MPU6050 FIFO Interrupt and DMP
//TODO: switch between soft i2c and STM32 hardware i2c, DMA in the future.
void mpu6050_info_print(mpu6050_info* data){
    printf("Temp: %.2f C\r\n",data->temperature);
    printf("Accel: X: %.2f g, Y: %.2f g, Z: %.2f g\r\n",data->accel_x,data->accel_y,data->accel_z);
    printf("Gyro:  X: %.2f d/s, Y: %.2f d/s, Z: %.2f d/s\r\n",data->gyro_x,data->gyro_y,data->gyro_z);
}
