#include <uart.h>

UART::UART(USART_TypeDef *uart, uint32_t uart_tx_pin,
           GPIO_TypeDef *uart_tx_gpio_port, uint32_t uart_rx_pin,
           GPIO_TypeDef *uart_rx_gpio_port)
    : m_uart(uart), m_uart_tx_pin(uart_tx_pin),
      m_uart_tx_gpio_port(uart_tx_gpio_port), m_uart_rx_pin(uart_rx_pin),
      m_uart_rx_gpio_port(uart_rx_gpio_port) {
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = m_uart_tx_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(m_uart_tx_gpio_port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = m_uart_rx_pin;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(m_uart_rx_gpio_port, &GPIO_InitStruct);

  m_uart_handle.Instance = m_uart;
  m_uart_handle.Init.BaudRate = 115200;
  m_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
  m_uart_handle.Init.StopBits = UART_STOPBITS_1;
  m_uart_handle.Init.Parity = UART_PARITY_NONE;
  m_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  m_uart_handle.Init.Mode = UART_MODE_TX_RX;
  m_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&m_uart_handle);
}

void UART::Write(const char *msg, size_t len) {
  HAL_UART_Transmit(&m_uart_handle, (uint8_t *)msg, len, HAL_MAX_DELAY);
}

bool UART::ReadChar(uint8_t *buffer) {
  uint8_t read[2] = {0, 0};
  if (HAL_UART_Receive(&m_uart_handle, read, 1, 1) == HAL_OK) {
    *buffer = read[0];
    return true;
  }
  return false;
}
