//
// Created by a2012 on 5/9/2025.
//

#include "../Inc/periph_led.h"

void bsp_led_init(){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitTypeDef led_gpio_initstruct = {
            .GPIO_Mode = GPIO_Mode_Out_PP,
            .GPIO_Pin = LED_PIN,
            .GPIO_Speed = GPIO_Speed_2MHz
    };
    GPIO_Init(LED_PORT,&led_gpio_initstruct);
}


void bsp_led_toggle(){
    static uint8_t led_stat = 0;
    GPIO_WriteBit(LED_PORT,LED_PIN,led_stat);
    led_stat = 1- led_stat;
}
