#include <stddef.h>           // untuk NULL
#include "../include/printf.h"
#include "../include/task.h"
#include "../include/kernel.h"
#include "../include/mm.h"
#include "../include/scheduler.h"
#include "../include/uart.h"
#include "../include/oled.h"
#include "../include/i2c.h"
#include "../boards/nucleo-f446re/board_config.h"

// =============================================
//  External test functions (optional modules)
// =============================================
extern void run_all_tests(void);
extern void run_memory_tests(void);
extern void run_stress_tests(void);
extern void test_runner_task(void *arg);

// =============================================
//  Boot Banner
// =============================================
static void print_boot_banner(void)
{
    printf("\r\n");
    printf("=============================================================\r\n");
    printf("       ___    ____  __  ___   ______   _____  _____  _____   \r\n");
    printf("      / _ |  / __ \\/  |/  /  / __  /  / ___/ / ___/ / ___/   \r\n");
    printf("     / __ | / /_/ / /|_/ /  / /_/ /  / /__  (__  ) (__  )    \r\n");
    printf("    /_/ |_| \\____/_/  /_/   \\__,_/   \\___/ /____/ /____/     \r\n");
    printf("                                                             \r\n");
    printf("                 Boot SUCCESSFULLY INITIALIZED               \r\n");
    printf("                 Welcome to  ARMC4-OS  Kernel                \r\n");
    printf("=============================================================\r\n\r\n");
}

// =============================================
//  Application Tasks
// =============================================
void app_task1(void *arg)
{
    int counter = 0;
    while (1) {
        printf("App Task 1 - Counter: %d\r\n", counter++);
        task_sleep(2000);
    }
}

void app_task2(void *arg)
{
    int counter = 0;
    while (1) {
        printf("App Task 2 - Counter: %d\r\n", counter++);
        task_sleep(3000);
    }
}

void system_monitor_task(void *arg)
{
    while (1) {
        printf("\n=== System Monitor ===\r\n");
        printf("System ticks : %lu\r\n", kernel_get_ticks());
        printf("Memory used  : %lu bytes\r\n", mm_get_used());
        printf("Memory free  : %lu bytes\r\n", mm_get_free());
        printf("Active tasks : %d\r\n", scheduler_get_task_count());
        printf("=======================\r\n");
        task_sleep(10000); // tiap 10 detik
    }
}

void oled_display_task(void *arg)
{
    i2c_init();
    oled_init();
    oled_clear();
    oled_write_string("Welcome to ARMC4-OS");

    while (1) {
        oled_clear();
        oled_write_string("NUCLEO-F446RE OK");
        task_sleep(8000);
    }
}

// =============================================
//  app_main() - create tasks
// =============================================
int app_main(void)
{
    // Tampilkan banner boot di UART
    print_boot_banner();

    // Buat test runner task (opsional)
    task_create(test_runner_task, NULL, 5);

    // Buat system monitor task
    task_create(system_monitor_task, NULL, 1);

    // Buat application tasks
    task_create(app_task1, NULL, 2);
    task_create(app_task2, NULL, 3);
    task_create(oled_display_task, NULL, 4);

    return 0;
}

// =============================================
//  main() - entry point
// =============================================
int main(void)
{
    // Inisialisasi board dan peripheral (GPIO, UART, I2C, OLED, dsb)
    Board_Init();

    // Inisialisasi kernel & subsistem
    void kernel_Init(const char *message);

    // Jalankan aplikasi utama
    app_main();

    // Start scheduler multitasking
    scheduler_start();

    // Seharusnya tidak pernah kembali ke sini
    while (1);
}
