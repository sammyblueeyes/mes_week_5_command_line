#include <led.h>
#include <button.h>
#include <dbg_logging.h>

#include <io_mapping.h>
#include <memory>


// stm32cube overrides are C functions
extern "C" {

  void SysTick_Handler(void)
  {
    HAL_IncTick();
  }

  void EXTI0_IRQHandler(void)   
  {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  }

  void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

} // extern "C"


enum eLEDs {
  LED_BLUE,
  LED_GREEN,
  LED_ORANGE,
  LED_RED,
  LED_MAX,
};

enum eInterrupts {
  IT_BUTTON = GPIO_PIN_0,
  IT_MAX,
};

struct ProgramState {
    std::unique_ptr<LED> leds[LED_MAX];
    std::unique_ptr<Button> button;
};

const uint32_t BLINK_INTERVAL_MS = 1000;
const uint32_t LOOP_INTERVAL_MS = 5; 

ProgramState g_state;

int main(void)
{
  HAL_Init();
  LoggingInit();

  // NOTE (from HAL): After reset, the peripheral clock (used for registers read/write access)
  // is disabled and the application software has to enable this clock before using it.
  LED_GPIO_CLK_ENABLE();
  BUTTON_GPIO_CLK_ENABLE();

  g_state.leds[LED_BLUE] = std::make_unique<LED>(LED_GPIO_PORT, BLUE_LED_PIN);
  g_state.leds[LED_GREEN] = std::make_unique<LED>(LED_GPIO_PORT, GREEN_LED_PIN);
  g_state.leds[LED_ORANGE] = std::make_unique<LED>(LED_GPIO_PORT, ORANGE_LED_PIN);
  g_state.leds[LED_RED] = std::make_unique<LED>(LED_GPIO_PORT, RED_LED_PIN);
  g_state.button = std::make_unique<Button>(BUTTON_GPIO_PORT, BUTTON_PIN, BUTTON_IRQn);

  int cur_led = LED_BLUE;
  uint32_t blink_elapsed = 0;


  while (1)
  {
    if (g_state.button->GetState() == Button::PRESSED) {
      g_state.leds[cur_led]->TurnOff();
      blink_elapsed = BLINK_INTERVAL_MS;
      g_state.leds[LED_RED]->TurnOn();
      Log(LOG_SUBSYSTEM_MAIN, LOG_LEVEL_DEBUG, "Button Pressed\r\n");
    } else {
      g_state.leds[LED_RED]->TurnOff();
      blink_elapsed += LOOP_INTERVAL_MS;
      if (blink_elapsed >= BLINK_INTERVAL_MS) {
        blink_elapsed = 0;
        g_state.leds[cur_led]->Blink();
        Log(LOG_SUBSYSTEM_MAIN, LOG_LEVEL_DEBUG, "Toggling\r\n");
      }
    
    }
    HAL_Delay(LOOP_INTERVAL_MS);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
      case IT_BUTTON:
      {
        g_state.button->HandleInterrupt();
        break;
      }
      case IT_MAX:
        break;
  }
}
