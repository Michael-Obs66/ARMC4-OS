
#include "../include/page.h"
#include "../include/heap.h"   // Untuk HEAP_SIZE, karena PAGE_COUNT tergantung HEAP_SIZE

// Array untuk semua page
static page_t pages[PAGE_COUNT];

void page_init(void) {
    for (uint32_t i = 0; i < PAGE_COUNT; i++) {
        pages[i].used = 0;
        pages[i].reserved = 0;
    }
}

void *page_alloc(void) {
    for (uint32_t i = 0; i < PAGE_COUNT; i++) {
        if (!pages[i].used) {
            pages[i].used = 1;
            return (void *)(i * PAGE_SIZE); // alamat page logis
        }
    }
    return NULL; // Tidak ada page kosong
}

void page_free(void *page) {
    if (!page) return;

    uint32_t index = (uint32_t)page / PAGE_SIZE;
    if (index < PAGE_COUNT) {
        pages[index].used = 0;
        pages[index].reserved = 0;
    }
}

uint32_t page_get_free_count(void) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < PAGE_COUNT; i++) {
        if (!pages[i].used) count++;
    }
    return count;
}

uint32_t page_get_used_count(void) {
    return PAGE_COUNT - page_get_free_count();
}
