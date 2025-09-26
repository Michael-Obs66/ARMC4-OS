#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS 8
#define STACK_SIZE 1024

typedef enum { TASK_READY, TASK_RUNNING, TASK_BLOCKED, TASK_SUSPENDED } task_state_t;

typedef struct {
    uint32_t sp;
    void (*task_func)(void*);
    void *arg;
    task_state_t state;
    uint32_t priority;
    uint32_t stack[STACK_SIZE];
    uint32_t task_id;
} task_t;

void task_init(void);
task_t* task_create(void (*func)(void*), void *arg, uint32_t priority);
void task_suspend(uint32_t task_id);
void task_resume(uint32_t task_id);
void task_yield(void);

#endif




