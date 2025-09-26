#include <stddef.h>
#include "../include/heap.h"

#define HEAP_BLOCK_SIZE 32
#define HEAP_BLOCKS (HEAP_SIZE / HEAP_BLOCK_SIZE)

typedef struct heap_block {
    uint32_t size : 31;
    uint32_t used : 1;
} heap_block_t;

static heap_block_t heap_blocks[HEAP_BLOCKS];

void heap_init(void)
{
    for (int i = 0; i < HEAP_BLOCKS; i++) {
        heap_blocks[i].used = 0;
        heap_blocks[i].size = HEAP_BLOCK_SIZE;
    }
}

void *heap_alloc(uint32_t size)
{
    uint32_t blocks_needed = (size + sizeof(heap_block_t) + HEAP_BLOCK_SIZE - 1) / HEAP_BLOCK_SIZE;
    
    for (int i = 0; i < HEAP_BLOCKS - blocks_needed; i++) {
        if (!heap_blocks[i].used) {
            int consecutive_free = 0;
            for (int j = i; j < i + blocks_needed; j++) {
                if (!heap_blocks[j].used) {
                    consecutive_free++;
                }
            }
            
            if (consecutive_free == blocks_needed) {
                heap_blocks[i].used = 1;
                heap_blocks[i].size = blocks_needed * HEAP_BLOCK_SIZE;
                return (void*)((uint8_t*)heap_blocks + i * HEAP_BLOCK_SIZE + sizeof(heap_block_t));
            }
        }
    }
    
    return NULL;
}

void heap_free(void *ptr)
{
    if (ptr == NULL) return;
    
    uint32_t block_index = ((uint8_t*)ptr - (uint8_t*)heap_blocks - sizeof(heap_block_t)) / HEAP_BLOCK_SIZE;
    
    if (block_index < HEAP_BLOCKS) {
        heap_blocks[block_index].used = 0;
    }
}



