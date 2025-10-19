#include <stdarg.h>
#include "../include/uart.h"

int printf(const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    uart_send_string(buf);
    return len;
}
