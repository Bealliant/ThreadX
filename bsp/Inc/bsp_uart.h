#ifndef BSP_UART_H
#define BSP_UART_H
    #include "stm32f10x.h"
    #include "stdio.h"

typedef struct {
    USART_TypeDef * usartx;
    GPIO_TypeDef* usartx_GPIOPort;
    uint16_t usartx_tx;
    uint16_t usartx_rx;
    uint8_t IRQn_Channel;
}USART_MAP;

#define UART_BaudRate 115200
#define printf_UART USART2

void bsp_uartx_init(USART_TypeDef* USARTx, FunctionalState USARTx_ITEN);// Activate common settings without NVIC
void bsp_uartx_transmit(USART_TypeDef* USARTx,uint8_t send_char);
void bsp_uartx_sendarray(USART_TypeDef* USARTx, uint8_t* array_buf, uint8_t array_len);




#endif

