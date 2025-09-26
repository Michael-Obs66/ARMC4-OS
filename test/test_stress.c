#include "../lib/kernel.h"
#include "../lib/task.h"
#include "../lib/printf.h"

#define STRESS_TASK_COUNT 10
#define STRESS_ITERATIONS 100

static int stress_counters[STRESS_TASK_COUNT] = {0};

void stress_task(void *arg)
{
    int task_id = (int)arg;
    
    for (int i = 0; i < STRESS_ITERATIONS; i++) {
        stress_counters[task_id]++;
        
        if (i % 10 == 0) {
            printf("Stress task %d: iteration %d\r\n", task_id, i);
        }
        
        // Varying sleep times to create different scheduling patterns
        task_sleep(50 + (task_id * 10));
    }
    
    printf("Stress task %d completed %d iterations\r\n", task_id, STRESS_ITERATIONS);
    task_exit();
}

void test_stress_scheduler(void)
{
    printf("\n=== Stress Test: Scheduler ===\r\n");
    
    uint32_t start_ticks = kernel_get_ticks();
    
    // Create many tasks to stress the scheduler
    for (int i = 0; i < STRESS_TASK_COUNT; i++) {
        task_t *task = task_create(stress_task, (void*)i, i % 5 + 1);
        if (task == NULL) {
            printf("Failed to create stress task %d\r\n", i);
        }
    }
    
    // Wait for tasks to complete
    task_sleep(10000);
    
    uint32_t end_ticks = kernel_get_ticks();
    uint32_t duration = end_ticks - start_ticks;
    
    printf("Stress test completed in %lu ticks\r\n", duration);
    
    // Verify all tasks completed
    int total_iterations = 0;
    for (int i = 0; i < STRESS_TASK_COUNT; i++) {
        total_iterations += stress_counters[i];
        printf("Task %d: %d iterations\r\n", i, stress_counters[i]);
    }
    
    int expected_total = STRESS_TASK_COUNT * STRESS_ITERATIONS;
    printf("Total iterations: %d (expected: %d)\r\n", total_iterations, expected_total);
    
    if (total_iterations == expected_total) {
        printf("✓ Stress test PASSED\r\n");
    } else {
        printf("✗ Stress test FAILED\r\n");
    }
}

void test_stress_memory(void)
{
    printf("\n=== Stress Test: Memory ===\r\n");
    
    #define NUM_ALLOCATIONS 50
    #define ALLOC_SIZE 64
    
    void *allocations[NUM_ALLOCATIONS];
    
    // Allocate many small blocks
    for (int i = 0; i < NUM_ALLOCATIONS; i++) {
        allocations[i] = kmalloc(ALLOC_SIZE);
        if (allocations[i] == NULL) {
            printf("Allocation failed at iteration %d\r\n", i);
            break;
        }
        
        // Use the memory
        memset(allocations[i], i, ALLOC_SIZE);
    }
    
    printf("Completed %d allocations of %d bytes each\r\n", NUM_ALLOCATIONS, ALLOC_SIZE);
    printf("Memory used: %lu bytes\r\n", mm_get_used());
    printf("Memory free: %lu bytes\r\n", mm_get_free());
    
    // Note: Simple allocator doesn't support free, so we just report
    printf("Memory stress test completed (allocator doesn't support free)\r\n");
}

void run_stress_tests(void)
{
    printf("Starting Stress Tests\r\n");
    printf("=====================\r\n");
    
    test_stress_scheduler();
    test_stress_memory();
    
    printf("\nStress tests completed.\r\n");
}
