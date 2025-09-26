#include <stddef.h>
#include "../include/page.h"

#define PAGE_SIZE 4096
#define PAGE_COUNT (HEAP_SIZE / PAGE_SIZE)

typedef struct {
    uint32_t used : 1;
    uint32_t reserved : 31;
} page_t;

static page_t pages[PAGE_COUNT];

void page_init(void)
{
    for (int i = 0; i < PAGE_COUNT; i++) {
        pages[i].used = 0;
    }
}

void *page_alloc(void)
{
    for (int i = 0; i < PAGE_COUNT; i++) {
        if (!pages[i].used) {
            pages[i].used = 1;
            return (void*)((uint8_t*)heap + i * PAGE_SIZE);
        }
    }
    return NULL;
}

void page_free(void *page)
{
    if (page == NULL) return;
    
    uint32_t page_index = ((uint8_t*)page - (uint8_t*)heap) / PAGE_SIZE;
    
    if (page_index < PAGE_COUNT) {
        pages[page_index].used = 0;
    }
}

uint32_t page_get_free_count(void)
{
    uint32_t count = 0;
    for (int i = 0; i < PAGE_COUNT; i++) {
        if (!pages[i].used) {
            count++;
        }
    }
    return count;
}
