#include "kernel.h"
#include "task.h"
#include "stm32f4xx.h" 

task_t *current_task = NULL;
task_t task_table[MAX_TASKS];
uint32_t task_count = 0;
volatile uint32_t systick_count = 0;

void kernel_init(void) {
    task_init();
    
    // Configure SysTick timer for 1ms interrupts
    uint32_t *STK_LOAD = (uint32_t*)0xE000E014;
    uint32_t *STK_CTRL = (uint32_t*)0xE000E010;
    
    *STK_LOAD = (SystemCoreClock / TICK_RATE) - 1;
    *STK_CTRL = 0x07; // Enable SysTick with processor clock
    
    // Enable interrupts
    __asm volatile ("cpsie i");
}

void kernel_start(void) {
    if (task_count > 0) {
        current_task = &task_table[0];
        current_task->state = TASK_RUNNING;
        
        // Start the first task
        __asm volatile (
            "msr psp, %0\n"
            "mov r0, #0x02\n"
            "msr control, r0\n"
            "isb\n"
            "bx lr" 
            : : "r" (current_task->sp) : "r0"
        );
    }
}

void create_task(void (*task)(void*), void *arg, uint32_t priority) {
    if (task_count < MAX_TASKS) {
        task_t *new_task = task_create(task, arg, priority);
        if (new_task) {
            task_table[task_count++] = *new_task;
        }
    }
}

void SysTick_Handler(void) {
    systick_count++;
    scheduler();
}

