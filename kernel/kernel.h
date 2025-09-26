#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include "task.h"

void kernel_init(void);
void kernel_start(void);
void create_task(void (*task)(void*), void *arg, uint32_t priority);
void scheduler(void);
void context_switch(void);
void systick_handler(void);

extern task_t *current_task;
extern task_t task_table[MAX_TASKS];
extern uint32_t task_count;

#endif

