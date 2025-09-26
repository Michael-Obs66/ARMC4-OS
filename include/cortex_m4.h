#ifndef CORTEX_M4_H
#define CORTEX_M4_H

#include <stdint.h>

/* NVIC (Nested Vectored Interrupt Controller) functions */
void cortex_m4_enable_irq(int irq);
void cortex_m4_disable_irq(int irq);
void cortex_m4_set_priority(int irq, uint8_t priority);

/* SysTick timer functions */
void cortex_m4_systick_config(uint32_t ticks);

/* Core register functions */
uint32_t cortex_m4_get_ipsr(void);

/* Low power / event functions */
void cortex_m4_wfi(void);
void cortex_m4_wfe(void);
void cortex_m4_sev(void);

#endif /* CORTEX_M4_H */
