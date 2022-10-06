#include <button.h>

Button::eState Button::ReadState() const {
  // Check state
  GPIO_PinState state = HAL_GPIO_ReadPin(m_port, m_pin);
  return (state == GPIO_PIN_SET) ? PRESSED : RELEASED;
}

Button::Button(GPIO_TypeDef *port, uint32_t pin, IRQn_Type irq)
    : m_port(port), m_pin(pin), m_irq(irq), m_state(RELEASED) {
  // Setup the GPIO:
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = m_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(m_port, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(m_irq, 0, 0);
  HAL_NVIC_EnableIRQ(m_irq);
}

Button::eState Button::GetState() {
  if (m_int_received) {
    m_int_received = false;
    m_state = ReadState();
  }
  return m_state;
}

void Button::HandleInterrupt() { m_int_received = true; }