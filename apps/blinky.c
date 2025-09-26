#include "kernel.h"
#include "drivers.h"
#include "stm32f4xx.h" 

void blinky_task(void *arg) {
    int led_pin = (int)arg;
    gpio_set_direction(led_pin, 1);
    
    int state = 0;
    uint32_t counter = 0;
    
    while (1) {
        gpio_write(led_pin, state);
        state = !state;
        
        counter++;
        
        // Every 100 blinks, print status
        if (counter % 100 == 0) {
            uart_puts("Blinky task running - count: ");
            // You'd need itoa function here
            uart_puts("\r\n");
        }
        
        // Non-blocking delay using task yield
        for (int i = 0; i < 500; i++) {
            task_yield();
        }
    }
}

void blinky_init(void) {
    // Create multiple blinky tasks with different LEDs
    create_task(blinky_task, (void*)13, 1);  // LED 1
    create_task(blinky_task, (void*)14, 2);  // LED 2
    create_task(blinky_task, (void*)15, 3);  // LED 3
}
