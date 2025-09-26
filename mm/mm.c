#include <stddef.h>
#include "../include/mm.h"
#include "../include/printf.h"

#define HEAP_SIZE (64 * 1024) // 64KB
static uint8_t heap[HEAP_SIZE];
static uint32_t heap_used = 0;

void mm_init(void)
{
    heap_used = 0;
    printf("Memory manager initialized. Heap size: %d bytes\r\n", HEAP_SIZE);
}

void *kmalloc(uint32_t size)
{
    // Simple sequential allocator
    if (heap_used + size > HEAP_SIZE) {
        return NULL;
    }
    
    void *ptr = &heap[heap_used];
    heap_used += size;
    return ptr;
}

void kfree(void *ptr)
{
    // Simple allocator doesn't support free
    // In real implementation, this would manage free blocks
}

uint32_t mm_get_free(void)
{
    return HEAP_SIZE - heap_used;
}

uint32_t mm_get_used(void)
{
    return heap_used;
}



