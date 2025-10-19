.syntax unified
.cpu cortex-m4
.thumb

.global _estack
.global Reset_Handler

/* =========================================================
 * Vector Table
 * ========================================================= */
.section .isr_vector, "a", %progbits
.type g_pfnVectors, %object
.size g_pfnVectors, .-g_pfnVectors
g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word SVC_Handler
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

/* =========================================================
 * Reset Handler
 * ========================================================= */
.section .text.Reset_Handler
Reset_Handler:
    ldr r0, =_estack
    mov sp, r0

    bl SystemInit
    bl main

LoopForever:
    b LoopForever
