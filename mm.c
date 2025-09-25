#include <stdint.h>
#include <stddef.h>

#define HEAP_START 0x20001000
#define HEAP_END   0x2000F000
#define BLOCK_SIZE 64

typedef struct block {
    struct block *next;
    size_t size;
    int free;
} block_t;

static block_t *free_list = NULL;

void mm_init(void) {
    free_list = (block_t*)HEAP_START;
    free_list->size = HEAP_END - HEAP_START - sizeof(block_t);
    free_list->free = 1;
    free_list->next = NULL;
}

void* kmalloc(size_t size) {
    block_t *current = free_list;
    block_t *previous = NULL;
    
    while (current) {
        if (current->free && current->size >= size) {
            if (current->size > size + sizeof(block_t) + BLOCK_SIZE) {
                // Split block
                block_t *new_block = (block_t*)((char*)current + sizeof(block_t) + size);
                new_block->size = current->size - size - sizeof(block_t);
                new_block->free = 1;
                new_block->next = current->next;
                
                current->size = size;
                current->next = new_block;
            }
            current->free = 0;
            return (void*)((char*)current + sizeof(block_t));
        }
        previous = current;
        current = current->next;
    }
    return NULL;
}

void kfree(void *ptr) {
    if (!ptr) return;
    
    block_t *block = (block_t*)((char*)ptr - sizeof(block_t));
    block->free = 1;
    
    // Merge with next block if free
    if (block->next && block->next->free) {
        block->size += sizeof(block_t) + block->next->size;
        block->next = block->next->next;
    }
}