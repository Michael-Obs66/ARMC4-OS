#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

void kernel_main(void);
uint32_t kernel_get_ticks(void);
uint8_t kernel_is_initialized(void);
void kernel_panic(const char *message);

#endif
