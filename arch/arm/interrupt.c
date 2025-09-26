#include "../lib/interrupt.h"

static interrupt_handler_t interrupt_handlers[256];

void interrupt_init(void)
{
    for (int i = 0; i < 256; i++) {
        interrupt_handlers[i] = NULL;
    }
}

void interrupt_register(int irq, interrupt_handler_t handler)
{
    if (irq >= 0 && irq < 256) {
        interrupt_handlers[irq] = handler;
    }
}

void interrupt_enable(int irq)
{
    cortex_m4_enable_irq(irq);
}

void interrupt_disable(int irq)
{
    cortex_m4_disable_irq(irq);
}

// Default interrupt handler
void Default_Handler(void)
{
    while (1);
}

// IRQ Handlers
void USART1_IRQHandler(void) 
{
    if (interrupt_handlers[USART1_IRQn]) {
        interrupt_handlers[USART1_IRQn]();
    }
}

void TIM1_IRQHandler(void)
{
    if (interrupt_handlers[TIM1_IRQn]) {
        interrupt_handlers[TIM1_IRQn]();
    }
}

void EXTI0_IRQHandler(void)
{
    if (interrupt_handlers[EXTI0_IRQn]) {
        interrupt_handlers[EXTI0_IRQn]();
    }
}

