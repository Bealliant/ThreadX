//
// Created by a2012 on 18/9/2025.
//
#include "periph_at24cxx.h"

// TODO: Add Hardware i2c and DMA feature for eeprom read and write
/**
 * @brief: Initialize the at24cxx eeprom
 * @param: None
 * @return: None
 * */
void at24cxx_init(void){
    Softi2c at24cxx_i2c_initstruct = {
            .i2c_GPIOPort = GPIOB,
            .i2c_scl = GPIO_Pin_10,
            .i2c_sda = GPIO_Pin_11,
            .debug = DISABLE
    };
    bsp_soft_i2c_init(&at24cxx_i2c_initstruct);
}

#if defined(AT24C01) || defined(AT24C02) || defined(AT24C04) || defined(AT24C08) || defined(AT24C16)
/**
 * @brief: to write a data byte `data` to the address `addr`
 * @param: addr: the address to write to, 0~255
 * @param: data: the data byte to write
 * @return: None
 * */
void at24cxx_byte_write(uint8_t addr, uint8_t data){
    bsp_soft_i2c_reg8byte(at24cxx_addr_write, addr, data);
}

/**
 * @brief: to read a data byte from the address `addr`
 * @param: addr: the address to read from, 0~255
 * @return: the data byte read
 * */
uint8_t at24cxx_byte_read(uint8_t addr){
    uint8_t data;
    bsp_soft_i2c_reg8array(at24cxx_addr_read, addr, &data, 1);
    return data;
}

/**
 * @brief: to write an array of data `data` to the address `addr`
 * @param: addr: the starting address to write to, 0~255
 * @param: data: the pointer to the data array to write
 * @param: length: the length of the data array to write
 * @return: None
 * */
void at24cxx_array_write(uint8_t addr, const uint8_t* data, uint8_t length){
    bsp_soft_i2c_reg8array(at24cxx_addr_write, addr, data, length);
}

/**
 * @brief: to read an array of data from the address `addr`
 * @param: addr: the starting address to read from, 0~255
 * @param: data: the pointer to the data array to read into
 * @param: length: the length of the data array to read
 * @return: None
 * */
void at24cxx_array_read(uint8_t addr, uint8_t* data, uint8_t length){
    // Write the `addr` to the internal address register of at24cxx
    bsp_soft_i2c_byte_write(at24cxx_addr_write,addr);
    bsp_soft_i2c_array_read(at24cxx_addr_read, data, length);
}
#endif

#if defined(AT24C32) || defined(AT24C64) || defined (AT24C128) || defined(AT24C256) || defined (AT24C512)
/**
 * @brief: to write a single byte `data` to the 16-bit address `addr`, Attention the address is 2-Byte
 * @param: addr: the address to read from, 0~65535
 * @return: the data byte read
 * */
static uint8_t temp_addr[5];
/**
 * @brief: to write a data byte `data` to the 16-bit address `addr`
 * @param: addr: the 16-bit address to read from 0~65535
 * @return: the data byte read
 * */
void at24cxx_byte_write(uint16_t addr, uint8_t data){
   temp_addr[0] = (addr>>8)&0xFF;
   temp_addr[1] = addr&0xFF;
   temp_addr[2] = data;
   bsp_soft_i2c_array_write(at24cxx_addr_write, temp_addr, 3);
}

/**
 * @brief: to read a data byte from the 16-bit address `addr`
 * @param: addr: the 16-bit address to read from, 0~65535
 * @return: the data byte read
 * */
uint8_t at24cxx_byte_read(uint16_t addr){
    uint8_t data;
    temp_addr[0] = (addr>>8)&0xFF;
    temp_addr[1] = addr&0xFF;
    bsp_soft_i2c_array_write(at24cxx_addr_write, temp_addr, 2);
    bsp_soft_i2c_array_read(at24cxx_addr_read, &data, 1);
    return data;
}

/**
 * @brief: to write an array of data `data` to the 16-bit address `addr`
 * @param: addr: the starting 16-bit address to write to, 0~65535
 * @param: data: the pointer to the data array to write
 * @param: length: the length of the data array to write
 * @return: None
 * */
void at24cxx_array_write(uint16_t addr, const uint8_t* data, uint16_t length){
    bsp_soft_i2c_start();
    bsp_soft_i2c_send(at24cxx_addr_write);
    bsp_soft_i2c_send((addr>>8)&0xFF);
    bsp_soft_i2c_send(addr&0xFF);
    for (uint16_t i=0; i<length; i++) {
        bsp_soft_i2c_send(data[i]);
    }
    bsp_soft_i2c_end();
}

/**
 * @brief: to read an array of data from the 16-bit address `addr`
 * @param: addr: the starting 16-bit address to read from, 0~65535
 * @param: data: the pointer to the data array to read into
 * @param: length: the length of the data array to read
 * @return: None
 * */
void at24cxx_array_read(uint16_t addr, uint8_t* data, uint8_t length) {
    bsp_soft_i2c_start();
    bsp_soft_i2c_send(at24cxx_addr_write);
    bsp_soft_i2c_send((addr >> 8) & 0xFF);
    bsp_soft_i2c_send(addr & 0xFF);
    bsp_soft_i2c_array_read(at24cxx_addr_read, data, length);
}
#endif