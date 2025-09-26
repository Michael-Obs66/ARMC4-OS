#include "../include/kernel.h"
#include "../include/task.h"
#include "../include/drivers.h"
#include "../include/printf.h"
#include "../include/system.h"
#include "../include/uart.h"
#include "../include/timer.h"
#include "../include/scheduler.h"
#include "../include/mm.h"


static volatile uint32_t system_ticks = 0;
static uint8_t kernel_initialized = 0;

void kernel_main(void)
{
    // Initialize system
    SystemInit();
    uart_init();
    timer_init();
    
    printf("μKernelOS Starting...\r\n");
    
    // Initialize memory management
    mm_init();
    
    // Initialize scheduler
    scheduler_init();
    
    // Create idle task
    task_create(idle_task, NULL, 0);
    
    kernel_initialized = 1;
    
    printf("Kernel initialized. Starting scheduler...\r\n");
    
    // Start scheduler
    scheduler_start();
}

uint32_t kernel_get_ticks(void)
{
    return system_ticks;
}

uint8_t kernel_is_initialized(void)
{
    return kernel_initialized;
}

void SysTick_Handler(void)
{
    system_ticks++;
    
    if (kernel_initialized) {
        scheduler_tick();
    }
}

void idle_task(void *arg)
{
    while (1) {
        // Low power mode
        __asm volatile ("wfi");
    }
}

void kernel_panic(const char *message)
{
    printf("KERNEL PANIC: %s\r\n", message);
    while (1);
}






