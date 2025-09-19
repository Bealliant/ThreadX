//
// Created by a2012 on 18/9/2025.
//

#ifndef THREADX_PERIPH_AT24CXX_H
#define THREADX_PERIPH_AT24CXX_H

#include "stm32f10x.h"
#include "bsp_i2c.h"

typedef enum {
    at24cxx_addr_write = 0b10100000,
    at24cxx_addr_read = 0b10100001,
} at24cxx_addr;

#define AT24C64


void at24cxx_init(void);
#if defined(AT24C01) || defined(AT24C02) || defined(AT24C04) || defined(AT24C08) || defined(AT24C16)
void at24cxx_byte_write(uint8_t addr, uint8_t data);
uint8_t at24cxx_byte_read(uint8_t addr);
void at24cxx_array_write(uint8_t addr, const uint8_t* data, uint8_t length);
void at24cxx_array_read(uint8_t addr, uint8_t* data, uint8_t length);
#endif

// Define the type of AT24CXX Series EEPROM, for AT24C32+ use 16-bit address
#if defined(AT24C32) || defined(AT24C64) || defined (AT24C128) || defined(AT24C256) || defined (AT24C512)
void at24cxx_byte_write(uint16_t addr, uint8_t data);
uint8_t at24cxx_byte_read(uint16_t addr);
void at24cxx_array_write(uint16_t addr, const uint8_t* data, uint16_t length);
void at24cxx_array_read(uint16_t addr, uint8_t* data, uint8_t length);
#endif

#endif //THREADX_PERIPH_AT24CXX_H
