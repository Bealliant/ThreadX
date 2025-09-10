//
// Created by a2012 on 3/5/2025.
//
#include "bsp_i2c.h"

/**
 * @brief - to Activate the GPIO Clock Source, GPIO Pin for Soft i2c using
 * @param - None
 * @return - None
 */
void bsp_soft_i2c_init(){
    // ENABLE the APB2 Clock Peripheral of GPIOx.
    RCC_APB2PeriphClockCmd(SOFT_I2C_RCCPeriph,ENABLE);

    GPIO_InitTypeDef soft_i2c_sda_gpioinit = {
            .GPIO_Mode = GPIO_Mode_Out_OD,
            .GPIO_Speed = GPIO_Speed_10MHz,
            .GPIO_Pin = SOFT_I2C_SDA
    };
    GPIO_Init(SOFT_I2C_Port,&soft_i2c_sda_gpioinit);
    GPIO_InitTypeDef soft_i2c_scl_gpioinit = {
            .GPIO_Mode = GPIO_Mode_Out_OD,
            .GPIO_Pin = SOFT_I2C_SCL,
            .GPIO_Speed = GPIO_Speed_10MHz
    };
    GPIO_Init(SOFT_I2C_Port,&soft_i2c_scl_gpioinit);
}

///@brief: to start the i2c transmission session.
///@param: None
///@retval None
void bsp_soft_i2c_start(){
    // pull the SCL and SDA high.
    i2c_scl_latch(1);
    i2c_sda_latch(1);
    __NOP();
    // when SCL is high, pull SDA from high to low
    i2c_sda_latch(0);
    i2c_scl_latch(0);
    // After Starting the session both SCL and SDA are pulled low.
}

/**
 * @brief to send the data after starting i2c session
 * @param addr: the combination of 7-bit address and read/write flag
 * @param data: is the data sent from master to slave
 * @retval None
*/
void bsp_soft_i2c_byte(uint8_t byte){
    static uint8_t temp_bit;
    for (uint8_t i=0; i<7; i++){
        i2c_sda_latch(byte&(0x80>>i));
        i2c_scl_latch(1);
        i2c_scl_latch(0);
    }
    // ACK, SDA is pulled high
    i2c_sda_latch(1);
    i2c_scl_latch(1);
    __NOP();
    i2c_scl_latch(0);
}

/**
 * @brief: to end the i2c transmission session.
 * @param None
 * @retval None
 * */
void bsp_soft_i2c_end(){
    i2c_scl_latch(1);
    // when an i2c session ends, during high period of SCL, SDA is pulled low.
    i2c_sda_latch(0);
    i2c_scl_latch(0);
}

void bsp_soft_i2c_send(uint8_t addr, uint8_t data){
    bsp_soft_i2c_start();
    bsp_soft_i2c_byte(addr);
    bsp_soft_i2c_byte(data);
    bsp_soft_i2c_end();
}
