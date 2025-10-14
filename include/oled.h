#ifndef OLED_H
#define OLED_H

#include <stdint.h>

#define OLED_ADDR 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

void oled_init(void);
void oled_clear(void);
void oled_set_cursor(uint8_t x, uint8_t y);
void oled_write_char(char c);
void oled_write_string(const char *str);

#endif
