.syntax unified
.cpu cortex-m4
.thumb

/* Vector table */
.section .isr_vector, "a", %progbits
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

/* Reset handler */
.section .text
.thumb_func
.global Reset_Handler

Reset_Handler:
    /* Set up the stack pointer */
    ldr   r0, =_estack
    mov   sp, r0

    /* Copy initialized data from flash to RAM
       R0 = src (_data_load_start), R1 = dst (_data_start), R2 = end (_data_end) */
    ldr   r0, =_data_load_start
    ldr   r1, =_data_start
    ldr   r2, =_data_end
    cmp   r1, r2
    beq   clear_bss      /* nothing to copy */

copy_data_loop:
    ldr   r3, [r0], #4
    str   r3, [r1], #4
    cmp   r1, r2
    blt   copy_data_loop

/* Zero the .bss section
   R0 = dst (_bss_start), R1 = end (_bss_end), R2 = zero */
clear_bss:
    ldr   r0, =_bss_start
    ldr   r1, =_bss_end
    movs  r2, #0
    cmp   r0, r1
    beq   call_system

zero_bss_loop:
    str   r2, [r0], #4
    cmp   r0, r1
    blt   zero_bss_loop

call_system:
    bl    SystemInit
    bl    main
    b     .              /* Should never return */

/* Default handler (weak aliases below will point to this) */
.thumb_func
.global Default_Handler
Default_Handler:
    b .

/* weak aliases for exceptions/interrupts that default to Default_Handler */
.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler
.weak HardFault_Handler
.thumb_set HardFault_Handler, Default_Handler
.weak MemManage_Handler
.thumb_set MemManage_Handler, Default_Handler
.weak BusFault_Handler
.thumb_set BusFault_Handler, Default_Handler
.weak UsageFault_Handler
.thumb_set UsageFault_Handler, Default_Handler
.weak SVC_Handler
.thumb_set SVC_Handler, Default_Handler
.weak DebugMon_Handler
.thumb_set DebugMon_Handler, Default_Handler
.weak PendSV_Handler
.thumb_set PendSV_Handler, Default_Handler
.weak SysTick_Handler
.thumb_set SysTick_Handler, Default_Handler
