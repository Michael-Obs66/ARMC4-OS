#include "kernel.h"
#include "task.h"
#include "drivers/gpio.h" // contoh untuk LED

// Contoh task: Blinky LED
void blinky_task(void *arg) {
    (void)arg; // hapus warning unused
    while (1) {
        gpio_toggle_pin(5); // misal LED di pin 5
        for (volatile int i = 0; i < 1000000; i++); // delay kasar
        task_yield(); // beri kesempatan task lain
    }
}

int main(void) {
    // Inisialisasi kernel dan driver
    kernel_init();
    gpio_init(); // init GPIO

    // Buat task
    create_task(blinky_task, NULL, 1);

    // Mulai scheduler
    kernel_start();

    // Main seharusnya tidak kembali
    while (1);
}
