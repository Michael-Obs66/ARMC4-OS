.syntax unified
.cpu cortex-m4
.thumb

.text
.thumb_func
.global task_switch
.global task_switch_to
.global PendSV_Handler

.equ NVIC_INT_CTRL, 0xE000ED04
.equ NVIC_PENDSVSET, 0x10000000

task_switch:
    // Save current task context
    mrs r0, psp
    stmdb r0!, {r4-r11, lr}
    
    // Save SP to current task
    ldr r2, =current_task
    ldr r1, [r2]
    str r0, [r1]
    
    // Switch to next task
    ldr r1, [r2, #4] // next_task
    str r1, [r2]     // current_task = next_task
    
    // Restore next task context
    ldr r0, [r1]
    ldmia r0!, {r4-r11, lr}
    msr psp, r0
    
    bx lr

task_switch_to:
    // r0 = task pointer
    ldr r1, =current_task
    str r0, [r1]
    
    // Restore task context
    ldr r0, [r0]
    ldmia r0!, {r4-r11, lr}
    msr psp, r0
    
    // Set PSP as active stack
    mov r0, #2
    msr control, r0
    isb
    
    bx lr

PendSV_Handler:
    // Save context
    mrs r0, psp
    stmdb r0!, {r4-r11, lr}
    
    // Save current task SP
    ldr r2, =current_task
    ldr r1, [r2]
    str r0, [r1]
    
    // Call scheduler
    bl schedule
    
    // Load next task SP
    ldr r1, [r2]
    ldr r0, [r1]
    ldmia r0!, {r4-r11, lr}
    msr psp, r0
    
    bx lr
