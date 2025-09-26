#include "kernel.h"
#include "task.h"

void scheduler(void) {
    if (task_count == 0) return;

    for (uint32_t i = 0; i < task_count; i++) {
        task_t *next_task = &task_table[i];

        if (next_task->state == TASK_READY) {
            if (current_task != next_task) {
                task_t *prev_task = current_task;
                current_task = next_task;
                current_task->state = TASK_RUNNING;
                if (prev_task) prev_task->state = TASK_READY;
                context_switch();
            }
        }
    }
}
