#include <stdint.h>

#define GPIO_BASE 0x50000000

typedef struct {
    volatile uint32_t DATA;
    volatile uint32_t DIR;
    volatile uint32_t IS;
    volatile uint32_t IBE;
    volatile uint32_t IEV;
    volatile uint32_t IE;
    volatile uint32_t RIS;
    volatile uint32_t MIS;
    volatile uint32_t ICR;
    volatile uint32_t AFSEL;
} gpio_t;

#define GPIO ((gpio_t*)GPIO_BASE)

void gpio_set_direction(int pin, int output) {
    if (output) {
        GPIO->DIR |= (1 << pin);
    } else {
        GPIO->DIR &= ~(1 << pin);
    }
}

void gpio_write(int pin, int value) {
    if (value) {
        GPIO->DATA |= (1 << pin);
    } else {
        GPIO->DATA &= ~(1 << pin);
    }
}

int gpio_read(int pin) {
    return (GPIO->DATA >> pin) & 1;
}