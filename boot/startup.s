.syntax unified
.cpu cortex-m4
.thumb

.section .isr_vector
.align 2
.global _isr_vector

_isr_vector:
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

.section .text
.thumb_func
.global Reset_Handler

Reset_Handler:
    ldr r0, =_estack
    mov sp, r0
    
    ldr r0, =_data_start
    ldr r1, =_data_end
    ldr r2, =_data_load_start
    cmp r0, r1
    beq main_init
    
copy_data:
    ldr r3, [r2], #4
    str r3, [r0], #4
    cmp r0, r1
    blt copy_data

main_init:
    ldr r0, =_bss_start
    ldr r1, =_bss_end
    mov r2, #0
    
zero_bss:
    str r2, [r0], #4
    cmp r0, r1
    blt zero_bss
    
    bl SystemInit
    bl main
    b .

.thumb_func
.global Default_Handler
Default_Handler:
    b .

.weak NMI_Handler
.thumb_set NMI_Handler,Default_Handler
.weak HardFault_Handler
.thumb_set HardFault_Handler,Default_Handler
.weak MemManage_Handler
.thumb_set MemManage_Handler,Default_Handler
.weak BusFault_Handler
.thumb_set BusFault_Handler,Default_Handler
.weak UsageFault_Handler
.thumb_set UsageFault_Handler,Default_Handler
.weak SVC_Handler
.thumb_set SVC_Handler,Default_Handler
.weak DebugMon_Handler
.thumb_set DebugMon_Handler,Default_Handler
.weak PendSV_Handler
.thumb_set PendSV_Handler,Default_Handler
.weak SysTick_Handler
.thumb_set SysTick_Handler,Default_Handler
