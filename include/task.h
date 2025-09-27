#ifndef TASK_H
#define TASK_H

#include <stdint.h>

/* =============================
 *  Konstanta konfigurasi task
 * ============================= */
#define MAX_TASKS        16          // Jumlah maksimum task
#define TASK_STACK_SIZE  256         // Ukuran stack tiap task (dalam word)
#define TASK_TIME_SLICE  10          // Default time slice untuk scheduler RR

/* =============================
 *  Status task
 * ============================= */
typedef enum {
    TASK_FREE = 0,       // Slot kosong, belum digunakan
    TASK_READY,          // Siap dijalankan
    TASK_RUNNING,        // Sedang berjalan
    TASK_SLEEPING,       // Tertidur (delay)
    TASK_BLOCKED,        // Diblokir karena menunggu resource
    TASK_ZOMBIE          // Selesai, menunggu pembersihan
} task_state_t;

/* =============================
 *  Struktur task
 * ============================= */
typedef struct task {
    uint32_t pid;                // ID unik untuk task
    uint8_t  priority;           // Prioritas task
    task_state_t state;          // Status task saat ini

    uint32_t *sp;                // Stack pointer untuk context switch
    uint32_t *stack_ptr;         // Alamat awal stack yang dialokasikan
    uint32_t time_slice;         // Time slice untuk round-robin
    uint32_t sleep_ticks;        // Sisa tick untuk sleep/delay

    struct task *next;           // Pointer ke task berikutnya (ready queue)
    struct task *prev;           // Pointer ke task sebelumnya (ready queue)
} task_t;

/* =============================
 *  API Task
 * ============================= */
void task_switch_to(task_t *task); 

void task_init(void);
task_t *task_create(void (*entry)(void *), void *arg, uint8_t priority);
void task_exit(void);
void task_sleep(uint32_t ticks);
void task_wakeup_sleeping(void);
task_t *task_get_by_pid(uint32_t pid);

#endif /* TASK_H */
