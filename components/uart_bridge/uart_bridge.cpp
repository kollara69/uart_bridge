#include "uart_bridge.h"
#include "esphome/core/log.h"

namespace esphome {
namespace uart_bridge {

static const char *TAG = "uart_bridge";

// Konstruktor
UARTBridge::UARTBridge(uart::UARTComponent *mcu, uart::UARTComponent *disp) {
  this->uart1 = mcu;
  this->uart2 = disp;
}

// A setup() és loop() már a header-ben definiálva van, itt nem kell újra implementálni

}  // namespace uart_bridge
}  // namespace esphome
