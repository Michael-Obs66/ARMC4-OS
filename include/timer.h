#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Inisialisasi timer
void timer_init(void);

// Delay dalam milidetik (busy wait)
void timer_delay_ms(uint32_t ms);

// Mendapatkan jumlah ticks saat ini dari kernel
uint32_t timer_get_ticks(void);

#endif // TIMER_H
