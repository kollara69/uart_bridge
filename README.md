# UART Bridge for ESPHome

This is an external component that transparently bridges two UART ports
while publishing received bytes as text sensors in Home Assistant.

## Installation

Add to your ESPHome YAML:

```yaml
external_components:
  - source: github://kollara69/uart_bridge
    components: [ uart_bridge ]
