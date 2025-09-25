#include "kernel.h"
#include "task.h"

task_t* task_create(void (*func)(void*), void *arg, uint32_t priority) {
    static uint32_t task_id_counter = 0;
    
    if (task_count >= MAX_TASKS) return NULL;
    
    task_t *task = &task_table[task_count];
    task->task_func = func;
    task->arg = arg;
    task->priority = priority;
    task->state = TASK_READY;
    task->task_id = task_id_counter++;
    
    // Initialize stack
    uint32_t *stack_top = &task->stack[STACK_SIZE - 1];
    
    // Create initial context on stack
    stack_top -= sizeof(context_t) / sizeof(uint32_t);
    context_t *ctx = (context_t*)stack_top;
    
    ctx->pc = (uint32_t)func;
    ctx->lr = (uint32_t)0xFFFFFFFD; // Return to thread mode with PSP
    ctx->xpsr = 0x01000000; // Thumb state
    ctx->r0 = (uint32_t)arg;
    
    task->sp = (uint32_t)stack_top;
    
    return task;
}

void task_init(void) {
    task_count = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        task_table[i].state = TASK_SUSPENDED;
    }
}

void task_suspend(uint32_t task_id) {
    for (int i = 0; i < task_count; i++) {
        if (task_table[i].task_id == task_id) {
            task_table[i].state = TASK_SUSPENDED;
            break;
        }
    }
}

void task_resume(uint32_t task_id) {
    for (int i = 0; i < task_count; i++) {
        if (task_table[i].task_id == task_id) {
            task_table[i].state = TASK_READY;
            break;
        }
    }
}
