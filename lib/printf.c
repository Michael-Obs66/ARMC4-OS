#include <stdarg.h>
#include <stdint.h>
#include "../include/uart.h"

/* =========================================================
 *  Mini printf untuk ARMC4-OS
 *  Tanpa libc, support: %s %d %u %x %c
 * ========================================================= */
static void utoa_dec(char *buf, unsigned int val)
{
    char tmp[16];
    int i = 0;
    do {
        tmp[i++] = (val % 10) + '0';
        val /= 10;
    } while (val > 0);

    while (i--) *buf++ = tmp[i];
    *buf = '\0';
}

static void utoa_hex(char *buf, unsigned int val)
{
    const char hex[] = "0123456789ABCDEF";
    char tmp[16];
    int i = 0;
    do {
        tmp[i++] = hex[val & 0xF];
        val >>= 4;
    } while (val > 0);

    while (i--) *buf++ = tmp[i];
    *buf = '\0';
}

int printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char ch;
    char numbuf[32];
    const char *s;

    while ((ch = *fmt++)) {
        if (ch != '%') {
            uart_send_char(ch);
            continue;
        }

        ch = *fmt++;
        switch (ch) {
        case 's':
            s = va_arg(args, const char *);
            if (s) uart_send_string(s);
            break;
        case 'd':
        case 'u':
            utoa_dec(numbuf, va_arg(args, unsigned int));
            uart_send_string(numbuf);
            break;
        case 'x':
        case 'X':
            utoa_hex(numbuf, va_arg(args, unsigned int));
            uart_send_string(numbuf);
            break;
        case 'c':
            uart_send_char((char)va_arg(args, int));
            break;
        case '%':
            uart_send_char('%');
            break;
        default:
            uart_send_char('?');
        }
    }

    va_end(args);
    return 0;
}
