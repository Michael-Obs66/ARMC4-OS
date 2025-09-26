#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>

void printf(const char *fmt, ...);
void sprintf(char *str, const char *fmt, ...);
int vsprintf(char *str, const char *fmt, va_list args);

#endif
