//
// Created by a2012 on 3/5/2025.
//
#include "stm32f10x.h"
#ifndef BSP_I2C_H
#define BSP_I2C_H

#define SOFT_I2C_Port GPIOB
#define SOFT_I2C_SDA GPIO_Pin_6
#define SOFT_I2C_SCL GPIO_Pin_7

#define SOFT_I2C_RCCPeriph (SOFT_I2C_Port==GPIOA?RCC_APB2Periph_GPIOA: \
(SOFT_I2C_Port==GPIOB)?RCC_APB2Periph_GPIOB:                           \
(SOFT_I2C_Port==GPIOC)?RCC_APB2Periph_GPIOC:                           RCC_APB2Periph_GPIOD) \

void bsp_soft_i2c_init(void);
void bsp_soft_i2c_start();
void bsp_soft_i2c_byte(uint8_t byte);
void bsp_soft_i2c_end();
void bsp_soft_i2c_send(uint8_t addr, uint8_t data);

/// @brief: to latch a bit in SCL line
/// @param: dat can be either 0 or 1
inline void i2c_scl_latch(uint8_t dat){
    SOFT_I2C_Port->BSRR|=(SOFT_I2C_SCL<<((1-dat)*16));
}
/// @brief: to latch a bit in SDA line
/// @param: dat can be either 0 or 1
inline void i2c_sda_latch(uint8_t dat){
    SOFT_I2C_Port->BSRR|=(SOFT_I2C_SDA<<((1-dat)*16));
}

#endif
