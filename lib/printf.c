#include "../lib/printf.h"

static void print_char(char c)
{
    uart_putc(c);
}

static void print_string(const char *s)
{
    while (*s) {
        print_char(*s++);
    }
}

static void print_number(uint32_t num, uint32_t base)
{
    char buffer[32];
    char *p = buffer;
    
    if (num == 0) {
        print_char('0');
        return;
    }
    
    while (num > 0) {
        uint32_t digit = num % base;
        *p++ = digit < 10 ? '0' + digit : 'a' + digit - 10;
        num /= base;
    }
    
    while (p > buffer) {
        print_char(*--p);
    }
}

void printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd': {
                    int num = va_arg(args, int);
                    if (num < 0) {
                        print_char('-');
                        num = -num;
                    }
                    print_number(num, 10);
                    break;
                }
                case 'u': {
                    uint32_t num = va_arg(args, uint32_t);
                    print_number(num, 10);
                    break;
                }
                case 'x': {
                    uint32_t num = va_arg(args, uint32_t);
                    print_number(num, 16);
                    break;
                }
                case 's': {
                    char *s = va_arg(args, char*);
                    print_string(s);
                    break;
                }
                case 'c': {
                    char c = va_arg(args, int);
                    print_char(c);
                    break;
                }
                case '%': {
                    print_char('%');
                    break;
                }
            }
        } else {
            print_char(*fmt);
        }
        fmt++;
    }
    
    va_end(args);
}
