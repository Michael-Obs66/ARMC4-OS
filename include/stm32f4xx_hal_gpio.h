#ifndef STM32F4XX_HAL_GPIO_H
#define STM32F4XX_HAL_GPIO_H

#include "stm32f4xx.h"

/* =========================================================
 *  Mini HAL GPIO for ARMC4-OS
 * ========================================================= */
typedef struct
{
  uint32_t Pin;        /*!< Specifies the GPIO pins to be configured. */
  uint32_t Mode;       /*!< Specifies the operating mode for the selected pins. */
  uint32_t Pull;       /*!< Specifies the Pull-up or Pull-Down activation. */
  uint32_t Speed;      /*!< Specifies the speed for the selected pins. */
  uint32_t Alternate;  /*!< Peripheral to be connected to the selected pins. */
} GPIO_InitTypeDef;

/* =========================================================
 *  GPIO Mode definition
 * ========================================================= */
#define GPIO_MODE_INPUT              0x00000000U
#define GPIO_MODE_OUTPUT_PP          0x00000001U
#define GPIO_MODE_OUTPUT_OD          0x00000011U
#define GPIO_MODE_AF_PP              0x00000002U
#define GPIO_MODE_AF_OD              0x00000012U
#define GPIO_MODE_ANALOG             0x00000003U

/* =========================================================
 *  GPIO Pull-Up Pull-Down
 * ========================================================= */
#define GPIO_NOPULL                  0x00000000U
#define GPIO_PULLUP                  0x00000001U
#define GPIO_PULLDOWN                0x00000002U

/* =========================================================
 *  GPIO Speed
 * ========================================================= */
#define GPIO_SPEED_FREQ_LOW          0x00000000U
#define GPIO_SPEED_FREQ_MEDIUM       0x00000001U
#define GPIO_SPEED_FREQ_HIGH         0x00000002U
#define GPIO_SPEED_FREQ_VERY_HIGH    0x00000003U

/* =========================================================
 *  Mini HAL_GPIO_Init()
 * ========================================================= */
static inline void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    // Configure GPIO mode (basic version)
    if (GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP)
    {
        GPIOx->MODER &= ~(3U << (GPIO_Init->Pin * 2));
        GPIOx->MODER |=  (1U << (GPIO_Init->Pin * 2));
    }
}

#endif /* STM32F4XX_HAL_GPIO_H */
