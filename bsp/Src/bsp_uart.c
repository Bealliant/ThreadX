#include "bsp_uart.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

/// store the information about
static const USART_MAP map[]={
        {USART1,
         GPIOA,GPIO_Pin_9,GPIO_Pin_10,
         USART1_IRQn
         },
        {USART2,
         GPIOA,GPIO_Pin_2,GPIO_Pin_3,
         USART2_IRQn},
        {USART3,
         GPIOB,GPIO_Pin_10,GPIO_Pin_11,
         USART3_IRQn}
};

/**
 * @param USARTx : the input USARTx Peripheral for initialize
 * @return a `USART_MAP` type pointer
 */
const USART_MAP* get_usartx_map(USART_TypeDef* USARTx){
    if (USARTx==USART1) return map;
    else if (USARTx==USART2) return map+1;
    else if (USARTx==USART3) return map+2;
    return NULL;
}


/**
 * @param   None
 * @brief   Initialize the `USART1` Peripheral.
*/
void bsp_uartx_init(USART_TypeDef* USARTx, FunctionalState USARTx_ITEN){
    const USART_MAP* _map = get_usartx_map(USARTx);
    if (USARTx==USART1){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    }
    else if (USARTx==USART2){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    }
    else if (USARTx==USART3){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    }
    GPIO_InitTypeDef uart_tx_gpio_initstruct = {
        .GPIO_Mode = GPIO_Mode_AF_PP,
        .GPIO_Pin = _map->usartx_tx,
        .GPIO_Speed = GPIO_Speed_50MHz
    };
    GPIO_InitTypeDef uart_rx_gpio_initstruct = {
        .GPIO_Mode = GPIO_Mode_IN_FLOATING,
        .GPIO_Pin = _map->usartx_rx,
        .GPIO_Speed = GPIO_Speed_50MHz
    };
    GPIO_Init(_map->usartx_GPIOPort,&uart_tx_gpio_initstruct);
    GPIO_Init(_map->usartx_GPIOPort,&uart_rx_gpio_initstruct);

    USART_InitTypeDef uart_initstruct = {
        .USART_BaudRate = UART_BaudRate,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
        .USART_Mode = USART_Mode_Rx|USART_Mode_Tx, // Duplex Communication
        .USART_Parity = USART_Parity_No, 
        .USART_StopBits = USART_StopBits_1,
        .USART_WordLength = USART_WordLength_8b
    };
    USART_Init(_map->usartx,&uart_initstruct);
    if (USARTx_ITEN==ENABLE){
        NVIC_InitTypeDef uart_nvic_initstruct = {
            .NVIC_IRQChannel = USART1_IRQn,
            .NVIC_IRQChannelCmd = ENABLE,
            .NVIC_IRQChannelPreemptionPriority = 1,
            .NVIC_IRQChannelSubPriority = 1
        };
        NVIC_Init(&uart_nvic_initstruct);
    }

    USART_Cmd(_map->usartx,ENABLE);
}


/**
 * @param   USARTX, the usart channel 
 * @param   send_char, the send data
 * @brief   use the blocking method to send data `send_char` to the uart device
*/
void bsp_uartx_transmit(USART_TypeDef* USARTx,uint8_t send_char){
    USART_SendData(USARTx,send_char);
    while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
}

/**
 * @brief   using the USARTX to send an array buffer (HEX, char, etc) with a length of array_len via USART Channel USARTx.
 * @param   USARTx:     the USART Channel for sending data.
 * @param   array_buf:  the array pointer(uint8_t type)
 * @param   array_len:  the length of the array 
*/
void bsp_uartx_sendarray(USART_TypeDef* USARTx, uint8_t* array_buf, uint8_t array_len){
    for(uint8_t i=0; i<array_len; i++){
        bsp_uartx_transmit(USARTx,array_buf[i]);
    }
}

/**
 * @brief   remap the printf function.
 * @note    There are some methods not implemented in arm_gcc_eabi,
 * add link option ``--specs=nosys.specs`` is necessary
*/
int _write (int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        bsp_uartx_transmit(printf_UART,(uint8_t)*pBuffer++);
    }
    return size;
}

 

