#include <stdint.h>
#include <stdbool.h>
#include "system_stm32f4xx.h"
// OLED SSD1306 definitions
#define OLED_ADDRESS 0x3C
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// I2C registers (simulated)
#define I2C_BASE 0x40000000
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
} i2c_t;

#define I2C ((i2c_t*)I2C_BASE)

// OLED commands
#define OLED_SET_CONTRAST 0x81
#define OLED_SET_ENTIRE_ON 0xA4
#define OLED_SET_NORM_INV 0xA6
#define OLED_SET_DISP 0xAE
#define OLED_SET_MEM_ADDR 0x20
#define OLED_SET_COL_ADDR 0x21
#define OLED_SET_PAGE_ADDR 0x22
#define OLED_SET_DISP_START_LINE 0x40
#define OLED_SET_SEG_REMAP 0xA0
#define OLED_SET_MUX_RATIO 0xA8
#define OLED_SET_COM_OUT_DIR 0xC0
#define OLED_SET_DISP_OFFSET 0xD3
#define OLED_SET_COM_PIN_CFG 0xDA
#define OLED_SET_DISP_CLK_DIV 0xD5
#define OLED_SET_PRECHARGE 0xD9
#define OLED_SET_VCOM_DESEL 0xDB
#define OLED_SET_CHARGE_PUMP 0x8D

static uint8_t oled_buffer[OLED_WIDTH * OLED_HEIGHT / 8];

void i2c_init(void) {
    // Initialize I2C peripheral
    I2C->CR1 = 0;
    I2C->CR2 = (16 << 0); // Frequency
    I2C->OAR1 = (1 << 14); // Own address
    I2C->CR1 = (1 << 0); // Enable I2C
}

void i2c_start(void) {
    I2C->CR1 |= (1 << 8); // Generate start
    while (!(I2C->SR1 & (1 << 0))); // Wait for SB flag
}

void i2c_stop(void) {
    I2C->CR1 |= (1 << 9); // Generate stop
}

void i2c_write_byte(uint8_t data) {
    I2C->DR = data;
    while (!(I2C->SR1 & (1 << 7))); // Wait for TxE
}

void oled_write_command(uint8_t cmd) {
    i2c_start();
    i2c_write_byte(OLED_ADDRESS << 1);
    i2c_write_byte(0x00); // Control byte (command)
    i2c_write_byte(cmd);
    i2c_stop();
}

void oled_write_data(uint8_t data) {
    i2c_start();
    i2c_write_byte(OLED_ADDRESS << 1);
    i2c_write_byte(0x40); // Control byte (data)
    i2c_write_byte(data);
    i2c_stop();
}

void oled_init(void) {
    i2c_init();
    
    // Initialize OLED display
    oled_write_command(OLED_SET_DISP | 0x00); // Display off
    
    oled_write_command(OLED_SET_MEM_ADDR); // Memory addressing mode
    oled_write_command(0x00); // Horizontal addressing mode
    
    oled_write_command(OLED_SET_DISP_START_LINE | 0x00);
    oled_write_command(OLED_SET_SEG_REMAP | 0x01); // Segment remap
    oled_write_command(OLED_SET_MUX_RATIO); // Multiplex ratio
    oled_write_command(0x3F); // 64MUX
    
    oled_write_command(OLED_SET_COM_OUT_DIR | 0x08); // Scan direction
    oled_write_command(OLED_SET_DISP_OFFSET); // Display offset
    oled_write_command(0x00); // No offset
    
    oled_write_command(OLED_SET_COM_PIN_CFG); // COM pins configuration
    oled_write_command(0x12);
    
    oled_write_command(OLED_SET_CONTRAST); // Contrast control
    oled_write_command(0xCF);
    
    oled_write_command(OLED_SET_PRECHARGE); // Pre-charge period
    oled_write_command(0xF1);
    
    oled_write_command(OLED_SET_VCOM_DESEL); // VCOMH deselect level
    oled_write_command(0x40);
    
    oled_write_command(OLED_SET_ENTIRE_ON); // Entire display on
    oled_write_command(OLED_SET_NORM_INV); // Normal display
    
    oled_write_command(OLED_SET_CHARGE_PUMP); // Charge pump
    oled_write_command(0x14);
    
    oled_write_command(OLED_SET_DISP | 0x01); // Display on
    
    // Clear display
    oled_clear();
}

void oled_clear(void) {
    for (int i = 0; i < sizeof(oled_buffer); i++) {
        oled_buffer[i] = 0x00;
    }
    oled_update();
}

void oled_update(void) {
    oled_write_command(OLED_SET_COL_ADDR);
    oled_write_command(0);
    oled_write_command(OLED_WIDTH - 1);
    
    oled_write_command(OLED_SET_PAGE_ADDR);
    oled_write_command(0);
    oled_write_command(7);
    
    for (int i = 0; i < sizeof(oled_buffer); i++) {
        oled_write_data(oled_buffer[i]);
    }
}

void oled_draw_pixel(int x, int y, bool color) {
    if (x < 0 || x >= OLED_WIDTH || y < 0 || y >= OLED_HEIGHT) return;
    
    int page = y / 8;
    int bit = y % 8;
    
    if (color) {
        oled_buffer[x + page * OLED_WIDTH] |= (1 << bit);
    } else {
        oled_buffer[x + page * OLED_WIDTH] &= ~(1 << bit);
    }
}

void oled_draw_char(int x, int y, char c) {
    // Simple 8x8 font (first 128 ASCII characters)
    static const uint8_t font8x8[128][8] = {
        // Basic implementation - you'd need a proper font here
        [32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // Space
        [65] = {0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00, 0x00}, // A
        // Add more characters as needed
    };
    
    if (c < 0 || c > 127) return;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (font8x8[c][i] & (1 << j)) {
                oled_draw_pixel(x + j, y + i, true);
            }
        }
    }
}

void oled_draw_string(int x, int y, const char* str) {
    while (*str) {
        oled_draw_char(x, y, *str++);
        x += 8;
        if (x >= OLED_WIDTH - 8) {
            x = 0;
            y += 8;
        }
    }
}

void oled_draw_line(int x0, int y0, int x1, int y1, bool color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        oled_draw_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void oled_draw_rect(int x, int y, int w, int h, bool color, bool fill) {
    if (fill) {
        for (int i = x; i < x + w; i++) {
            for (int j = y; j < y + h; j++) {
                oled_draw_pixel(i, j, color);
            }
        }
    } else {
        oled_draw_line(x, y, x + w, y, color);
        oled_draw_line(x, y + h, x + w, y + h, color);
        oled_draw_line(x, y, x, y + h, color);
        oled_draw_line(x + w, y, x + w, y + h, color);
    }
}
