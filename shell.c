#include "kernel.h"
#include "drivers.h"

#define MAX_CMD_LEN 64

void shell_task(void *arg) {
    uart_init(115200);
    uart_puts("μKernelOS Shell v1.0\r\n");
    
    char buffer[MAX_CMD_LEN];
    int index = 0;
    
    while (1) {
        if (uart_getchar()) {
            char c = uart_getchar();
            
            if (c == '\r') {
                uart_puts("\r\n");
                buffer[index] = '\0';
                
                // Process command
                if (strcmp(buffer, "help") == 0) {
                    uart_puts("Available commands: help, tasks, mem\r\n");
                } else if (strcmp(buffer, "tasks") == 0) {
                    uart_puts("Running tasks: ");
                    // List tasks
                } else {
                    uart_puts("Unknown command\r\n");
                }
                
                index = 0;
            } else {
                uart_putchar(c);
                if (index < MAX_CMD_LEN - 1) {
                    buffer[index++] = c;
                }
            }
        }
        task_yield();
    }
}

void shell_init(void) {
    create_task(shell_task, NULL, 2);
}