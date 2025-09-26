#include "../lib/cortex_m4.h"

#define NVIC_BASE 0xE000E100
#define SCB_BASE  0xE000ED00

void cortex_m4_enable_irq(int irq)
{
    volatile uint32_t *nvic_iser = (volatile uint32_t*)(NVIC_BASE + 0x00);
    nvic_iser[irq >> 5] = 1 << (irq & 0x1F);
}

void cortex_m4_disable_irq(int irq)
{
    volatile uint32_t *nvic_icer = (volatile uint32_t*)(NVIC_BASE + 0x80);
    nvic_icer[irq >> 5] = 1 << (irq & 0x1F);
}

void cortex_m4_set_priority(int irq, uint8_t priority)
{
    volatile uint8_t *nvic_ip = (volatile uint8_t*)(NVIC_BASE + 0x300 + irq);
    *nvic_ip = priority << 4;
}

void cortex_m4_systick_config(uint32_t ticks)
{
    volatile uint32_t *syst_csr = (volatile uint32_t*)0xE000E010;
    volatile uint32_t *syst_rvr = (volatile uint32_t*)0xE000E014;
    volatile uint32_t *syst_cvr = (volatile uint32_t*)0xE000E018;
    
    *syst_rvr = ticks - 1;
    *syst_cvr = 0;
    *syst_csr = 0x00000007;
}

uint32_t cortex_m4_get_ipsr(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, ipsr" : "=r" (result));
    return result;
}

void cortex_m4_wfi(void)
{
    __asm volatile ("wfi");
}

void cortex_m4_wfe(void)
{
    __asm volatile ("wfe");
}

void cortex_m4_sev(void)
{
    __asm volatile ("sev");
}

