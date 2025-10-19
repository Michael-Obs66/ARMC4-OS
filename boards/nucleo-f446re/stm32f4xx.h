#ifndef __STM32F4XX_H
#define __STM32F4XX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* =========================================================
 *  CPU & Memory Base
 * ========================================================= */
#define PERIPH_BASE           ((uint32_t)0x40000000)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)
#define APB1PERIPH_BASE       (PERIPH_BASE + 0x00000000)
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000)

/* =========================================================
 *  RCC (Reset and Clock Control)
 * ========================================================= */
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)
typedef struct
{
  volatile uint32_t CR;
  volatile uint32_t PLLCFGR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t AHB1RSTR;
  volatile uint32_t AHB2RSTR;
  volatile uint32_t AHB3RSTR;
  uint32_t RESERVED0;
  volatile uint32_t APB1RSTR;
  volatile uint32_t APB2RSTR;
  uint32_t RESERVED1[2];
  volatile uint32_t AHB1ENR;
  volatile uint32_t AHB2ENR;
  volatile uint32_t AHB3ENR;
  uint32_t RESERVED2;
  volatile uint32_t APB1ENR;
  volatile uint32_t APB2ENR;
} RCC_TypeDef;

#define RCC                 ((RCC_TypeDef *) RCC_BASE)

/* =========================================================
 *  GPIO
 * ========================================================= */
typedef struct
{
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00)

#define GPIOA                 ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC                 ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD                 ((GPIO_TypeDef *) GPIOD_BASE)

/* Pin macros */
#define GPIO_PIN_0            ((uint16_t)0x0001)
#define GPIO_PIN_1            ((uint16_t)0x0002)
#define GPIO_PIN_2            ((uint16_t)0x0004)
#define GPIO_PIN_3            ((uint16_t)0x0008)
#define GPIO_PIN_4            ((uint16_t)0x0010)
#define GPIO_PIN_5            ((uint16_t)0x0020)
#define GPIO_PIN_6            ((uint16_t)0x0040)
#define GPIO_PIN_7            ((uint16_t)0x0080)
#define GPIO_PIN_8            ((uint16_t)0x0100)
#define GPIO_PIN_9            ((uint16_t)0x0200)
#define GPIO_PIN_10           ((uint16_t)0x0400)
#define GPIO_PIN_11           ((uint16_t)0x0800)
#define GPIO_PIN_12           ((uint16_t)0x1000)
#define GPIO_PIN_13           ((uint16_t)0x2000)
#define GPIO_PIN_14           ((uint16_t)0x4000)
#define GPIO_PIN_15           ((uint16_t)0x8000)

/* =========================================================
 *  USART
 * ========================================================= */
typedef struct
{
  volatile uint32_t SR;
  volatile uint32_t DR;
  volatile uint32_t BRR;
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t CR3;
  volatile uint32_t GTPR;
} USART_TypeDef;

#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
#define USART2                ((USART_TypeDef *) USART2_BASE)

/* =========================================================
 *  I2C
 * ========================================================= */
typedef struct
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t OAR1;
  volatile uint32_t OAR2;
  volatile uint32_t DR;
  volatile uint32_t SR1;
  volatile uint32_t SR2;
  volatile uint32_t CCR;
  volatile uint32_t TRISE;
} I2C_TypeDef;

#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
#define I2C1                  ((I2C_TypeDef *) I2C1_BASE)

/* =========================================================
 *  SysTick and NVIC (Cortex-M4 Core)
 * ========================================================= */
#define SCS_BASE              ((uint32_t)0xE000E000)
#define SysTick_BASE          (SCS_BASE + 0x0010)
#define NVIC_BASE             (SCS_BASE + 0x0100)

typedef struct
{
  volatile uint32_t CTRL;
  volatile uint32_t LOAD;
  volatile uint32_t VAL;
  volatile uint32_t CALIB;
} SysTick_Type;

#define SysTick               ((SysTick_Type *) SysTick_BASE)

/* =========================================================
 *  Clock Enable Macros
 * ========================================================= */
#define __HAL_RCC_GPIOA_CLK_ENABLE()   (RCC->AHB1ENR |= (1U << 0))
#define __HAL_RCC_GPIOB_CLK_ENABLE()   (RCC->AHB1ENR |= (1U << 1))
#define __HAL_RCC_USART2_CLK_ENABLE()  (RCC->APB1ENR |= (1U << 17))
#define __HAL_RCC_I2C1_CLK_ENABLE()    (RCC->APB1ENR |= (1U << 21))

/* =========================================================
 *  Simple delay
 * ========================================================= */
static inline void delay(volatile uint32_t n)
{
    while (n--) __asm__("nop");
}

/* =========================================================
 *  Misc constants
 * ========================================================= */
#define HSE_VALUE     ((uint32_t)8000000)
#define HSI_VALUE     ((uint32_t)16000000)

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4XX_H */
