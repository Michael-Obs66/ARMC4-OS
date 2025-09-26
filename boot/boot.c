#include <stdint.h>
#include "stm32f4xx.h"      // pastikan ada core_cm4.h include di sini
#include "kernel/kernel.h"  // header kernel_init() dan create_task()
#include "mm/mm.h"          // header kmalloc, heap, dll

// ==============================
// Stub global variables
// ==============================
uint32_t SystemCoreClock = 16000000; // contoh 16 MHz, sesuaikan MCU

// ==============================
// Stub fungsi libc minimal
// ==============================
void *memcpy(void *dest, const void *src, size_t n) {
    char *d = (char*)dest;
    const char *s = (const char*)src;
    for(size_t i=0; i<n; i++)
        d[i] = s[i];
    return dest;
}

int strcmp(const char *a, const char *b) {
    while(*a && (*a == *b)) { a++; b++; }
    return *(unsigned char*)a - *(unsigned char*)b;
}

// ==============================
// Stub context switch
// ==============================
void context_switch(void) {
    // nanti diisi implementasi context switch
}

// ==============================
// Sistem init & delay (contoh)
// ==============================
void SystemInit(void) {
    // inisialisasi hardware clock jika perlu
}

#define SYSTICK_LOAD_VAL 8000
#define SYSTICK_CTRL_ENABLE (1 << 0)
#define SYSTICK_CTRL_CLKSRC (1 << 2)
#define SYSTICK_CTRL_COUNTFLAG (1 << 16)

void systick_delay_ms(uint32_t delay) {
    volatile uint32_t *STK_LOAD = (uint32_t*)0xE000E014;
    volatile uint32_t *STK_VAL  = (uint32_t*)0xE000E018;
    volatile uint32_t *STK_CTRL = (uint32_t*)0xE000E010;

    *STK_LOAD = SYSTICK_LOAD_VAL - 1;
    *STK_VAL  = 0;
    *STK_CTRL = SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_CLKSRC;

    for(uint32_t i=0; i<delay; i++) {
        while(!(*STK_CTRL & SYSTICK_CTRL_COUNTFLAG));
    }
    *STK_CTRL = 0;
}

// ==============================
// Stub main system
// ==============================
int main(void) {
    SystemInit();    // init system
    cortex_m4_init();// init MCU core
    kernel_init();   // init kernel
    // bisa tambahkan create_task(blinky_task, ...) dsb.

    while(1) {
        // loop utama kernel
    }

    return 0; // meskipun di bare-metal, return tidak dipakai
}
