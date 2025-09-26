#include "kernel.h"

// Global variables
task_t task_table[MAX_TASKS];
task_t *current_task = NULL;
uint32_t task_count = 0;

void kernel_init(void) {
    // Init kernel-related peripherals
}

void kernel_start(void) {
    if (task_count > 0) {
        current_task = &task_table[0];
        current_task->state = TASK_RUNNING;
        scheduler();
    }
}


