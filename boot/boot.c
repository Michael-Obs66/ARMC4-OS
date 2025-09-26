#include "kernel.h"
#include "task.h"
#include <stdint.h>

// Contoh task sederhana
void example_task(void *arg) {
    (void)arg; // supaya warning unused parameter hilang
    while (1) {
        // tugas loop, misal toggle LED nanti
    }
}

int main(void) {
    // Inisialisasi kernel
    kernel_init();

    // Buat task
    create_task(example_task, NULL, 1);  // prioritas 1

    // Mulai scheduler
    kernel_start();

    // Tidak akan pernah sampai sini
    while (1) { }
}
