#include <stddef.h>
#include "../include/task.h"
#include "../include/kernel.h"
#include "../include/mm.h"
#include "../include/string.h"
#include "../include/printf.h"
#include "../include/scheduler.h"

uint32_t next_pid = 1;                  
task_t task_table[MAX_TASKS];           

void task_init(void)
{
    memset(task_table, 0, sizeof(task_table));
    for (int i = 0; i < MAX_TASKS; i++) {
        task_table[i].state = TASK_FREE;
        task_table[i].next = NULL;
        task_table[i].prev = NULL;
    }
    next_pid = 1;
}

task_t *task_create(void (*entry)(void*), void *arg, uint8_t priority)
{
    // Cari slot task kosong
    task_t *task = NULL;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].state == TASK_FREE) {
            task = &task_table[i];
            break;
        }
    }
    if (task == NULL) return NULL;

    // Inisialisasi struktur task
    task->pid       = next_pid++;
    task->priority  = priority;
    task->state     = TASK_READY;
    task->time_slice = TASK_TIME_SLICE;
    task->sleep_ticks = 0;
    task->next = NULL;
    task->prev = NULL;

    // Alokasi stack
    task->stack_ptr = (uint32_t*)kmalloc(TASK_STACK_SIZE * sizeof(uint32_t));
    if (task->stack_ptr == NULL) return NULL;

    // Inisialisasi stack frame untuk context switch
    uint32_t *sp = task->stack_ptr + TASK_STACK_SIZE - 16;

    sp[0]  = 0x01000000;            // xPSR (Thumb bit)
    sp[1]  = (uint32_t)entry;       // PC (alamat fungsi task)
    sp[2]  = 0xFFFFFFFD;            // LR (return ke thread mode, PSP)
    sp[3]  = 0;                     // R12
    sp[4]  = 0;                     // R3
    sp[5]  = 0;                     // R2
    sp[6]  = 0;                     // R1
    sp[7]  = (uint32_t)arg;         // R0 (argumen)

    // Sisa register R4-R11
    for (int i = 8; i < 16; i++)
        sp[i] = 0;

    task->sp = sp;

    // Tambahkan ke scheduler
    scheduler_add_task(task);
    printf("Task %d created\r\n", task->pid);
    return task;
}

void task_exit(void)
{
    task_t *current = scheduler_get_current_task();
    if (current != NULL) {
        current->state = TASK_ZOMBIE;
        scheduler_remove_task(current);

        // Bebaskan stack agar tidak leak
        if (current->stack_ptr) {
            kfree(current->stack_ptr);
            current->stack_ptr = NULL;
        }

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
        scheduler_remove_task(current);
        schedule();
    }
}

void task_wakeup_sleeping(void)
{
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].state == TASK_SLEEPING) {
            if (--task_table[i].sleep_ticks == 0) {
                task_table[i].state = TASK_READY;
                scheduler_add_task(&task_table[i]);
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
