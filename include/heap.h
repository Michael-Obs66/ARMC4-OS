#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

// Konfigurasi heap
#ifndef HEAP_SIZE
#define HEAP_SIZE (1024 * 10)      // 10 KB heap default
#endif

#ifndef HEAP_BLOCK_SIZE
#define HEAP_BLOCK_SIZE 32         // ukuran tiap block
#endif

// Heap management functions
void heap_init(void);
void *heap_alloc(uint32_t size);
void heap_free(void *ptr);
uint32_t heap_get_free_size(void);
uint32_t heap_get_used_size(void);

// Heap block structure
typedef struct heap_block {
    uint32_t size : 31;
    uint32_t used : 1;
    struct heap_block *next;
} heap_block_t;

#endif

