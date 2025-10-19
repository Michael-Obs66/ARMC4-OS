#ifndef STM32F4XX_HAL_GPIO_H
#define STM32F4XX_HAL_GPIO_H

#include "stm32f4xx.h"

/* =========================================================
 *  Mini HAL GPIO for ARMC4-OS
 *  Lightweight replacement for STM32Cube HAL GPIO
 * ========================================================= */

typedef struct
{
  uint32_t Pin;        /*!< Specifies the GPIO pin number (0-15). */
  uint32_t Mode;       /*!< Specifies the operating mode. */
  uint32_t Pull;       /*!< Specifies Pull-up or Pull-down activation. */
  uint32_t Speed;      /*!< Specifies output speed. */
  uint32_t Alternate;  /*!< Alternate function selection (AF0–AF15). */
} GPIO_InitTypeDef;

/* =========================================================
 *  GPIO Mode definitions
 * ========================================================= */
#define GPIO_MODE_INPUT              0x00000000U
#define GPIO_MODE_OUTPUT_PP          0x00000001U
#define GPIO_MODE_OUTPUT_OD          0x00000011U
#define GPIO_MODE_AF_PP              0x00000002U
#define GPIO_MODE_AF_OD              0x00000012U
#define GPIO_MODE_ANALOG             0x00000003U

/* =========================================================
 *  GPIO Pull definitions
 * ========================================================= */
#define GPIO_NOPULL                  0x00000000U
#define GPIO_PULLUP                  0x00000001U
#define GPIO_PULLDOWN                0x00000002U

/* =========================================================
 *  GPIO Speed definitions
 * ========================================================= */
#define GPIO_SPEED_FREQ_LOW          0x00000000U
#define GPIO_SPEED_FREQ_MEDIUM       0x00000001U
#define GPIO_SPEED_FREQ_HIGH         0x00000002U
#define GPIO_SPEED_FREQ_VERY_HIGH    0x00000003U

/* =========================================================
 *  Alternate Function mappings
 * ========================================================= */
#define GPIO_AF0_RTC_50Hz        ((uint8_t)0x00)
#define GPIO_AF1_TIM1            ((uint8_t)0x01)
#define GPIO_AF2_TIM3            ((uint8_t)0x02)
#define GPIO_AF3_TIM9            ((uint8_t)0x03)
#define GPIO_AF4_I2C1            ((uint8_t)0x04)
#define GPIO_AF5_SPI1            ((uint8_t)0x05)
#define GPIO_AF6_SPI3            ((uint8_t)0x06)
#define GPIO_AF7_USART2          ((uint8_t)0x07)
#define GPIO_AF8_USART6          ((uint8_t)0x08)
#define GPIO_AF9_CAN1            ((uint8_t)0x09)
#define GPIO_AF10_OTG_FS         ((uint8_t)0x0A)
#define GPIO_AF11_ETH            ((uint8_t)0x0B)
#define GPIO_AF12_FSMC           ((uint8_t)0x0C)
#define GPIO_AF13_DCMI           ((uint8_t)0x0D)
#define GPIO_AF14_LTDC           ((uint8_t)0x0E)
#define GPIO_AF15_EVENTOUT       ((uint8_t)0x0F)

/* =========================================================
 *  Helper Macros
 * ========================================================= */
#define __HAL_RCC_GPIOA_CLK_ENABLE()   (RCC->AHB1ENR |= (1U << 0))
#define __HAL_RCC_GPIOB_CLK_ENABLE()   (RCC->AHB1ENR |= (1U << 1))
#define __HAL_RCC_GPIOC_CLK_ENABLE()   (RCC->AHB1ENR |= (1U << 2))
#define __HAL_RCC_GPIOD_CLK_ENABLE()   (RCC->AHB1ENR |= (1U << 3))
#define __HAL_RCC_GPIOE_CLK_ENABLE()   (RCC->AHB1ENR |= (1U << 4))

/* =========================================================
 *  Mini HAL_GPIO_Init()
 * ========================================================= */
static inline void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t pos = GPIO_Init->Pin;
    uint32_t mode = GPIO_Init->Mode;
    uint32_t pull = GPIO_Init->Pull;
    uint32_t speed = GPIO_Init->Speed;
    uint32_t alternate = GPIO_Init->Alternate;

    // 1️⃣ Configure mode (MODER)
    GPIOx->MODER &= ~(3U << (pos * 2));
    if (mode == GPIO_MODE_INPUT)
        GPIOx->MODER |= (0U << (pos * 2));
    else if (mode == GPIO_MODE_OUTPUT_PP || mode == GPIO_MODE_OUTPUT_OD)
        GPIOx->MODER |= (1U << (pos * 2));
    else if (mode == GPIO_MODE_AF_PP || mode == GPIO_MODE_AF_OD)
        GPIOx->MODER |= (2U << (pos * 2));
    else if (mode == GPIO_MODE_ANALOG)
        GPIOx->MODER |= (3U << (pos * 2));

    // 2️⃣ Configure output type (OTYPER)
    if (mode == GPIO_MODE_OUTPUT_OD || mode == GPIO_MODE_AF_OD)
        GPIOx->OTYPER |= (1U << pos);
    else
        GPIOx->OTYPER &= ~(1U << pos);

    // 3️⃣ Configure speed (OSPEEDR)
    GPIOx->OSPEEDR &= ~(3U << (pos * 2));
    GPIOx->OSPEEDR |= (speed << (pos * 2));

    // 4️⃣ Configure pull-up/pull-down (PUPDR)
    GPIOx->PUPDR &= ~(3U << (pos * 2));
    GPIOx->PUPDR |= (pull << (pos * 2));

    // 5️⃣ Configure alternate function (AFR)
    if (mode == GPIO_MODE_AF_PP || mode == GPIO_MODE_AF_OD)
    {
        uint8_t afr_index = pos / 8;
        uint8_t afr_shift = (pos % 8) * 4;
        GPIOx->AFR[afr_index] &= ~(0xFU << afr_shift);
        GPIOx->AFR[afr_index] |= ((alternate & 0xFU) << afr_shift);
    }
}

/* =========================================================
 *  Mini HAL_GPIO_WritePin() & HAL_GPIO_TogglePin()
 * ========================================================= */
static inline void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t State)
{
    if (State)
        GPIOx->BSRR = (1U << GPIO_Pin);
    else
        GPIOx->BSRR = (1U << (GPIO_Pin + 16));
}

static inline void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= (1U << GPIO_Pin);
}

/* =========================================================
 *  Mini HAL_GPIO_ReadPin()
 * ========================================================= */
static inline uint8_t HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    return (uint8_t)((GPIOx->IDR & (1U << GPIO_Pin)) ? 1 : 0);
}

#endif /* STM32F4XX_HAL_GPIO_H */
