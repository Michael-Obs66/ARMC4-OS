#include <stddef.h>
#include "../include/task.h"
#include "../include/kernel.h"
#include "../include/mm.h"
#include "../include/string.h"
#include "../include/printf.h"
#include "../include/scheduler.h"

uint32_t next_pid = 1;                 // PID counter global
task_t task_table[MAX_TASKS];          // tabel task global

void task_init(void)
{
    memset(task_table, 0, sizeof(task_table));

    for (int i = 0; i < MAX_TASKS; i++) {
        task_table[i].state = TASK_FREE;
        task_table[i].next  = NULL;
        task_table[i].prev  = NULL;
        task_table[i].sp    = NULL;
        task_table[i].stack_ptr = NULL;
        task_table[i].sleep_ticks = 0;
    }

    next_pid = 1;
}

/* -------------------------------------------------------------
   Membuat task baru
--------------------------------------------------------------*/
task_t *task_create(void (*entry)(void*), void *arg, uint8_t priority)
{
    // Cari slot kosong
    task_t *task = NULL;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].state == TASK_FREE) {
            task = &task_table[i];
            break;
        }
    }
    if (!task) return NULL;

    // Inisialisasi atribut dasar
    task->pid          = next_pid++;
    task->priority     = priority;
    task->state        = TASK_READY;
    task->time_slice   = TASK_TIME_SLICE;
    task->sleep_ticks  = 0;
    task->next         = NULL;
    task->prev         = NULL;

    // Alokasi stack
    task->stack_ptr = (uint32_t*)kmalloc(TASK_STACK_SIZE * sizeof(uint32_t));
    if (!task->stack_ptr) return NULL;

    /* -------------------------------------------------------------
       Stack diatur agar kompatibel dengan exception return Cortex-M4:
       Frame otomatis (xPSR, PC, LR, R12, R3, R2, R1, R0)
       + R4-R11 disimpan manual oleh context switch
    --------------------------------------------------------------*/
    uint32_t *sp = task->stack_ptr + TASK_STACK_SIZE - 16;

    sp[0]  = 0x01000000;          // xPSR: bit Thumb wajib 1
    sp[1]  = (uint32_t)entry;     // PC: alamat fungsi task
    sp[2]  = 0xFFFFFFFD;          // LR: return ke Thread mode (PSP)
    sp[3]  = 0;                   // R12
    sp[4]  = 0;                   // R3
    sp[5]  = 0;                   // R2
    sp[6]  = 0;                   // R1
    sp[7]  = (uint32_t)arg;       // R0: argumen untuk task

    for (int i = 8; i < 16; i++)  // R4-R11
        sp[i] = 0;

    task->sp = sp;

    // Tambah ke scheduler
    scheduler_add_task(task);

    printf("Task %u created (prio=%u)\r\n", task->pid, task->priority);
    return task;
}

/* -------------------------------------------------------------
   Mengakhiri task saat ini
--------------------------------------------------------------*/
void task_exit(void)
{
    task_t *current = scheduler_get_current_task();

    if (current) {
        current->state = TASK_ZOMBIE;
        scheduler_remove_task(current);

        if (current->stack_ptr) {
            kfree(current->stack_ptr);
            current->stack_ptr = NULL;
        }

        printf("Task %u exited\r\n", current->pid);
    }

    schedule();  // pilih task berikutnya
}

/* -------------------------------------------------------------
   Menidurkan task
--------------------------------------------------------------*/
void task_sleep(uint32_t ticks)
{
    task_t *current = scheduler_get_current_task();

    if (current) {
        current->sleep_ticks = ticks;
        current->state       = TASK_SLEEPING;
        scheduler_remove_task(current);
        schedule();   // beralih ke task lain
    }
}

/* -------------------------------------------------------------
   Membangunkan task yang sedang tidur
--------------------------------------------------------------*/
void task_wakeup_sleeping(void)
{
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].state == TASK_SLEEPING) {
            if (task_table[i].sleep_ticks > 0)
                task_table[i].sleep_ticks--;

            if (task_table[i].sleep_ticks == 0) {
                task_table[i].state = TASK_READY;
                scheduler_add_task(&task_table[i]);
            }
        }
    }
}

/* -------------------------------------------------------------
   Mencari task berdasarkan PID
--------------------------------------------------------------*/
task_t *task_get_by_pid(uint32_t pid)
{
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_table[i].pid == pid && task_table[i].state != TASK_FREE)
            return &task_table[i];
    }
    return NULL;
}
