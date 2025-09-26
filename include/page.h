#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

// Page management functions
void page_init(void);
void *page_alloc(void);
void page_free(void *page);
uint32_t page_get_free_count(void);
uint32_t page_get_used_count(void);

// Page constants
#define PAGE_SIZE 4096
#define PAGE_COUNT (HEAP_SIZE / PAGE_SIZE)

// Page frame structure
typedef struct {
    uint32_t used : 1;
    uint32_t reserved : 31;
} page_t;

#endif
