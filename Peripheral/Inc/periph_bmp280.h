//
// Created by a2012 on 20/9/2025.
//

#include "stm32f10x.h"
#include "bsp_delay.h"
#include "bsp_i2c.h"
#ifndef THREADX_PERIPH_BMP280_H
#define THREADX_PERIPH_BMP280_H
#define USE_I2C

/**
 * @enum - bmp280_reg_t, the register map of BMP280 sensor
 * */
typedef enum{
    BMP280_reg_id = 0xD0,
    BMP280_reg_reset = 0xE0,
    BMP280_reg_status = 0xF3,
    BMP280_reg_ctrl_meas = 0xF4,
    BMP280_reg_config = 0xF5,
    BMP280_reg_rawdata = 0xF7,
    // when read the raw data, the register address will auto-increment, starting from 0xF7 to 0xFC.
}bmp280_reg_t;
/**
 * @enum bmp280_mode_t, to set the power mode of BMP280 sensor
 * */
typedef enum{
    BMP280_mode_sleep = 0b00,
    BMP280_mode_forced = 0b01,
    BMP280_mode_normal = 0b11,
}bmp280_mode_t;
/**
 * @enum - bmp280_osrs_t, to set the over-sampling rate of temperature and pressure measurement.
 * */
typedef enum{
    BMP280_osrs_skipped = 0b000,
    BMP280_osrs_x1 = 0b001,
    BMP280_osrs_x2 = 0b010,
    BMP280_osrs_x4 = 0b011,
    BMP280_osrs_x8 = 0b100,
    BMP280_osrs_x16 = 0b101,
}bmp280_osrs_t;
#define osrs_temp_offset 5
#define osrs_pres_offset 2

/**
 * @enum - bmp280_filter_t, to set the IIR filter coefficient
 * */
typedef enum{
    BMP280_filter_off = 0b000,
    BMP280_filter_x2 = 0b001,
    BMP280_filter_x4 = 0b010,
    BMP280_filter_x8 = 0b011,
    BMP280_filter_x16 = 0b100,
}bmp280_filter_t;
#define filter_t_offset 2
/**
 * @enum - bmp280_standby_t, to set the standby time in normal mode
 * */
typedef enum{
    BMP280_standby_0_5ms = 0b000,
    BMP280_standby_62_5ms = 0b001,
    BMP280_standby_125ms = 0b010,
    BMP280_standby_250ms = 0b011,
    BMP280_standby_500ms = 0b100,
    BMP280_standby_1000ms = 0b101,
    BMP280_standby_2000ms = 0b110,
    BMP280_standby_4000ms = 0b111,
}bmp280_standby_t;
#define standby_t_offset 5
#ifdef USE_I2C
#define BMP280_SDO 0
/**
 * @enum - bmp280_addr_t, the I2C address of BMP280 sensor when read and write
 * */
typedef enum{
    BMP280_addr_write = 0b11101100|(BMP280_SDO<<1),
    BMP280_addr_read  = 0b11101101|(BMP280_SDO<<1),
}bmp280_addr_t;

/**
 * @struct - bmp280_data_t, to store the compensated temperature and pressure data
 * */
typedef struct {
    double temperature;
    double pressure;
}bmp280_data_t;

/**
 * @struct - bmp280_calib_t, to store the calibration parameters read from the BMP280 sensor
 * */
typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
} bmp280_calib;


void bmp280_init(void);
#endif
#ifdef USE_SPI
// TODO: SPI implementation for BMP280 pressure sensor.
#endif
#endif //THREADX_PERIPH_BMP280_H

void bmp280_read_rawdata(bmp280_data_t* data);
void bmp280_info(bmp280_data_t* data);
