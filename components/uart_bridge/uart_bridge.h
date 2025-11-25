#pragma once

#include "esphome.h"

namespace esphome {
namespace uart_bridge {

class UARTBridge : public Component {
 public:
  uart::UARTComponent *uart1{nullptr};
  uart::UARTComponent *uart2{nullptr};

  text_sensor::TextSensor *sensor_mcu_to_display{nullptr};
  text_sensor::TextSensor *sensor_display_to_mcu{nullptr};

  // Konstruktor
  UARTBridge(uart::UARTComponent *mcu, uart::UARTComponent *disp) {
    uart1 = mcu;
    uart2 = disp;
  }

  void setup() override
