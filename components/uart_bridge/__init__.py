import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

# A C++ kódunk (uart_bridge.h) namespace és osztálya
# Itt feltételezzük, hogy a komponens neve 'uart_bridge'
CODEOWNERS = ["@kollara69"]
uart_bridge_ns = cg.esphome_ns.namespace('uart_bridge')
UARTBridge = uart_bridge_ns.class_('UARTBridge', cg.Component)

# Importáljuk a UART komponens osztályt, hogy tudjunk hivatkozni rá
uart_component_ns = cg.esphome_ns.namespace('uart')
UARTComponent = uart_component_ns.class_('UARTComponent', cg.Component)

# A Komponens Konfigurációs Sémája (CONFIG_SCHEMA)
# Ez definálja, milyen beállításokat fogad el a YAML
CONFIG_SCHEMA = cv.Schema({
    # A kötelező 'id' beállítás, ami az objektum ID-jét adja meg
    cv.GenerateID(): cv.declare_id(UARTBridge),
    
    # A 'uart_mcu_id' beállítás, ami a C++ kódban lévő UART komponensre hivatkozik
    cv.Required("uart_mcu_id"): cv.use_id(UARTComponent),
    
    # A 'uart_display_id' beállítás, ami a C++ kódban lévő UART komponensre hivatkozik
    cv.Required("uart_display_id"): cv.use_id(UARTComponent),
}).extend(cv.COMPONENT_SCHEMA) # Minden komponenshez szükséges alapvető beállítások

# A kódgenerátor függvény (to_code)
# Ez a függvény hozza létre a C++ kódot az inicializáláshoz
async def to_code(config):
    # UART komponensek ID-inek feloldása
    uart_mcu = await cg.get_variable(config["uart_mcu_id"])
    uart_display = await cg.get_variable(config["uart_display_id"])

    # A UARTBridge objektum létrehozása az ID-vel
    var = cg.new_Pvariable(config[CONF_ID], uart_mcu, uart_display)
    await cg.register_component(var, config)

    # A C++ kód visszatérő értéke, ami végül beépül a main.cpp-be
