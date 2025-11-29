#pragma once

#include "esphome.h"
#include <vector>
#include <string>

namespace esphome {
namespace uart_bridge {

class UARTBridge : public Component {
 public:
  UARTBridge(uart::UARTComponent *mcu, uart::UARTComponent *disp);

  void setup() override;
  void loop() override;

  // getters used from YAML template lambdas
  std::string get_last_mcu_to_display() { return last_mcu_to_display_; }
  std::string get_last_display_to_mcu() { return last_display_to_mcu_; }

  // optional: configure behaviour
  void set_frame_timeout(uint16_t ms) { frame_timeout_ms_ = ms; }

 protected:
  uart::UARTComponent *uart1_{nullptr};  // MCU
  uart::UARTComponent *uart2_{nullptr};  // DISPLAY

  // Running buffers for each direction
  std::vector<uint8_t> buf_mcu_to_disp_;
  std::vector<uint8_t> buf_disp_to_mcu_;

  // Last time (millis) a byte arrived to each buffer
  uint32_t last_byte_time_mcu_to_disp_{0};
  uint32_t last_byte_time_disp_to_mcu_{0};

  // last flushed frame as hex string (for display via template sensor)
  std::string last_mcu_to_display_;
  std::string last_display_to_mcu_;

  // frame timeout (ms) — if no new byte arrives within this, we flush current buffer as a frame
  uint16_t frame_timeout_ms_{10};

  // internal helpers
  void handle_incoming_from_uart(uart::UARTComponent *from, uart::UARTComponent *to,
                                 std::vector<uint8_t> &buf, uint32_t &last_time,
                                 std::string &last_hex, const char *dir_label);

  void flush_buffer_if_timed_out(std::vector<uint8_t> &buf, uint32_t last_time,
                                 std::string &last_hex, const char *dir_label);

  static std::string hexify(const std::vector<uint8_t> &buf);
};

}  // namespace uart_bridge
}  // namespace esphome
