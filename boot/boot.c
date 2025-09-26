#include <stdint.h>

#define SYSTICK_LOAD_VAL 8000
#define SYSTICK_CTRL_ENABLE (1 << 0)
#define SYSTICK_CTRL_CLKSRC (1 << 2)
#define SYSTICK_CTRL_COUNTFLAG (1 << 16)

void SystemInit(void) {
    // Configure clock if needed
}

void systick_delay_ms(uint32_t delay) {
    uint32_t *STK_LOAD = (uint32_t*)0xE000E014;
    uint32_t *STK_VAL  = (uint32_t*)0xE000E018;
    uint32_t *STK_CTRL = (uint32_t*)0xE000E010;
    
    *STK_LOAD = SYSTICK_LOAD_VAL - 1;
    *STK_VAL = 0;
    *STK_CTRL = SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_CLKSRC;
    
    for(uint32_t i = 0; i < delay; i++) {
        while(!(*STK_CTRL & SYSTICK_CTRL_COUNTFLAG));
    }
    *STK_CTRL = 0;
}
