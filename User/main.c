//
// Created by a2012 on 3/5/2025.
//
#include "commonheader.h"

int main(){
    bmp280_data_t bmp280_data;
    bsp_uartx_init(printf_UART,DISABLE);
    bsp_led_init();
    bmp280_init();
    while (1)
    {
        bsp_led_toggle();
        bmp280_read_rawdata(&bmp280_data);
        bmp280_info(&bmp280_data);
        Delay_ms(50);
    }
    return 0;
}