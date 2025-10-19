#include <stdint.h>
#include "../include/kernel.h"

/* STM32F4 Peripheral Base */
#define RCC_BASE        0x40023800UL
#define FLASH_BASE      0x40023C00UL
#define SCB_BASE        0xE000ED00UL

#define RCC_AHB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x44))
#define FLASH_ACR       (*(volatile uint32_t *)(FLASH_BASE + 0x00))
#define SCB_VTOR        (*(volatile uint32_t *)(SCB_BASE + 0x08))
#define SCB_CCR         (*(volatile uint32_t *)(SCB_BASE + 0x14))

void SystemInit(void)
{
    SCB_VTOR = 0x08000000U;                     // Vector table base
    FLASH_ACR = (1 << 8) | (1 << 9) | (2 << 0); // Prefetch & 2 WS latency
    RCC_AHB1ENR |= (1 << 0) | (1 << 1) | (1 << 2); // GPIOA,B,C
    RCC_APB1ENR |= (1 << 17);                   // USART2
    RCC_APB2ENR |= (1 << 12);                   // SPI1
    SCB_CCR |= (1 << 4);                        // Sleep divide enable
}

void Boot_Init(void)
{
    SystemInit();
    kernel_main();
}
