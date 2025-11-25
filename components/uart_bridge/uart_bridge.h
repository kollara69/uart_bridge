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

  void setup() override {}

  void loop() override {
    uint8_t b;

    // MCU → DISPLAY
    while (uart1->available()) {
      if (uart1->read_byte(&b)) {
        if (sensor_mcu_to_display) {
          sensor_mcu_to_display->publish_state(std::to_string(b));
        }
        uart2->write_byte(b);
        ESP_LOGD("uart_bridge", "MCU->DISPLAY 0x%02X (%c)", b, isprint(b) ? b : '.');
      }
    }

    // DISPLAY → MCU
    while (uart2->available()) {
      if (uart2->read_byte(&b)) {
        if (sensor_display_to_mcu) {
          sensor_display_to_mcu->publish_state(std::to_string(b));
        }
        uart1->write_byte(b);
        ESP_LOGD("uart_bridge", "DISPLAY->MCU 0x%02X (%c)", b, isprint(b) ? b : '.');
      }
    }
  }
};

}  // namespace uart_bridge
}  // namespace esphome
