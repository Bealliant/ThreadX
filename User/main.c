//
// Created by a2012 on 3/5/2025.
//
#include "stm32f10x.h"
#include "bsp_i2c.h"
#include "bsp_delay.h"
#include "stdio.h"
#include "periph_led.h"
#include "bsp_uart.h"
#include "periph_mpu6050.h"
#include "periph_at24cxx.h"


int main(){
    uint8_t write_buf[5] = {0xDE, 0xAD, 0xBE, 0xEF, 0x01};
    uint8_t read_buf[5] = {0};
    bsp_uartx_init(printf_UART,DISABLE);
    bsp_led_init();
    at24cxx_init();
    while (1)
    {
        bsp_led_toggle();
        at24cxx_array_write(0x00, write_buf, 5);
        Delay_ms(10);
        at24cxx_array_read(0x00, read_buf, 5);
        for (int i = 0; i < 5; ++i) {
            printf("0x%02X ", read_buf[i]);
        }
        Delay_ms(50);
    }
    return 0;
}