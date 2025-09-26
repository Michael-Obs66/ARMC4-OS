#include "kernel.h"
#include "task.h"
#include "core_cm4.h"

static uint32_t current_task_index = 0;

void scheduler(void) {
    if (task_count == 0) return;
    
    // Simple round-robin scheduling
    current_task_index = (current_task_index + 1) % task_count;
    task_t *next_task = &task_table[current_task_index];
    
    if (next_task->state == TASK_READY) {
        if (current_task != next_task) {
            task_t *prev_task = current_task;
            current_task = next_task;
            current_task->state = TASK_RUNNING;
            prev_task->state = TASK_READY;
            
            context_switch();
        }
    }
}

void task_yield(void) {
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

