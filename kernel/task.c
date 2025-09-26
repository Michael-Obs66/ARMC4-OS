#include "task.h"

task_t* task_create(void (*func)(void*), void *arg, uint32_t priority) {
    if (task_count >= MAX_TASKS) return NULL;

    task_t *t = &task_table[task_count];
    t->task_func = func;
    t->arg = arg;
    t->priority = priority;
    t->state = TASK_READY;
    t->sp = (uint32_t)(t->stack + STACK_SIZE - 16); // contoh stack pointer
    t->task_id = task_count;

    task_count++;
    return t;
}

void task_suspend(uint32_t task_id) {
    if (task_id >= task_count) return;
    task_table[task_id].state = TASK_SUSPENDED;
}

void task_resume(uint32_t task_id) {
    if (task_id >= task_count) return;
    task_table[task_id].state = TASK_READY;
}

void task_yield(void) {
    scheduler();
}
