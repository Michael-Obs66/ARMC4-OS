#include "kernel.h"
#include "task.h"
#include "gpio.h"

// contoh task blinky
void blinky_task(void *arg) {
    (void)arg; // menghindari warning unused parameter

    while (1) {
        gpio_toggle_pin(5); // toggle LED
        for (volatile int i=0; i<100000; i++); // delay sederhana
        task_yield();
    }
}

// main system stub
int main(void) {
    // inisialisasi hardware
    gpio_init();

    // kernel init
    kernel_init();

    // buat task blinky
    create_task(blinky_task, NULL, 1);

    // start scheduler
    kernel_start();

    while (1);
    return 0;
}
