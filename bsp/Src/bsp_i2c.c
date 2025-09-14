//
// Created by a2012 on 3/5/2025.
//
#include "bsp_i2c.h"
#include "bsp_delay.h"

static GPIO_TypeDef* gpiox;
static uint16_t scl, sda;
static uint8_t debug_flag;

/**
 * @brief - to Activate the GPIO Clock Source, GPIO Pin for Soft i2c using
 * @param softi2c_initstruct: A softi2c* struct pointer
 * @return - None
 */
void bsp_soft_i2c_init(Softi2c* softi2c_initstruct){
    // ENABLE the APB2 Clock Peripheral of GPIOx.
    uint32_t soft_i2c_rccperiph;
    gpiox = softi2c_initstruct->i2c_GPIOPort;
    scl = softi2c_initstruct->i2c_scl;
    sda = softi2c_initstruct->i2c_sda;
    debug_flag = softi2c_initstruct->debug;
    if (softi2c_initstruct->i2c_GPIOPort == GPIOA){
        soft_i2c_rccperiph = RCC_APB2Periph_GPIOA;
    } else if (softi2c_initstruct->i2c_GPIOPort == GPIOB){
        soft_i2c_rccperiph = RCC_APB2Periph_GPIOB;
    } else if (softi2c_initstruct->i2c_GPIOPort == GPIOC){
        soft_i2c_rccperiph = RCC_APB2Periph_GPIOC;
    } else if (softi2c_initstruct->i2c_GPIOPort == GPIOD){
        soft_i2c_rccperiph = RCC_APB2Periph_GPIOD;
    } else if (softi2c_initstruct->i2c_GPIOPort == GPIOE){
        soft_i2c_rccperiph = RCC_APB2Periph_GPIOE;
    } else if (softi2c_initstruct->i2c_GPIOPort == GPIOF){
        soft_i2c_rccperiph = RCC_APB2Periph_GPIOF;
    } else if (softi2c_initstruct->i2c_GPIOPort == GPIOG){
        soft_i2c_rccperiph = RCC_APB2Periph_GPIOG;
    }
    RCC_APB2PeriphClockCmd(soft_i2c_rccperiph,ENABLE);
    GPIO_InitTypeDef soft_i2c_sda_gpioinit = {
            .GPIO_Mode = softi2c_initstruct->debug ? GPIO_Mode_Out_PP : GPIO_Mode_Out_OD,
            .GPIO_Speed = GPIO_Speed_10MHz,
            .GPIO_Pin = softi2c_initstruct->i2c_sda
    };
    GPIO_Init(softi2c_initstruct->i2c_GPIOPort, &soft_i2c_sda_gpioinit);
    GPIO_InitTypeDef soft_i2c_scl_gpioinit = {
            .GPIO_Mode = softi2c_initstruct->debug ? GPIO_Mode_Out_PP : GPIO_Mode_Out_OD,
            .GPIO_Speed = GPIO_Speed_10MHz,
            .GPIO_Pin = softi2c_initstruct->i2c_scl,
    };
    GPIO_Init(softi2c_initstruct->i2c_GPIOPort, &soft_i2c_scl_gpioinit);

    GPIO_WriteBit(softi2c_initstruct->i2c_GPIOPort,softi2c_initstruct->i2c_scl,Bit_SET);
    GPIO_WriteBit(softi2c_initstruct->i2c_GPIOPort,softi2c_initstruct->i2c_sda,Bit_SET);
    Delay_ms(5);
}

/**
 * @brief: to control the bit action of scl line
 * @param dat: can be set within {0,1} and determines whether the scl is high or low.
 * @note: the bit set/reset register (GPIOx->BSRR/GPIO->BRR) shared a uint32_t space, where BSRR
 * occupies the lower 2 Bytes and BRR higher 2 Bytes.
 * */
static void i2c_scl_drive(uint8_t dat){
    gpiox->BSRR = scl<<(16*(1-dat));
    Delay_us(10);
}
/**
 * @brief: to control the bit action of sda line
 * @param dat: can be set within {0,1} and determines whether the scl is high or low.
 * */
static void i2c_sda_drive(uint8_t dat){
    gpiox->BSRR = sda<<(16*(1-dat));
    Delay_us(10);
}

///@brief: to start the i2c transmission session.
///@param: None
///@retval None
void bsp_soft_i2c_start(){
    // pull the SCL and SDA high.
    i2c_sda_drive(1);
    i2c_scl_drive(1);
    __NOP();
    // when SCL is high, pull SDA from high to low
    i2c_sda_drive(0);
    i2c_scl_drive(0);
    // After Starting the session both SCL and SDA are pulled low.
}

