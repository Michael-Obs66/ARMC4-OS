#include "../include/drivers.h"

#define GPIOA_BASE 0x48000000
#define GPIOA_MODER (*((volatile uint32_t *)(GPIOA_BASE + 0x00)))
#define GPIOA_ODR   (*((volatile uint32_t *)(GPIOA_BASE + 0x14)))

#define GPIOB_BASE 0x48000400
#define GPIOB_MODER (*((volatile uint32_t *)(GPIOB_BASE + 0x00)))
#define GPIOB_ODR   (*((volatile uint32_t *)(GPIOB_BASE + 0x14)))

void gpio_init(void)
{
    // Enable GPIO clocks already done in SystemInit
    
    // Configure PA5 as output (LED)
    GPIOA_MODER |= (1 << 10);
}

void gpio_set(uint8_t port, uint8_t pin, uint8_t value)
{
    volatile uint32_t *odr;
    
    switch (port) {
        case GPIO_PORT_A:
            odr = &GPIOA_ODR;
            break;
        case GPIO_PORT_B:
            odr = &GPIOB_ODR;
            break;
        default:
            return;
    }
    
    if (value) {
        *odr |= (1 << pin);
    } else {
        *odr &= ~(1 << pin);
    }
}

uint8_t gpio_get(uint8_t port, uint8_t pin)
{
    volatile uint32_t *idr;
    
    switch (port) {
        case GPIO_PORT_A:
            idr = (volatile uint32_t*)(GPIOA_BASE + 0x10);
            break;
        case GPIO_PORT_B:
            idr = (volatile uint32_t*)(GPIOB_BASE + 0x10);
            break;
        default:
            return 0;
    }
    
    return (*idr >> pin) & 1;
}


