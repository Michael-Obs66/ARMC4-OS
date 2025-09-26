#include "task.h"
#include "kernel.h"
#include "../mm/mm.h"
#include "../lib/string.h"
#include "../lib/printf.h"

static uint32_t next_pid = 1;
static task_t task_table[MAX_TASKS];

void task_init(void)
{
    memset(task_table, 0, sizeof(task_table));
}

task_t *task_create(void (*entry)(void*), void *arg, uint8_t priority)
{
    // Find free task slot
    task_t *task = NULL;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].state == TASK_FREE) {
            task = &task_table[i];
            break;
        }
    }
    
    if (task == NULL) {
        return NULL;
    }
    
    // Initialize task structure
    task->pid = next_pid++;
    task->priority = priority;
    task->state = TASK_READY;
    task->time_slice = TASK_TIME_SLICE;
    task->stack_ptr = (uint32_t*)kmalloc(TASK_STACK_SIZE);
    
    if (task->stack_ptr == NULL) {
        return NULL;
    }
    
    // Initialize stack for context switching
    uint32_t *sp = task->stack_ptr + TASK_STACK_SIZE - 16;
    
    // Initialize stack frame for exception return
    sp[0] = 0x01000000; // xPSR
    sp[1] = (uint32_t)entry; // PC
    sp[2] = 0xFFFFFFFE; // LR
    sp[3] = 0; // R12
    sp[4] = 0; // R3
    sp[5] = 0; // R2
    sp[6] = 0; // R1
    sp[7] = (uint32_t)arg; // R0
    
    // Remaining registers
    sp[8] = 0; // R11
    sp[9] = 0; // R10
    sp[10] = 0; // R9
    sp[11] = 0; // R8
    sp[12] = 0; // R7
    sp[13] = 0; // R6
    sp[14] = 0; // R5
    sp[15] = 0; // R4
    
    task->sp = sp;
    
    // Add to scheduler
    scheduler_add_task(task);
    
    printf("Task %d created\r\n", task->pid);
    
    return task;
}

void task_exit(void)
{
    task_t *current = scheduler_get_current_task();
    if (current != NULL) {
        current->state = TASK_FREE;
        kfree(current->stack_ptr);
        scheduler_remove_task(current);
        printf("Task %d exited\r\n", current->pid);
    }
    schedule();
}

void task_sleep(uint32_t ticks)
{
    task_t *current = scheduler_get_current_task();
    if (current != NULL) {
        current->sleep_ticks = ticks;
        current->state = TASK_SLEEPING;
        schedule();
    }
}

void task_wakeup_sleeping(void)
{
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].state == TASK_SLEEPING) {
            if (--task_table[i].sleep_ticks == 0) {
                task_table[i].state = TASK_READY;
            }
        }
    }
}

task_t *task_get_by_pid(uint32_t pid)
{
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].pid == pid && task_table[i].state != TASK_FREE) {
            return &task_table[i];
        }
    }
    return NULL;
}
