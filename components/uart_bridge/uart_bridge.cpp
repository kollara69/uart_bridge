#include "uart_bridge.h"
static const char *TAG = "uart_bridge";

UARTBridge::UARTBridge(uart::UARTComponent *mcu, uart::UARTComponent *disp) {
  this->uart1 = mcu;
  this->uart2 = disp;
}

void setup() { /* … */ }
void loop() { /* … */ }

// Registration
void register_uart_bridge_component(UARTBridge *comp) {
  // esp/home registration code
}


namespace esphome {
namespace uart_bridge {

// No additional registration needed – all logic in h file.
// This file exists to make ESPHome's build system happy.

}  // namespace uart_bridge
}  // namespace esphome
