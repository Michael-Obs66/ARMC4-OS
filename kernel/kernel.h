#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

// Inisialisasi kernel
void kernel_init(void);

// Buat task baru (stub)
void create_task(void (*task_func)(void*), const char *name);

#endif // KERNEL_H
