#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "task.h"


extern task_t *current_task;  
void scheduler_init(void);
void scheduler_add_task(task_t *task);
void scheduler_remove_task(task_t *task);
void schedule(void);
void scheduler_tick(void);
void scheduler_start(void);
task_t *scheduler_get_current_task(void);
uint32_t scheduler_get_task_count(void);

#endif
