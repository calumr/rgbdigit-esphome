import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import display
from esphome.const import (
    CONF_PIN,
    CONF_ID,
    CONF_LAMBDA
)

CODEOWNERS = ["@calumr"]

CONF_NUM_DIGITS = "num_digits"

rgbdigit_ns = cg.esphome_ns.namespace("rgbdigit")
RGBDigitDisplay = rgbdigit_ns.class_("RGBDigitDisplay", cg.PollingComponent)
RGBDigitDisplayRef = RGBDigitDisplay.operator("ref")

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(RGBDigitDisplay),
        cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_NUM_DIGITS, default=1): cv.int_range(min=1, max=10),
    }
).extend(cv.polling_component_schema("1s"))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield display.register_display(var, config)

    pin = yield cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin_and_digits(pin, config[CONF_NUM_DIGITS]))

    if CONF_LAMBDA in config:
        lambda_ = yield cg.process_lambda(
            config[CONF_LAMBDA], [(RGBDigitDisplayRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))

    cg.add_library("adafruit/Adafruit NeoPixel", "1.8.7")