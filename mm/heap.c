#include "../include/heap.h"

#define HEAP_SIZE       (1024 * 10) // 10 KB heap
#define HEAP_BLOCK_SIZE 32           // ukuran tiap block
#define HEAP_BLOCKS     (HEAP_SIZE / HEAP_BLOCK_SIZE)

static heap_block_t heap_blocks[HEAP_BLOCKS];

void heap_init(void) {
    for (int i = 0; i < HEAP_BLOCKS; i++) {
        heap_blocks[i].size = HEAP_BLOCK_SIZE;
        heap_blocks[i].used = 0;
        heap_blocks[i].next = (i < HEAP_BLOCKS - 1) ? &heap_blocks[i + 1] : NULL;
    }
}

void *heap_alloc(uint32_t size) {
    // implementasi sederhana
    heap_block_t *block = heap_blocks;
    while (block) {
        if (!block->used && block->size >= size) {
            block->used = 1;
            return (void *)(block + 1); // return alamat setelah header
        }
        block = block->next;
    }
    return NULL; // gagal alokasi
}

void heap_free(void *ptr) {
    if (!ptr) return;
    heap_block_t *block = ((heap_block_t *)ptr) - 1;
    block->used = 0;
}

uint32_t heap_get_free_size(void) {
    uint32_t free_size = 0;
    heap_block_t *block = heap_blocks;
    while (block) {
        if (!block->used)
            free_size += block->size;
        block = block->next;
    }
    return free_size;
}

uint32_t heap_get_used_size(void) {
    return HEAP_SIZE - heap_get_free_size();
}
