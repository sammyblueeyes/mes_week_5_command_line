#pragma once

#include <stm32f4xx_hal.h>

class Button {

public:
  enum eState {
    RELEASED,
    PRESSED,
  };

private:
  GPIO_TypeDef *m_port;
  uint32_t m_pin;
  IRQn_Type m_irq;
  eState m_state;
  bool m_int_received;

  eState ReadState() const;

public:
  Button(GPIO_TypeDef *port, uint32_t pin, IRQn_Type irq);

  eState GetState();

  void HandleInterrupt();
};
