#include "../include/oled.h"
#include "../include/i2c.h"
#include "../include/font5x7.h"

static void oled_write_command(uint8_t cmd);
static void oled_write_data(uint8_t data);

void oled_init(void)
{
    oled_write_command(0xAE);
    oled_write_command(0x20); oled_write_command(0x00);
    oled_write_command(0x40);
    oled_write_command(0x81); oled_write_command(0x7F);
    oled_write_command(0xA1);
    oled_write_command(0xA6);
    oled_write_command(0xA8); oled_write_command(0x3F);
    oled_write_command(0xC8);
    oled_write_command(0xD3); oled_write_command(0x00);
    oled_write_command(0xD5); oled_write_command(0x80);
    oled_write_command(0xD9); oled_write_command(0xF1);
    oled_write_command(0xDA); oled_write_command(0x12);
    oled_write_command(0xDB); oled_write_command(0x40);
    oled_write_command(0x8D); oled_write_command(0x14);
    oled_write_command(0xAF);
    oled_clear();
}

void oled_clear(void)
{
    for (uint8_t page = 0; page < 8; page++) {
        oled_write_command(0xB0 + page);
        oled_write_command(0x00);
        oled_write_command(0x10);
        for (uint8_t col = 0; col < OLED_WIDTH; col++) {
            oled_write_data(0x00);
        }
    }
}

void oled_write_char(char c)
{
    if (c < 32 || c > 126) c = '?';
    const uint8_t *bitmap = font5x7[c - 32];
    for (int i = 0; i < 5; i++) oled_write_data(bitmap[i]);
    oled_write_data(0x00);
}

void oled_write_string(const char *str)
{
    while (*str) oled_write_char(*str++);
}

static void oled_write_command(uint8_t cmd)
{
    uint8_t control = 0x00;
    i2c_write(OLED_ADDR, &control, 1);
    i2c_write(OLED_ADDR, &cmd, 1);
}

static void oled_write_data(uint8_t data)
{
    uint8_t control = 0x40;
    i2c_write(OLED_ADDR, &control, 1);
    i2c_write(OLED_ADDR, &data, 1);
}
