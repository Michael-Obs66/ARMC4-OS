#include "../lib/drivers.h"

#define USART1_BASE 0x40013800
#define USART1_SR   (*((volatile uint32_t *)(USART1_BASE + 0x00)))
#define USART1_DR   (*((volatile uint32_t *)(USART1_BASE + 0x04)))
#define USART1_BRR  (*((volatile uint32_t *)(USART1_BASE + 0x08)))
#define USART1_CR1  (*((volatile uint32_t *)(USART1_BASE + 0x0C)))

#define GPIOA_BASE 0x48000000
#define GPIOA_AFRH (*((volatile uint32_t *)(GPIOA_BASE + 0x24)))

void uart_init(void)
{
    // Configure PA9 as TX, PA10 as RX
    // Enable AF mode (AF7 for USART1)
    GPIOA_AFRH |= (7 << 4) | (7 << 8);
    
    // Configure USART1
    USART1_BRR = 833; // 72MHz / 115200 = 625
    USART1_CR1 = (1 << 13) | (1 << 3) | (1 << 2); // UE, TE, RE
}

void uart_putc(char c)
{
    while (!(USART1_SR & (1 << 7))); // Wait for TXE
    USART1_DR = c;
}

char uart_getc(void)
{
    while (!(USART1_SR & (1 << 5))); // Wait for RXNE
    return USART1_DR & 0xFF;
}

void uart_puts(const char *str)
{
    while (*str) {
        uart_putc(*str++);
    }
}