/**
 * @brief to send the data after starting i2c session
 * @param addr: the combination of 7-bit address and read/write flag
 * @param data: is the data sent from master to slave
 * @retval None
*/
void bsp_soft_i2c_send(uint8_t byte){
    for (uint8_t i=0; i<8; i++){
        uint8_t temp_dat = (byte>>(7-i))&0x01;
        i2c_sda_drive(temp_dat);
        i2c_scl_drive(1);
        __NOP();
        i2c_scl_drive(0);
    }
    // ACK, SDA is pulled high in no debug mode, SDA is low to simulate an `ACK`.
    i2c_sda_drive(1-debug_flag);
    i2c_scl_drive(1);
    __NOP();//
    i2c_scl_drive(0);

}

/**
 * @brief: to end the i2c transmission session.
 * @param None
 * @retval None
 * */
void bsp_soft_i2c_end(){
    i2c_sda_drive(0);
    i2c_scl_drive(1);
    // when an i2c session ends, during high period of SCL, SDA is pulled low.
    i2c_sda_drive(1);
}

/**
 * @brief: to send a data byte to the device
 * @param device: the device address of an i2c peripheral
 * @param data: a uint8_t type data for transmission
 */
void bsp_soft_i2c_byte_write(uint8_t device, uint8_t data){
    bsp_soft_i2c_start();
    bsp_soft_i2c_send(device);
    bsp_soft_i2c_send(data);
    bsp_soft_i2c_end();
}

/**
 * @brief: to read a byte of data from the i2c peripheral
 * @param device: the device address of an i2c peripheral, the LSB should be set to 1 to indicate a read operation
 * @param data: a uint8_t type for the data buffer.
 * @param length: the length for all the data to be read.
 * @note: When we read the data from the i2c slave, gpio pin `SDA` should be configured as input mode,
 * therefore, frequent switching between input and output mode is of low efficiency.
 * * */
void bsp_soft_i2c_array_read(uint8_t device, uint8_t* data, uint8_t length) {
    bsp_soft_i2c_start();
    bsp_soft_i2c_send(device);
    // to change the mode of SDA pin to input mode
    for (uint8_t i= 0;  i< length; i++) {
        i2c_byte_read(data++, (i==length-1)?DISABLE:ENABLE);
    }
}

/**
 * @brief: internal method, abstract the process of i2c reading a byte of data from the slave
 * @param pos: a uint8_t* pointer for the data buffer
 * @param ack: to determine whether to send an ACK after reading a byte of data,
 * when a NACK is sent, the i2c slave will stop putting data onto the i2c bus.
 * */
static void i2c_byte_read(uint8_t* pos, FunctionalState ack) {
    uint8_t _temp = 0;
    sda_mode_switch(0);
    for (uint8_t i= 0;  i<8 ; i++) {
        i2c_scl_drive(1);
        __NOP();
        _temp |= ((gpiox->IDR & sda)?1:0) << (7-i);
        i2c_scl_drive(0);
    }
    *pos = _temp;
    // the process of sending ACK or NACK
    sda_mode_switch(1);
    i2c_sda_drive(1-ack);
    i2c_scl_drive(1);
    __NOP();
    i2c_scl_drive(0);
 }

 /**
  * @brief: to write a byte of data to a specific 8-bit address register of an i2c peripheral
  * @param device: the device address of an i2c peripheral
  * @param reg: the 8-bit register address of the i2c peripheral
  * @param data: a uint8_t type data for transmission
  * */
void bsp_soft_i2c_reg8byte(uint8_t device, uint8_t reg, uint8_t data){
    bsp_soft_i2c_start();
    bsp_soft_i2c_send(device);
    bsp_soft_i2c_send(reg);
    bsp_soft_i2c_send(data);
    bsp_soft_i2c_end();
}

/**
 * @brief: to send a uint8_t array to the device
 * @param device: the device address of an i2c peripheral, the LSB should be set to 0 to indicate a write operation
 * @param reg: the 8-bit register address of the i2c peripheral
 * @param data: a uint8_t pointer for the data buffer
 * @param length: the length for all the data to be sent
 * */
void bsp_soft_i2c_reg8array(uint8_t device, uint8_t  reg, const uint8_t* data, uint8_t length) {
    bsp_soft_i2c_start();
    bsp_soft_i2c_send(device);
    bsp_soft_i2c_send(reg);
    for (uint8_t pos = 0; pos< length; pos++) {
        bsp_soft_i2c_send(data[pos]);
    }
    bsp_soft_i2c_end();
}

/**
 * @brief： to switch the input/output mode of sda pin
 * @param mode: if mode ==0, the sda pin will be set to GPIO_MODE_IN_FLOATING,
 *              elif mode ==1, the sda pin will be set to GPIO_MODE_OUT_OD or GPIO_MODE_OUT_PP,
 *              depending on the debug_flag.
 * */
static void sda_mode_switch(uint8_t mode){
    GPIO_InitTypeDef sda_mode = {
        .GPIO_Mode = mode? (debug_flag?GPIO_Mode_Out_PP:GPIO_Mode_Out_OD) : GPIO_Mode_IN_FLOATING,
        .GPIO_Speed = GPIO_Speed_10MHz,
        .GPIO_Pin = sda
    };
    GPIO_Init(gpiox,&sda_mode);
}
