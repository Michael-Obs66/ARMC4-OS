#include "../include/i2c.h"
#include "../include/task.h"

#define GPIOB_BASE      0x48000400UL
#define RCC_BASE        0x40021000UL

#define RCC_AHB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x4C))
#define RCC_APB1ENR1    (*(volatile uint32_t *)(RCC_BASE + 0x58))
#define GPIOB_MODER     (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_AFRL      (*(volatile uint32_t *)(GPIOB_BASE + 0x20))

#define I2C_ISR_TXIS    (1 << 1)
#define I2C_ISR_STOPF   (1 << 5)
#define I2C_CR2_AUTOEND (1 << 25)
#define I2C_CR2_START   (1 << 13)
#define I2C_CR2_RD_WRN  (1 << 10)

void i2c_init(void)
{
    // Enable clock GPIOB + I2C1
    RCC_AHB2ENR |= (1 << 1);
    RCC_APB1ENR1 |= (1 << 21);

    // PB6=SCL, PB7=SDA Alternate Function AF4
    GPIOB_MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)));
    GPIOB_MODER |= ((2 << (6 * 2)) | (2 << (7 * 2)));
    GPIOB_AFRL &= ~((0xF << (6 * 4)) | (0xF << (7 * 4)));
    GPIOB_AFRL |= ((4 << (6 * 4)) | (4 << (7 * 4)));

    // Disable I2C before config
    I2C1_CR1 &= ~1;

    // Timing config for 100kHz @16MHz APB1
    I2C1_TIMINGR = 0x20303E5D;

    // Enable I2C1
    I2C1_CR1 |= 1;
}

void i2c_write(uint8_t addr, const uint8_t *data, uint16_t len)
{
    // Wait if BUSY
    while (I2C1_ISR & (1 << 15));

    I2C1_CR2 = (addr << 1) | (len << 16) | I2C_CR2_START | I2C_CR2_AUTOEND;

    for (uint16_t i = 0; i < len; i++) {
        while (!(I2C1_ISR & I2C_ISR_TXIS));
        I2C1_TXDR = data[i];
    }

    while (!(I2C1_ISR & I2C_ISR_STOPF));
    I2C1_ICR |= I2C_ISR_STOPF;
}

uint8_t i2c_read(uint8_t addr, uint8_t *data, uint16_t len)
{
    while (I2C1_ISR & (1 << 15));

    I2C1_CR2 = (addr << 1) | (len << 16) | I2C_CR2_START | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN;

    for (uint16_t i = 0; i < len; i++) {
        while (!(I2C1_ISR & (1 << 2))); // RXNE
        data[i] = (uint8_t)I2C1_RXDR;
    }

    while (!(I2C1_ISR & I2C_ISR_STOPF));
    I2C1_ICR |= I2C_ISR_STOPF;

    return 0;
}
