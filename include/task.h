#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS 16
#define TASK_STACK_SIZE 1024
#define TASK_TIME_SLICE 10

typedef enum {
    TASK_FREE,
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SLEEPING
} task_state_t;

typedef struct task {
    uint32_t pid;
    uint32_t *sp;
    uint32_t *stack_ptr;
    task_state_t state;
    uint8_t priority;
    uint32_t time_slice;
    uint32_t sleep_ticks;
    struct task *next;
    struct task *prev;
} task_t;

void task_init(void);
task_t *task_create(void (*entry)(void*), void *arg, uint8_t priority);
void task_exit(void);
void task_sleep(uint32_t ticks);
void task_wakeup_sleeping(void);
task_t *task_get_by_pid(uint32_t pid);

#endif
