#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

#define MAX_TASKS 8
#define STACK_SIZE 1024
#define TICK_RATE 1000

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SUSPENDED
} task_state_t;

typedef struct {
    uint32_t sp;
    void (*task_func)(void*);
    void *arg;
    task_state_t state;
    uint32_t priority;
    uint32_t stack[STACK_SIZE];
    uint32_t task_id;
} task_t;

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