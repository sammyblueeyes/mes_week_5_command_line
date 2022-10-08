#include <button.h>
#include <io_mapping.h>
#include <led.h>
#include <logger.h>
#include <memory>
#include <uart.h>

// stm32cube overrides are C functions
extern "C" {

void SysTick_Handler(void) { HAL_IncTick(); }

void EXTI0_IRQHandler(void) { HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); }

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#include <console.h>
#include <consoleIo.h>
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
  std::unique_ptr<UART> debug_uart;
};

const uint32_t BLINK_INTERVAL_MS = 1000;
const uint32_t LOOP_INTERVAL_MS = 5;

ProgramState g_state;

void Init() {
  HAL_Init();

  // NOTE (from HAL): After reset, the peripheral clock (used for registers
  // read/write access) is disabled and the application software has to enable
  // this clock before using it.
  LED_GPIO_CLK_ENABLE();
  BUTTON_GPIO_CLK_ENABLE();
  DBG_UART_RX_GPIO_CLK_ENABLE();
  DBG_UART_TX_GPIO_CLK_ENABLE();
  DBG_UART_CLK_ENABLE();

  g_state.leds[LED_BLUE] = std::make_unique<LED>(LED_GPIO_PORT, BLUE_LED_PIN);
  g_state.leds[LED_GREEN] = std::make_unique<LED>(LED_GPIO_PORT, GREEN_LED_PIN);
  g_state.leds[LED_ORANGE] =
      std::make_unique<LED>(LED_GPIO_PORT, ORANGE_LED_PIN);
  g_state.leds[LED_RED] = std::make_unique<LED>(LED_GPIO_PORT, RED_LED_PIN);
  g_state.button =
      std::make_unique<Button>(BUTTON_GPIO_PORT, BUTTON_PIN, BUTTON_IRQn);
  g_state.debug_uart =
      std::make_unique<UART>(DBG_UART, DBG_UART_TX_PIN, DBG_UART_TX_GPIO_PORT,
                             DBG_UART_RX_PIN, DBG_UART_RX_GPIO_PORT);

  ConsoleInit();
}

class UARTLoggingBackend : public LoggingBackend {
public:
  virtual void Init() {}
  virtual void Write(const char *buffer, size_t len) {
    g_state.debug_uart->Write(buffer, len);
  }
};

int main(void) {
  Init();

  auto backend = std::make_unique<UARTLoggingBackend>();
  Logger logger("main", std::move(backend));

  int cur_led = LED_BLUE;
  uint32_t blink_elapsed = 0;

  while (1) {
    ConsoleProcess();
    if (g_state.button->GetState() == Button::PRESSED) {
      g_state.leds[cur_led]->TurnOff();
      blink_elapsed = BLINK_INTERVAL_MS;
      g_state.leds[LED_RED]->TurnOn();
    } else {
      g_state.leds[LED_RED]->TurnOff();
      blink_elapsed += LOOP_INTERVAL_MS;
      if (blink_elapsed >= BLINK_INTERVAL_MS) {
        blink_elapsed = 0;
        g_state.leds[cur_led]->Blink();
      }
    }
    HAL_Delay(LOOP_INTERVAL_MS);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
  case IT_BUTTON: {
    g_state.button->HandleInterrupt();
    break;
  }
  case IT_MAX:
    break;
  }
}

eConsoleError ConsoleIoInit(void) { return CONSOLE_SUCCESS; }

eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength,
                               uint32_t *readLength) {
  uint8_t read;
  *readLength = 0;
  while (*readLength < bufferLength && g_state.debug_uart->ReadChar(&read)) {
    g_state.leds[LED_GREEN]->Blink();

    buffer[*readLength] = read;
    g_state.debug_uart->Write((const char *)&read, 1);

    (*readLength)++;
  }
  return CONSOLE_SUCCESS;
}

eConsoleError ConsoleIoSendString(const char *buffer) {
  g_state.debug_uart->Write(buffer, strlen(buffer));
  return CONSOLE_SUCCESS;
}
