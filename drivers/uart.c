#include <stdint.h>
#include "system_stm32f4xx.h" 
#define UART0_BASE 0x4000C000

typedef struct {
    volatile uint32_t DATA;
    volatile uint32_t STATE;
    volatile uint32_t CTRL;
    volatile uint32_t BAUDDIV;
} uart_t;

#define UART0 ((uart_t*)UART0_BASE)

void uart_init(uint32_t baudrate) {
    UART0->BAUDDIV = SystemCoreClock / baudrate;
    UART0->CTRL = 1; // Enable UART
}

void uart_putchar(char c) {
    while (UART0->STATE & 0x02); // Wait for TX ready
    UART0->DATA = c;
}

char uart_getchar(void) {
    while (!(UART0->STATE & 0x01)); // Wait for RX ready
    return UART0->DATA;
}

void uart_puts(const char *str) {
    while (*str) {
        uart_putchar(*str++);
    }
}

