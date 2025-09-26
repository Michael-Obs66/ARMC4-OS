#include <stdint.h>

extern void SystemInit(void);
extern void kernel_init(void);

int main(void) {
    SystemInit();
    kernel_init();  // jalankan init kernel
    while(1);       // loop selamanya
    return 0;
}
