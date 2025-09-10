//
// Created by a2012 on 5/9/2025.
//

#ifndef BSP_LED_H
#define BSP_LED_H

#include "stm32f10x.h"

#define LED_PORT GPIOC
#define LED_PIN GPIO_Pin_13

void bsp_led_init(void);
void bsp_led_toggle(void);
#endif
