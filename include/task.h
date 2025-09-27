#ifndef TASK_H
#define TASK_H

#include <stdint.h>

// Konstanta untuk task
#define MAX_TASKS        16
#define TASK_STACK_SIZE  256
#define TASK_TIME_SLICE  10

// Status task
typedef enum {
    TASK_FREE = 0,
    TASK_READY,
    TASK_RUNNING,
    TASK_SLEEPING,
    TASK_BLOCKED,
    TASK_ZOMBIE
} task_state_t; 

// Struktur task
typedef struct task {
    uint32_t pid;
    uint8_t  priority;
    task_state_t state;

    uint32_t *sp;                // Stack pointer saat context switch
    uint32_t *stack_ptr;         // Alamat awal stack
    uint32_t time_slice;         // Time slice untuk round-robin
    uint32_t sleep_ticks;        // Tick untuk mode sleep/delay

    struct task *next;           // Pointer ke task berikutnya di ready-queue
    struct task *prev;           // Pointer ke task sebelumnya di ready-queue
} task_t;

// Deklarasi tabel task global
extern task_t task_table[MAX_TASKS];

// API task
void task_init(void);
task_t *task_create(void (*entry)(void *), void *arg, uint8_t priority);
void task_exit(void);
void task_sleep(uint32_t ticks);
void task_wakeup_sleeping(void);
task_t *task_get_by_pid(uint32_t pid);

#endif // TASK_H
