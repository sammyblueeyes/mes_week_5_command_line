#pragma once

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_uart.h>
#include <string.h>

class UART {
private:
  UART_HandleTypeDef m_uart_handle;
  USART_TypeDef *m_uart;
  uint32_t m_uart_tx_pin;
  GPIO_TypeDef *m_uart_tx_gpio_port;
  uint32_t m_uart_rx_pin;
  GPIO_TypeDef *m_uart_rx_gpio_port;

public:
  UART(USART_TypeDef *uart, uint32_t uart_tx_pin,
       GPIO_TypeDef *uart_tx_gpio_port, uint32_t uart_rx_pin,
       GPIO_TypeDef *uart_rx_gpio_port);

  void Write(const char *msg, size_t len);
};