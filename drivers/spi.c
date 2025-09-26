#include "drivers.h"

#define SPI1_BASE 0x40013000
#define SPI1_CR1  (*((volatile uint32_t *)(SPI1_BASE + 0x00)))
#define SPI1_SR   (*((volatile uint32_t *)(SPI1_BASE + 0x08)))
#define SPI1_DR   (*((volatile uint32_t *)(SPI1_BASE + 0x0C)))

void spi_init(void)
{
    // Configure SPI1
    SPI1_CR1 = (1 << 6) | (1 << 2) | (1 << 1); // SPE, MSTR, SSI
}

uint8_t spi_transfer(uint8_t data)
{
    while (!(SPI1_SR & (1 << 1))); // Wait for TXE
    SPI1_DR = data;
    
    while (!(SPI1_SR & (1 << 0))); // Wait for RXNE
    return SPI1_DR;
}

void spi_write(const uint8_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        spi_transfer(data[i]);
    }
}

void spi_read(uint8_t *buffer, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        buffer[i] = spi_transfer(0xFF);
    }
}
