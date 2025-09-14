//
// Created by a2012 on 3/5/2025.
//
#include "stm32f10x.h"
#ifndef F103VET6_BSP_I2C_H
#define F103VET6_BSP_I2C_H


/// Use this struct to initialize soft i2c peripherals
/**
 * @brief: A soft_i2c configuration struct for initialization
 * @
 * */
typedef struct Softiic {
    GPIO_TypeDef* i2c_GPIOPort;
    uint16_t i2c_scl;
    uint16_t i2c_sda;
    FunctionalState debug;  // if debug is turned on, gpio mode will set to `PP` instead of `OD`
} Softi2c;

static void i2c_scl_drive(uint8_t dat);
static void i2c_sda_drive(uint8_t dat);
static void sda_mode_switch(uint8_t mode);

void bsp_soft_i2c_init(Softi2c* softi2c_initstruct);
void bsp_soft_i2c_start();
void bsp_soft_i2c_send(uint8_t byte);
void bsp_soft_i2c_end();
void bsp_soft_i2c_array_write(uint8_t device, const uint8_t* data, uint8_t length);
void bsp_soft_i2c_array_read(uint8_t device, uint8_t* data, uint8_t length);
static void i2c_byte_read(uint8_t* pos, FunctionalState ack);
void bsp_soft_i2c_byte_write(uint8_t device, uint8_t data);
void bsp_soft_i2c_reg8byte(uint8_t device, uint8_t reg, uint8_t data);
void bsp_soft_i2c_reg8array(uint8_t device, uint8_t reg, const uint8_t* data, uint8_t length);

#endif //F103VET6_BSP_I2C_H
