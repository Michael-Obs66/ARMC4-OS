#include <stdint.h>

#define SCB_CPACR (*(volatile uint32_t*)0xE000ED88)
#define SCB_ICSR  (*(volatile uint32_t*)0xE000ED04)

#define SCB_ICSR_PENDSVSET_Msk (1 << 28)

void cortex_m4_init(void) {
    // Enable FPU
    SCB_CPACR |= (0xF << 20);
    
    // Set priority grouping
    NVIC_SetPriorityGrouping(3);
}

uint32_t get_system_clock(void) {
    return SystemCoreClock;
}

void enable_interrupts(void) {
    __asm volatile ("cpsie i");
}

void disable_interrupts(void) {
    __asm volatile ("cpsid i");
}
