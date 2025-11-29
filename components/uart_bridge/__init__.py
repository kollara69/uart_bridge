import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@kollara69"]

uart_bridge_ns = cg.esphome_ns.namespace('uart_bridge')
UARTBridge = uart_bridge_ns.class_('UARTBridge', cg.Component)

uart_component_ns = cg.esphome_ns.namespace('uart')
UARTComponent = uart_component_ns.class_('UARTComponent')

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(UARTBridge),
    cv.Required("uart_mcu_id"): cv.use_id(UARTComponent),
    cv.Required("uart_display_id"): cv.use_id(UARTComponent),
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    uart_mcu = await cg.get_variable(config["uart_mcu_id"])
    uart_display = await cg.get_variable(config["uart_display_id"])

    var = cg.new_Pvariable(config[CONF_ID], uart_mcu, uart_display)
    await cg.register_component(var, config)

    # semmi text_sensor létrehozás itt: a YAML template sensorok a C++ gettereket fogják használni
