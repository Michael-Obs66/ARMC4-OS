#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "task.h"

/* =============================
 *  Variabel global
 * ============================= */
extern task_t *current_task;      // Pointer ke task yang sedang berjalan

/* =============================
 *  API Scheduler
 * ============================= */
void scheduler_init(void);                 // Inisialisasi scheduler
void scheduler_add_task(task_t *task);     // Tambahkan task ke ready-queue
void scheduler_remove_task(task_t *task);  // Hapus task dari ready-queue
void schedule(void);                       // Pilih dan switch ke task berikutnya
void scheduler_tick(void);                 // Dipanggil oleh timer interrupt
void scheduler_start(void);                // Mulai eksekusi task pertama
void context_switch(task_t *next);
task_t *scheduler_get_current_task(void);  // Ambil task yang sedang running
uint32_t scheduler_get_task_count(void);   // Ambil jumlah task aktif
uint32_t kernel_get_ticks(void);
#endif /* SCHEDULER_H */

