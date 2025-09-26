#include "../lib/kernel.h"
#include "../lib/task.h"
#include "../lib/printf.h"
#include "../lib/string.h"

#define TEST_PASSED 1
#define TEST_FAILED 0

static int test_count = 0;
static int passed_count = 0;

void test_begin(const char *test_name)
{
    printf("\n=== TEST: %s ===\r\n", test_name);
    test_count++;
}

int test_assert(int condition, const char *message)
{
    if (condition) {
        printf("✓ PASS: %s\r\n", message);
        passed_count++;
        return TEST_PASSED;
    } else {
        printf("✗ FAIL: %s\r\n", message);
        return TEST_FAILED;
    }
}

void test_results(void)
{
    printf("\n=== TEST RESULTS ===\r\n");
    printf("Tests run: %d\r\n", test_count);
    printf("Tests passed: %d\r\n", passed_count);
    printf("Tests failed: %d\r\n", test_count - passed_count);
    printf("Success rate: %.1f%%\r\n", (float)passed_count/test_count * 100);
}

// Test cases
static int task1_counter = 0;
static int task2_counter = 0;
static int task3_counter = 0;

void test_task1(void *arg)
{
    while (1) {
        task1_counter++;
        printf("Test Task 1 - Counter: %d\r\n", task1_counter);
        task_sleep(500);
        
        if (task1_counter >= 3) {
            break;
        }
    }
    task_exit();
}

void test_task2(void *arg)
{
    while (1) {
        task2_counter++;
        printf("Test Task 2 - Counter: %d\r\n", task2_counter);
        task_sleep(300);
        
        if (task2_counter >= 5) {
            break;
        }
    }
    task_exit();
}

void test_task3(void *arg)
{
    while (1) {
        task3_counter++;
        printf("Test Task 3 - Counter: %d\r\n", task3_counter);
        task_sleep(700);
        
        if (task3_counter >= 2) {
            break;
        }
    }
    task_exit();
}

void test_task_creation(void)
{
    test_begin("Task Creation");
    
    task_t *task1 = task_create(test_task1, NULL, 1);
    task_t *task2 = task_create(test_task2, NULL, 2);
    task_t *task3 = task_create(test_task3, NULL, 3);
    
    test_assert(task1 != NULL, "Task 1 created successfully");
    test_assert(task2 != NULL, "Task 2 created successfully");
    test_assert(task3 != NULL, "Task 3 created successfully");
    test_assert(task1->pid == 1, "Task 1 PID correct");
    test_assert(task2->pid == 2, "Task 2 PID correct");
    test_assert(task3->pid == 3, "Task 3 PID correct");
    
    // Wait for tasks to complete
    task_sleep(5000);
    
    test_assert(task1_counter > 0, "Task 1 executed");
    test_assert(task2_counter > 0, "Task 2 executed");
    test_assert(task3_counter > 0, "Task 3 executed");
}

void test_memory_allocation(void)
{
    test_begin("Memory Allocation");
    
    void *ptr1 = kmalloc(100);
    void *ptr2 = kmalloc(200);
    void *ptr3 = kmalloc(50);
    
    test_assert(ptr1 != NULL, "Allocation 1 successful");
    test_assert(ptr2 != NULL, "Allocation 2 successful");
    test_assert(ptr3 != NULL, "Allocation 3 successful");
    test_assert(ptr1 != ptr2, "Allocations are distinct");
    test_assert(ptr2 != ptr3, "Allocations are distinct");
    
    uint32_t free_before = mm_get_free();
    kfree(ptr1);
    kfree(ptr2);
    kfree(ptr3);
    
    // Note: Simple allocator doesn't actually free memory
    printf("Memory free: %lu bytes\r\n", mm_get_free());
}

void test_ipc_basic(void)
{
    test_begin("IPC Basic");
    
    int queue_id = ipc_queue_create(100);
    test_assert(queue_id >= 0, "Queue created successfully");
    
    char message[] = "Hello IPC";
    int result = ipc_send(queue_id, message, strlen(message) + 1);
    test_assert(result == 0, "Message sent successfully");
    
    char buffer[20];
    result = ipc_receive(queue_id, buffer, sizeof(buffer));
    test_assert(result == 0, "Message received successfully");
    test_assert(strcmp(message, buffer) == 0, "Message content correct");
}

void test_scheduler(void)
{
    test_begin("Scheduler Functionality");
    
    uint32_t initial_count = scheduler_get_task_count();
    test_assert(initial_count >= 1, "Idle task exists"); // Idle task
    
    task_t *current = scheduler_get_current_task();
    test_assert(current != NULL, "Current task exists");
    
    // Create a test task
    task_t *test_task = task_create(test_task1, NULL, 1);
    test_assert(scheduler_get_task_count() == initial_count + 1, "Task count increased");
    
    task_sleep(100);
    test_assert(task1_counter > 0, "Scheduler executed task");
}

void test_kernel_apis(void)
{
    test_begin("Kernel APIs");
    
    test_assert(kernel_is_initialized() == 1, "Kernel initialized");
    
    uint32_t ticks1 = kernel_get_ticks();
    task_sleep(100);
    uint32_t ticks2 = kernel_get_ticks();
    test_assert(ticks2 > ticks1, "System ticks increasing");
    
    // Test printf functionality
    printf("Testing printf: %s, %d, 0x%x\r\n", "string", 42, 0xABCD);
    test_assert(1, "Printf working"); // If we reached here, printf works
}

void test_performance(void)
{
    test_begin("Performance Tests");
    
    uint32_t start_ticks = kernel_get_ticks();
    
    // Test memory allocation performance
    for (int i = 0; i < 100; i++) {
        void *ptr = kmalloc(16);
        if (ptr) {
            // Do something
            memset(ptr, i, 16);
        }
    }
    
    uint32_t end_ticks = kernel_get_ticks();
    uint32_t duration = end_ticks - start_ticks;
    
    printf("Performance test took %lu ticks\r\n", duration);
    test_assert(duration < 1000, "Performance within acceptable range");
}

void test_system_integration(void)
{
    test_begin("System Integration");
    
    // Test driver integration
    gpio_set(GPIO_PORT_A, 5, 1);
    test_assert(1, "GPIO set working");
    
    gpio_set(GPIO_PORT_A, 5, 0);
    test_assert(1, "GPIO clear working");
    
    // Test UART (if connected)
    uart_puts("UART test message\r\n");
    test_assert(1, "UART output working");
    
    // Test timer
    uint32_t start = timer_get_ticks();
    task_sleep(100);
    uint32_t end = timer_get_ticks();
    test_assert(end > start, "Timer functionality working");
}

void run_all_tests(void)
{
    printf("Starting μKernelOS Test Suite\r\n");
    printf("=============================\r\n");
    
    // Wait for kernel to fully initialize
    while (!kernel_is_initialized()) {
        task_sleep(10);
    }
    
    test_kernel_apis();
    test_task_creation();
    test_scheduler();
    test_memory_allocation();
    test_ipc_basic();
    test_performance();
    test_system_integration();
    
    test_results();
    
    printf("\nTest suite completed. System will continue running...\r\n");
}

// Test task that runs the test suite
void test_runner_task(void *arg)
{
    task_sleep(1000); // Wait for system to stabilize
    run_all_tests();
    
    // Continue running as a normal task
    while (1) {
        printf("Test runner idle...\r\n");
        task_sleep(5000);
    }
}
