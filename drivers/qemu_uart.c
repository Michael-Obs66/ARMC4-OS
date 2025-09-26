#include "../include/drivers.h"

// UART emulation untuk QEMU
// QEMU semihosting atau PL011 UART

#ifdef QEMU_SEMIHOSTING
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static int stdio_initialized = 0;

void uart_init(void)
{
    // Di QEMU, stdio sudah tersedia
    stdio_initialized = 1;
}

void uart_putc(char c)
{
    write(1, &c, 1); // stdout
}

char uart_getc(void)
{
    char c;
    read(0, &c, 1); // stdin
    return c;
}

#else
// PL011 UART emulation (lebih realistis)

#define PL011_BASE 0x09000000  // QEMU virt machine UART base

volatile unsigned int *uart_reg = (volatile unsigned int *)PL011_BASE;

void uart_init(void)
{
    // Minimal init untuk QEMU
}

void uart_putc(char c)
{
    while (uart_reg[0x18] & (1 << 5)); // Wait for TX empty
    uart_reg[0] = c;
    
    // Also output to stdout untuk debugging
    write(1, &c, 1);
}

char uart_getc(void)
{
    if (uart_reg[0x18] & (1 << 4)) { // RX ready
        return uart_reg[0];
    }
    return 0;
}

#endif

void uart_puts(const char *str)
{
    while (*str) {
        uart_putc(*str++);
    }
}
