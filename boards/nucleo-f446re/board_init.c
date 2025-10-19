#include "board_config.h"
#include "../include/stm32f4xx_hal_gpio.h"

void Board_Init(void) {
    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure LED */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    /* Configure USART2 for debug */
    __HAL_RCC_USART2_CLK_ENABLE();
    GPIO_InitStruct.Pin = DEBUG_UART_TX_PIN | DEBUG_UART_RX_PIN;
    GPIO_InitStruct.Alternate = DEBUG_UART_AF;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(DEBUG_UART_GPIO_PORT, &GPIO_InitStruct);
}
