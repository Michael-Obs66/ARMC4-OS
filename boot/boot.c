#include "kernel.h"

// System stub, seperti yang kita bahas
void SystemInit(void) {
    // Setup clock jika perlu
}

// Fungsi main μKernelOS
int main(void) {
    SystemI#include "kernel.h"
#include "task.h"

// Contoh task sederhana
void example_task(void *arg) {
    while(1) {
        // Bisa ditambahkan aksi LED blink atau delay
        task_yield();  // beri kesempatan scheduler menjalankan task lain
    }
}

int main(void) {
    // Inisialisasi kernel
    kernel_init();

    // Buat task dengan prioritas 1
    task_create(example_task, NULL, 1);

    // Mulai scheduler
    kernel_start();

    // Main loop kernel (tidak akan kembali)
    while(1);
}

