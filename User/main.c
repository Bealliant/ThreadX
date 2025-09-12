//
// Created by a2012 on 3/5/2025.
//
#include "stm32f10x.h"
#include "bsp_i2c.h"
#include "bsp_delay.h"
#include "stdio.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_mpu6050.h"

int main(){

    bsp_led_init();
    bsp_uartx_init(printf_UART,DISABLE);
    mpu6050_info mpu6050_data;
    mpu6050_init();
//    Delay_ms(100);

    while (1)
    {
        bsp_led_toggle();
        mpu6050_get_data(&mpu6050_data);
        mpu6050_info_print(&mpu6050_data);
        Delay_ms(50);
    }
    return 0;
}