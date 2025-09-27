#include <stddef.h>           // untuk NULL
#include "../include/printf.h"
#include "../include/task.h"
#include "../include/mm.h"
#include "../include/scheduler.h"

// Test functions
extern void run_all_tests(void);
extern void run_memory_tests(void);
extern void run_stress_tests(void);
extern void test_runner_task(void *arg);

// Example application tasks
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
        printf("System ticks: %lu\r\n", kernel_get_ticks());
        printf("Memory used: %lu bytes\r\n", mm_get_used());
        printf("Memory free: %lu bytes\r\n", mm_get_free());
        printf("Active tasks: %d\r\n", scheduler_get_task_count());
        printf("=====================\r\n");
        
        task_sleep(10000); // Report every 10 seconds
    }
}

int apps_main(void)
{
    // Kernel will start automatically
    
    // Create test runner task (runs tests automatically)
    task_create(test_runner_task, NULL, 5);
    
    // Create system monitor task
    task_create(system_monitor_task, NULL, 1);
    
    // Create application tasks
    task_create(app_task1, NULL, 2);
    task_create(app_task2, NULL, 3);
    
    // Uncomment to run specific test suites manually:
    // run_all_tests();
    // run_memory_tests();
    // run_stress_tests();
    
    return 0;
}
