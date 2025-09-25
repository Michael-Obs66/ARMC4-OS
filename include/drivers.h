#ifndef DRIVERS_H
#define DRIVERS_H

// UART
void uart_init(uint32_t baudrate);
void uart_putchar(char c);
char uart_getchar(void);
void uart_puts(const char *str);

// GPIO
void gpio_set_direction(int pin, int output);
void gpio_write(int pin, int value);
int gpio_read(int pin);

// Timer
void timer_init(uint32_t interval_ms);
void timer_delay_ms(uint32_t ms);

#endif
