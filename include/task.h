#ifndef TASK_H
#define TASK_H

#include "kernel.h"   // biar task_t dikenali
#include <stdint.h>

void task_init(void);
task_t* task_create(void (*func)(void*), void *arg, uint32_t priority);
void task_suspend(uint32_t task_id);
void task_resume(uint32_t task_id);
void task_yield(void);

#endif


