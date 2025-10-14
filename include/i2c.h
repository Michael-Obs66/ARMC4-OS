#ifndef I2C_H
#define I2C_H

#include <stdint.h>

// Base address I2C1 STM32L4
#define I2C1_BASE 0x40005400UL

// Register offset
#define I2C_CR1_OFFSET      0x00
#define I2C_CR2_OFFSET      0x04
#define I2C_TIMINGR_OFFSET  0x10
#define I2C_ISR_OFFSET      0x18
#define I2C_ICR_OFFSET      0x1C
#define I2C_TXDR_OFFSET     0x28
#define I2C_RXDR_OFFSET     0x24

// Register map
#define I2C1_CR1     (*(volatile uint32_t *)(I2C1_BASE + I2C_CR1_OFFSET))
#define I2C1_CR2     (*(volatile uint32_t *)(I2C1_BASE + I2C_CR2_OFFSET))
#define I2C1_TIMINGR (*(volatile uint32_t *)(I2C1_BASE + I2C_TIMINGR_OFFSET))
#define I2C1_ISR     (*(volatile uint32_t *)(I2C1_BASE + I2C_ISR_OFFSET))
#define I2C1_ICR     (*(volatile uint32_t *)(I2C1_BASE + I2C_ICR_OFFSET))
#define I2C1_TXDR    (*(volatile uint32_t *)(I2C1_BASE + I2C_TXDR_OFFSET))
#define I2C1_RXDR    (*(volatile uint32_t *)(I2C1_BASE + I2C_RXDR_OFFSET))

void i2c_init(void);
void i2c_write(uint8_t addr, const uint8_t *data, uint16_t len);
uint8_t i2c_read(uint8_t addr, uint8_t *data, uint16_t len);

#endif
