#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm32f4xx.h"

/* System clock: 180 MHz (max) */
#define HSE_VALUE        8000000U
#define SYSCLK_FREQ      180000000U

/* UART for debug */
#define DEBUG_UART       USART2
#define DEBUG_UART_TX_PIN GPIO_PIN_2
#define DEBUG_UART_RX_PIN GPIO_PIN_3
#define DEBUG_UART_GPIO_PORT GPIOA
#define DEBUG_UART_AF     GPIO_AF7_USART2

/* LED pin (optional) */
#define LED_PORT GPIOA
#define LED_PIN  GPIO_PIN_5

/* Default stack and heap size */
#define STACK_SIZE       0x400
#define HEAP_SIZE        0x1000

void Board_Init(void);

#endif /* BOARD_CONFIG_H */
