#ifndef DRIVERS_H
#define DRIVERS_H

#include <stdint.h>

#define GPIO_PORT_A 0
#define GPIO_PORT_B 1
#define GPIO_PORT_C 2

void uart_init(void);
void uart_putc(char c);
char uart_getc(void);
void uart_puts(const char *str);

void gpio_init(void);
void gpio_set(uint8_t port, uint8_t pin, uint8_t value);
uint8_t gpio_get(uint8_t port, uint8_t pin);

void timer_init(void);
void timer_delay_ms(uint32_t ms);
uint32_t timer_get_ticks(void);

void spi_init(void);
uint8_t spi_transfer(uint8_t data);
void spi_write(const uint8_t *data, uint32_t len);
void spi_read(uint8_t *buffer, uint32_t len);

#endif
