#include <stdint.h>

#define HEAP_SIZE 0x10000
static uint8_t heap[HEAP_SIZE];
static uint32_t heap_ptr = 0;

void* simple_malloc(size_t size) {
    if (heap_ptr + size > HEAP_SIZE) {
        return NULL;
    }
    
    void *ptr = &heap[heap_ptr];
    heap_ptr += size;
    return ptr;
}

void heap_reset(void) {
    heap_ptr = 0;
}
