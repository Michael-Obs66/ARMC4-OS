#include <stdint.h>
#include <string.h>

// System clock dummy
uint32_t SystemCoreClock = 16000000UL;  // 16 MHz

// CMSIS stub
void NVIC_SetPriorityGrouping(uint32_t group) {}

// Minimal libc
void *memcpy(void *dst, const void *src, unsigned int n) {
    unsigned char *d = dst;
    const unsigned char *s = src;
    for(unsigned int i = 0; i < n; i++) d[i] = s[i];
    return dst;
}

int strcmp(const char *s1, const char *s2) {
    while(*s1 && (*s1==*s2)) { s1++; s2++; }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

// Scheduler stub
void context_switch(void) {}

// Kernel stub
void kernel_init(void) {}
