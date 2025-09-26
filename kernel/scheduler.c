#include "../lib/kernel.h"
#include "../lib/task.h"
#include "../lib/scheduler.h"
#include "../lib/printf.h"

static task_t *current_task = NULL;
static task_t *ready_queue = NULL;
static uint32_t task_count = 0;

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

void schedule(void)
{
    if (ready_queue == NULL || task_count == 0) {
        return;
    }
    
    task_t *next_task = ready_queue;
    task_t *prev_task = current_task;
    
    if (prev_task != next_task) {
        current_task = next_task;
        ready_queue = next_task->next;
        
        if (prev_task != NULL) {
            task_switch(prev_task, next_task);
        } else {
            task_switch_to(next_task);
        }
    }
}

void scheduler_tick(void)
{
    if (current_task != NULL) {
        current_task->time_slice--;
        
        if (current_task->time_slice == 0) {
            current_task->time_slice = TASK_TIME_SLICE;
            schedule();
        }
    }
}

void scheduler_start(void)
{
    if (ready_queue != NULL) {
        current_task = ready_queue;
        ready_queue = ready_queue->next;
        task_switch_to(current_task);
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


