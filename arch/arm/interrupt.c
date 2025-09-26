#include <stdint.h>
#include "stm32f4xx.h"
#include "core_cm4.h"
#define NVIC_ISER0 (*(volatile uint32_t*)0xE000E100)
#define NVIC_IPR0  (*(volatile uint32_t*)0xE000E400)

void nvic_enable_irq(int irq_number) {
    NVIC_ISER0 |= (1 << irq_number);
}

void nvic_set_priority(int irq_number, uint8_t priority) {
    volatile uint8_t *pri_reg = (volatile uint8_t*)(NVIC_IPR0 + irq_number);
    *pri_reg = priority;
}

void systick_init(uint32_t reload_value) {
    SysTick->LOAD = reload_value;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk |
                   SysTick_CTRL_ENABLE_Msk;
}

