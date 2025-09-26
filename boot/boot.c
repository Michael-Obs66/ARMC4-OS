#include "kernel/kernel.h"

// System stub, seperti yang kita bahas
void SystemInit(void) {
    // Setup clock jika perlu
}

// Fungsi main μKernelOS
int main(void) {
    SystemInit();
    kernel_init();

    // Buat contoh task
    create_task(NULL, "example_task");

    while(1) {
        // Loop utama kernel, bisa ditambahkan scheduler di sini nanti
    }

    return 0;
}
