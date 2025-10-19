#include <stdint.h>

/* 
 * Default Exception Handlers (Weak)
 * ---------------------------------
 * Setiap handler ditandai sebagai weak.
 * Jadi kalau ada definisi lain (misalnya dari kernel.c atau context_switch.s),
 * linker akan otomatis pakai yang override.
 */

__attribute__((weak)) void NMI_Handler(void)          { while (1); }
__attribute__((weak)) void HardFault_Handler(void)    { while (1); }
__attribute__((weak)) void MemManage_Handler(void)    { while (1); }
__attribute__((weak)) void BusFault_Handler(void)     { while (1); }
__attribute__((weak)) void UsageFault_Handler(void)   { while (1); }
__attribute__((weak)) void SVC_Handler(void)          { while (1); }
__attribute__((weak)) void DebugMon_Handler(void)     { while (1); }

/* 
 * PendSV dan SysTick biasanya diimplementasikan
 * oleh kernel multitasking, jadi jangan diisi while(1).
 * Biarkan kosong supaya bisa dioverride oleh kernel.c / context_switch.s
 */
__attribute__((weak)) void PendSV_Handler(void)       {}
__attribute__((weak)) void SysTick_Handler(void)      {}

