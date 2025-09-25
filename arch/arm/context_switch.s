.syntax unified
.cpu cortex-m4
.thumb

.text
.thumb_func
.global PendSV_Handler
.global context_switch

PendSV_Handler:
    cpsid i
    mrs r0, psp
    
    // Save current task context
    stmdb r0!, {r4-r11, lr}
    
    ldr r1, =current_task
    ldr r2, [r1]
    str r0, [r2]
    
    // Load next task context
    ldr r3, =current_task
    ldr r0, [r3]
    ldr r0, [r0]
    
    ldmia r0!, {r4-r11, lr}
    msr psp, r0
    
    cpsie i
    bx lr

context_switch:
    svc 0
    bx lr
