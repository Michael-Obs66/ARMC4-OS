#include "../kernel/kernel.h"
#include "../lib/printf.h"

void task1(void *arg)
{
    while (1) {
        printf("Task 1 running...\r\n");
        task_sleep(1000);
    }
}

void task2(void *arg)
{
    while (1) {
        printf("Task 2 running...\r\n");
        task_sleep(1500);
    }
}

void task3(void *arg)
{
    int counter = 0;
    while (1) {
        printf("Task 3 counter: %d\r\n", counter++);
        task_sleep(2000);
    }
}

int main(void)
{
    // Kernel will start automatically
    
    // Create application tasks
    task_create(task1, NULL, 1);
    task_create(task2, NULL, 2);
    task_create(task3, NULL, 3);
    
    return 0;
}
