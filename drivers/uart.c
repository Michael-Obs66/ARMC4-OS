#include "stm32f4xx.h"
#include "../include/uart.h"

#define USART2_BASE 0x40004400UL
#define RCC_BASE    0x40023800UL

#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define USART2_SR   (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR   (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR  (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1  (*(volatile uint32_t *)(USART2_BASE + 0x0C))

void uart_init(void)
{
    RCC_APB1ENR |= (1 << 17);        // Enable USART2 clock
    USART2_BRR = 0x0683;             // 9600 baud @16MHz
    USART2_CR1 = (1 << 13) | (1 << 3) | (1 << 2); // UE, TE, RE enable
}

void uart_send_char(char c)
{
    while (!(USART2_SR & (1 << 7))); // TXE wait
    USART2_DR = c;
}

void uart_send_string(const char *str)
{
    while (*str)
        uart_send_char(*str++);
}
