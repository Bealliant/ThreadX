//
// Created by a2012 on 20/9/2025.
//
#include "periph_bmp280.h"
#include "bsp_i2c.h"
#include "stdio.h"
#ifdef USE_I2C

bmp280_calib calib={
    .dig_T1=27504,
    .dig_T2=26435,
    .dig_T3=-1000,
    .dig_P1=36477,
    .dig_P2=-10685,
    .dig_P3=3024,
    .dig_P4=2855,
    .dig_P5=140,
    .dig_P6=-7,
    .dig_P7=15500,
    .dig_P8=-14600,
    .dig_P9=6000
};

/**
 * @brief - to Initialize the BMP280 sensor using I2C interface
 * @param - None
 * @return - None
 * @note - After initializing the I2C peripheral, it is necessary to configure the register config and ctrl_meas.
 */
void bmp280_init(void){
    Softi2c bmp280_i2c_initstruct = {
            .i2c_GPIOPort = GPIOA,
            .i2c_scl = GPIO_Pin_11,
            .i2c_sda = GPIO_Pin_12,
            .debug = DISABLE,
    };
    bsp_soft_i2c_init(&bmp280_i2c_initstruct);
    // configure the config register
    uint8_t init_command[] ={
            BMP280_reg_config,
            (BMP280_standby_62_5ms<<standby_t_offset)|(BMP280_filter_x16<<filter_t_offset)|0x00,
            BMP280_reg_ctrl_meas,
            (BMP280_osrs_x4<<osrs_temp_offset)|(BMP280_osrs_x1<<osrs_pres_offset)|BMP280_mode_normal
    };
    bsp_soft_i2c_array_write(BMP280_addr_write, init_command, sizeof(init_command)/ sizeof(init_command[0]));
}

#endif


static uint8_t bmp280_rawdata[6];
/**
 * @brief - to read the raw data of temperature and pressure from BMP280 sensor
 * @param -
 * @return - the chip id of BMP280 sensor, which should be 0x58
 * */
void bmp280_read_rawdata(bmp280_data_t* data){
    static int32_t temp_adc, pres_adc, var1, var2;
    static uint32_t p;
    bsp_soft_i2c_byte_write(BMP280_addr_write, BMP280_reg_rawdata); // set the current register address to 0xF7
    bsp_soft_i2c_array_read(BMP280_addr_read, bmp280_rawdata, 6);
    temp_adc = (int32_t)((((uint32_t)bmp280_rawdata[3])<<12) | (((uint32_t)bmp280_rawdata[4])<<4) | ((bmp280_rawdata[5])>>4));
    pres_adc = (int32_t)((((uint32_t)bmp280_rawdata[0])<<12) | (((uint32_t)bmp280_rawdata[1])<<4) | ((bmp280_rawdata[2])>>4));
    // utilize the fixed-point calculation algorithm for bmp280.
    var1 = ((((temp_adc>>3) - ((int32_t)calib.dig_T1 << 1))) * ((int32_t)calib.dig_T2)) >> 11;
    var2 = (((((temp_adc>>4) - ((int32_t)calib.dig_T1)) * ((temp_adc >> 4) - ((int32_t)calib.dig_T1))) >> 12) * ((int32_t)calib.dig_T3)) >> 14;
    int32_t t_fine = var1 + var2;
    data->temperature = (double)((t_fine * 5 + 128) >> 8) / 100.0;
    var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
    var2 = (((var1>>2) * (var1>>2)) >>11 ) * ((int32_t)calib.dig_P6);
    var2 = var2 + ((var1 * ((int32_t)calib.dig_P5)) << 1);
    var2 = (var2>>2) + (((int32_t)calib.dig_P4) << 16);
    var1 = (((calib.dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)calib.dig_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)calib.dig_P1)) >> 15);
    if (var1 == 0) {
        data->pressure = 0; // avoid exception caused by division by zero
        return;
    }
    p = (((uint32_t)(((int32_t)1048576) - pres_adc) - (var2 >> 12))) * 3125;
    if (p < 0x80000000) {
        p = (p << 1) / ((uint32_t)var1);
    } else {
        p = (p / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)calib.dig_P9) * ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(p>>2)) * ((int32_t)calib.dig_P8)) >> 13;
    p = (uint32_t)(((int32_t)p + ((var1 + var2 + calib.dig_P7) >> 4)));
    data->pressure = (double)p / 100.0; // Pa to hPa
}

/**
 * @brief: to print the temperature and pressure info of the sensor
 * */
void bmp280_info(bmp280_data_t* data){
    printf("Temperature:%.3f,  Pressure:%.2f\n",data->temperature, data->pressure);
}