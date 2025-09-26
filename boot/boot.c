#include <stdint.h>

#define RCC_BASE        0x40021000
#define RCC_AHBENR      (*((volatile uint32_t *)(RCC_BASE + 0x14)))
#define RCC_APB2ENR     (*((volatile uint32_t *)(RCC_BASE + 0x18)))

#define FLASH_BASE      0x40022000
#define FLASH_ACR       (*((volatile uint32_t *)(FLASH_BASE + 0x00)))

#define SCB_BASE        0xE000ED00
#define SCB_VTOR        (*((volatile uint32_t *)(SCB_BASE + 0x08)))
#define SCB_CCR         (*((volatile uint32_t *)(SCB_BASE + 0x14)))

void SystemInit(void)
{
    // Set vector table offset
    SCB_VTOR = 0x08000000;
    
    // Enable prefetch buffer and set flash latency
    FLASH_ACR = (1 << 4) | (1 << 3) | (2 << 0);
    
    // Enable clock for GPIOA, GPIOB, GPIOC
    RCC_AHBENR |= (1 << 17) | (1 << 18) | (1 << 19);
    
    // Enable clock for USART1, SPI1, TIM1
    RCC_APB2ENR |= (1 << 12) | (1 << 14) | (1 << 11);
    
    // Enable divide by 8 for sleep mode
    SCB_CCR |= (1 << 4);
}

void __attribute__((naked)) main(void)
{
    // Initialize kernel
    extern void kernel_main(void);
    kernel_main();
    
    while(1);
}
