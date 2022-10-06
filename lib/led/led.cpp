#include <led.h>

LED::LED(GPIO_TypeDef *port, uint32_t pin) : m_port(port), m_pin(pin) {
  // Setup the GPIO:
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = m_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(m_port, &GPIO_InitStruct);
}

void LED::TurnOn() const { HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET); }

void LED::TurnOff() const { HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET); }

void LED::Blink() const { HAL_GPIO_TogglePin(m_port, m_pin); }
