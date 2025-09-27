#include <stddef.h>
#include "../include/kernel.h"
#include "../include/task.h"
#include "../include/scheduler.h"
#include "../include/printf.h"
#include "../include/mm.h"       // untuk kfree()

// Variabel global agar bisa diakses context_switch.s
task_t *current_task = NULL;

// Queue siap jalan (circular doubly-linked list)
static task_t *ready_queue = NULL;
static uint32_t task_count = 0;


/* ---------------- Scheduler Core ---------------- */

void scheduler_init(void)
{
    current_task = NULL;
    ready_queue = NULL;
    task_count = 0;
}

void scheduler_add_task(task_t *task)
{
    if (ready_queue == NULL) {
        ready_queue = task;
        task->next = task;
        task->prev = task;
    } else {
        task->next = ready_queue;
        task->prev = ready_queue->prev;
        ready_queue->prev->next = task;
        ready_queue->prev = task;
    }
    task_count++;
}

void scheduler_remove_task(task_t *task)
{
    if (task->next == task) {
        ready_queue = NULL;
    } else {
        task->prev->next = task->next;
        task->next->prev = task->prev;
        if (ready_queue == task) {
            ready_queue = task->next;
        }
    }
    task_count--;
}

/* Pilih task berikutnya (round-robin) */
task_t *pick_next_task(void)
{
    if (ready_queue == NULL)
        return NULL;

    task_t *next = ready_queue;
    ready_queue  = ready_queue->next;
    return next;
}


/* -------------- Scheduler API ------------------ */

void schedule(void)
{
    // Bersihkan task zombie
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].state == TASK_ZOMBIE) {
            if (task_table[i].stack_ptr) {
                kfree(task_table[i].stack_ptr);
                task_table[i].stack_ptr = NULL;
            }
            task_table[i].state = TASK_FREE;
        }
    }

    // Pilih task berikutnya
    task_t *next = pick_next_task();
    if (next != NULL) {
        context_switch(next);      // ganti ke task baru
    }
}

void scheduler_tick(void)
{
    if (current_task != NULL) {
        if (--current_task->time_slice == 0) {
            current_task->time_slice = TASK_TIME_SLICE;
            schedule();             // pre-empt
        }
    }
}

void scheduler_start(void)
{
    if (ready_queue != NULL) {
        current_task = ready_queue;
        ready_queue  = ready_queue->next;
        task_switch_to(current_task);   // ganti ke task pertama
    }
}

task_t *scheduler_get_current_task(void)
{
    return current_task;
}

uint32_t scheduler_get_task_count(void)
{
    return task_count;
}


/* --------- Dummy untuk context switch ---------- */
/* NOTE: Ganti dengan ASM di context_switch.s nanti */

void context_switch(task_t *next)
{
    current_task = next;
    // TODO: isi mekanisme switch register di assembly
}
