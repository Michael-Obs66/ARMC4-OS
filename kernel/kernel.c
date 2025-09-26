#include "kernel.h"
#include <string.h> // untuk memcpy stub

// Stub kernel_init
void kernel_init(void) {
    // Kosong, nanti implementasi asli
}

// Stub create_task
void create_task(void (*task_func)(void*), const char *name) {
    (void)task_func;
    (void)name;
    // Hanya stub, tidak melakukan apa-apa
}
