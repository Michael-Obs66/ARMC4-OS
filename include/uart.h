#ifndef UART_H
#define UART_H

#include <stdint.h>   // Untuk tipe data uint32_t, dll
#include "../include/drivers.h"  // Sesuaikan path jika berbeda

// Inisialisasi UART
void uart_init(void);

// Kirim satu karakter
void uart_putc(char c);

// Terima satu karakter
char uart_getc(void);

// Kirim string
void uart_puts(const char *str);

#endif // UART_H
