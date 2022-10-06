#pragma once

#include <stm32f4xx_hal.h>

class LED {
private:
  GPIO_TypeDef *m_port;
  uint32_t m_pin;

public:
  LED(GPIO_TypeDef *port, uint32_t pin);

  void TurnOn() const;

  void TurnOff() const;

  void Blink() const;
};
