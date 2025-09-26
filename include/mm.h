#ifndef MM_H
#define MM_H

#include <stdint.h>

void mm_init(void);
void *kmalloc(uint32_t size);
void kfree(void *ptr);
uint32_t mm_get_free(void);
uint32_t mm_get_used(void);

#endif
