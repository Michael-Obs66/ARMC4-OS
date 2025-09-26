#include <stddef.h>
#include "../include/kernel.h"
#include "../include/mm.h"
#include "../include/printf.h"
#include "../include/string.h"

#define MEMORY_TEST_SIZE 1024

void test_memory_boundaries(void)
{
    printf("\n=== Memory Boundary Tests ===\r\n");
    
    // Test allocation at boundaries
    void *small_alloc = kmalloc(1);
    printf("Small allocation (1 byte): %p\r\n", small_alloc);
    
    void *medium_alloc = kmalloc(100);
    printf("Medium allocation (100 bytes): %p\r\n", medium_alloc);
    
    void *large_alloc = kmalloc(500);
    printf("Large allocation (500 bytes): %p\r\n", large_alloc);
    
    // Test memory content
    char *test_mem = (char*)kmalloc(50);
    if (test_mem) {
        memset(test_mem, 0xAA, 50);
        
        // Verify content
        int correct = 1;
        for (int i = 0; i < 50; i++) {
            if (test_mem[i] != 0xAA) {
                correct = 0;
                break;
            }
        }
        printf("Memory content test: %s\r\n", correct ? "PASS" : "FAIL");
    }
}

void test_memory_exhaustion(void)
{
    printf("\n=== Memory Exhaustion Tests ===\r\n");
    
    uint32_t initial_free = mm_get_free();
    printf("Initial free memory: %lu bytes\r\n", initial_free);
    
    // Try to allocate more than available
    void *huge_alloc = kmalloc(initial_free + 1000);
    if (huge_alloc == NULL) {
        printf("✓ Correctly rejected oversized allocation\r\n");
    } else {
        printf("✗ Should have rejected oversized allocation\r\n");
    }
    
    // Test sequential allocations
    void *allocations[100];
    int success_count = 0;
    
    for (int i = 0; i < 100; i++) {
        allocations[i] = kmalloc(100);
        if (allocations[i] != NULL) {
            success_count++;
        } else {
            break;
        }
    }
    
    printf("Successful sequential allocations: %d\r\n", success_count);
    printf("Remaining free memory: %lu bytes\r\n", mm_get_free());
}

void test_memory_patterns(void)
{
    printf("\n=== Memory Pattern Tests ===\r\n");
    
    // Test different patterns
    uint32_t *pattern_alloc = (uint32_t*)kmalloc(256);
    if (pattern_alloc) {
        // Write pattern
        for (int i = 0; i < 64; i++) {
            pattern_alloc[i] = 0xDEADBEEF;
        }
        
        // Verify pattern
        int pattern_ok = 1;
        for (int i = 0; i < 64; i++) {
            if (pattern_alloc[i] != 0xDEADBEEF) {
                pattern_ok = 0;
                break;
            }
        }
        printf("32-bit pattern test: %s\r\n", pattern_ok ? "PASS" : "FAIL");
    }
}

void run_memory_tests(void)
{
    printf("Starting Memory Tests\r\n");
    printf("=====================\r\n");
    
    test_memory_boundaries();
    test_memory_exhaustion();
    test_memory_patterns();
    
    printf("\nMemory tests completed.\r\n");
}
