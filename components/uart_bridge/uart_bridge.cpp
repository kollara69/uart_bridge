#include "uart_bridge.h"
#include "esphome/core/log.h"
#include <sstream>
#include <iomanip>

namespace esphome {
namespace uart_bridge {

static const char *TAG = "uart_bridge";

UARTBridge::UARTBridge(uart::UARTComponent *mcu, uart::UARTComponent *disp) {
  this->uart1_ = mcu;
  this->uart2_ = disp;
}

void UARTBridge::setup() {
  ESP_LOGI(TAG, "UART Bridge setup complete (frame timeout %ums)", frame_timeout_ms_);
}

void UARTBridge::loop() {
  const uint32_t now = millis();

  // process incoming bytes and forward (MCU -> DISPLAY)
  handle_incoming_from_uart(uart1_, uart2_, buf_mcu_to_disp_, last_byte_time_mcu_to_disp_, last_mcu_to_display_, "MCU->DISPLAY");

  // process incoming bytes and forward (DISPLAY -> MCU)
  handle_incoming_from_uart(uart2_, uart1_, buf_disp_to_mcu_, last_byte_time_disp_to_mcu_, last_display_to_mcu_, "DISPLAY->MCU");

  // check timeouts for both buffers
  flush_buffer_if_timed_out(buf_mcu_to_disp_, last_byte_time_mcu_to_disp_, last_mcu_to_display_, "MCU->DISPLAY");
  flush_buffer_if_timed_out(buf_disp_to_mcu_, last_byte_time_disp_to_mcu_, last_display_to_mcu_, "DISPLAY->MCU");
}

void UARTBridge::handle_incoming_from_uart(uart::UARTComponent *from, uart::UARTComponent *to,
                                           std::vector<uint8_t> &buf, uint32_t &last_time,
                                           std::string &last_hex, const char *dir_label) {
  uint8_t b;
  bool any = false;
  while (from->available()) {
    if (from->read_byte(&b)) {
      any = true;
      // append to buffer
      buf.push_back(b);
      last_time = millis();

      // forward immediately so the target MCU receives bytes in real time
      to->write_byte(b);

      // Optional debug: very verbose logs per-byte commented out by default
      // ESP_LOGD(TAG, "%s byte 0x%02X (%c)", dir_label, b, isprint(b) ? b : '.');
    } else {
      break;
    }
  }

  // If many bytes arrived and buffer grew large, we might flush immediately to avoid waiting
  const size_t max_immediate_flush = 1024;
  if (buf.size() >= max_immediate_flush) {
    ESP_LOGW(TAG, "%s buffer large (%u bytes), flushing proactively", dir_label, (unsigned)buf.size());
    last_hex = hexify(buf);
    ESP_LOGI(TAG, "[%s] %s", dir_label, last_hex.c_str());
    buf.clear();
  }
}

void UARTBridge::flush_buffer_if_timed_out(std::vector<uint8_t> &buf, uint32_t last_time,
                                           std::string &last_hex, const char *dir_label) {
  if (buf.empty()) return;
  const uint32_t now = millis();
  // if last_time is 0, no bytes were ever received yet
  if (last_time == 0) return;

  if ((uint32_t)(now - last_time) >= (uint32_t)frame_timeout_ms_) {
    // flush as a single frame
    last_hex = hexify(buf);
    ESP_LOGI(TAG, "[%s] %s", dir_label, last_hex.c_str());
    buf.clear();
    last_time = 0;
  }
}

std::string UARTBridge::hexify(const std::vector<uint8_t> &buf) {
  if (buf.empty()) return "";
  std::ostringstream oss;
  oss << std::uppercase << std::hex << std::setfill('0');
  for (size_t i = 0; i < buf.size(); ++i) {
    if (i) oss << ' ';
    oss << std::setw(2) << (int)(buf[i]);
  }
  return oss.str();
}

}  // namespace uart_bridge
}  // namespace esphome
