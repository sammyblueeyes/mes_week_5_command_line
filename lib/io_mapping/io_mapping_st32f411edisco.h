#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stm32f4xx_hal.h>
#include <string.h>

#define GREEN_LED_PIN GPIO_PIN_12
#define ORANGE_LED_PIN GPIO_PIN_13
#define RED_LED_PIN GPIO_PIN_14
#define BLUE_LED_PIN GPIO_PIN_15

#define LED_GPIO_PORT GPIOD
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define BUTTON_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define BUTTON_PIN GPIO_PIN_0
#define BUTTON_GPIO_PORT GPIOA
#define BUTTON_IRQn EXTI0_IRQn

void io_logging_init();
void io_logging_write(const char* msg, size_t len);

#ifdef __cplusplus
} // extern "C"
#endif