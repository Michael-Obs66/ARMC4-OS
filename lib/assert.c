#include <stddef.h>
#include "../include/assert.h"
#include "../include/printf.h"

void assert_failed(const char *file, int line, const char *expr)
{
    printf("Assertion failed: %s, file %s, line %d\r\n", expr, file, line);
    while (1);
}
