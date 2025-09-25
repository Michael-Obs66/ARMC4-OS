#ifndef TASK_H
#define TASK_H

#include <stdint.h>

typedef struct {
    uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
} context_t;

void task_init(void);
task_t* task_create(void (*func)(void*), void *arg, uint32_t priority);
void task_suspend(uint32_t task_id);
void task_resume(uint32_t task_id);
void task_yield(void);

#endif