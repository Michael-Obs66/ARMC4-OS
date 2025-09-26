#include "..lib/drivers.h"

#define TIM1_BASE 0x40012C00
#define TIM1_CR1  (*((volatile uint32_t *)(TIM1_BASE + 0x00)))
#define TIM1_PSC  (*((volatile uint32_t *)(TIM1_BASE + 0x28)))
#define TIM1_ARR  (*((volatile uint32_t *)(TIM1_BASE + 0x2C)))
#define TIM1_DIER (*((volatile uint32_t *)(TIM1_BASE + 0x0C)))
#define TIM1_SR   (*((volatile uint32_t *)(TIM1_BASE + 0x10)))

void timer_init(void)
{
    // Configure TIM1 for 1ms ticks
    TIM1_PSC = 7200 - 1; // 72MHz / 7200 = 10kHz
    TIM1_ARR = 10 - 1;   // 10kHz / 10 = 1kHz (1ms)
    
    TIM1_DIER = 1; // Enable update interrupt
    TIM1_CR1 = 1;  // Enable timer
}

void timer_delay_ms(uint32_t ms)
{
    // Simple busy wait delay
    for (volatile uint32_t i = 0; i < ms * 1000; i++);
}

uint32_t timer_get_ticks(void)
{
    return kernel_get_ticks();
}

