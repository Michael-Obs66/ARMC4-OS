#include <stdint.h>

#define TIMER_BASE 0x40010000

typedef struct {
    volatile uint32_t LOAD;
    volatile uint32_t VALUE;
    volatile uint32_t CTRL;
    volatile uint32_t INTCLR;
    volatile uint32_t RIS;
    volatile uint32_t MIS;
} timer_t;

#define TIMER ((timer_t*)TIMER_BASE)

void timer_init(uint32_t interval_ms) {
    TIMER->LOAD = (SystemCoreClock / 1000) * interval_ms;
    TIMER->CTRL = (1 << 0) | // Enable timer
                  (1 << 1) | // Periodic mode
                  (1 << 2);  // 32-bit mode
}

void timer_delay_ms(uint32_t ms) {
    TIMER->LOAD = (SystemCoreClock / 1000) * ms;
    TIMER->VALUE = TIMER->LOAD;
    TIMER->CTRL |= (1 << 0);
    
    while (!(TIMER->RIS & 1));
    TIMER->INTCLR = 1;
}
