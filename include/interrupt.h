#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>
#include "cortex_m4.h"

/* Define IRQ numbers (contoh, sesuaikan dengan MCU yang dipakai) */
#define USART1_IRQn   37
#define TIM1_IRQn     25
#define EXTI0_IRQn    6
/* Tambahkan IRQ lain sesuai kebutuhan */

/* Type definition untuk handler interrupt */
typedef void (*interrupt_handler_t)(void);

/* Inisialisasi sistem interrupt */
void interrupt_init(void);

/* Registrasi handler untuk IRQ tertentu */
void interrupt_register(int irq, interrupt_handler_t handler);

/* Enable / disable interrupt */
void interrupt_enable(int irq);
void interrupt_disable(int irq);

/* Default handler untuk IRQ yang tidak di-handle */
void Default_Handler(void);

/* IRQ Handler prototypes */
void USART1_IRQHandler(void);
void TIM1_IRQHandler(void);
void EXTI0_IRQHandler(void);

#endif /* INTERRUPT_H */
