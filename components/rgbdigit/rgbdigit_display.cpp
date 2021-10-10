#include "rgbdigit_display.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace {

const uint8_t RGBDIGIT_UNKNOWN_CHAR = 0b11111111;

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D   X
// XABCDEFG
const uint8_t RGBDIGIT_ASCII_TO_RAW[] PROGMEM = {
    0b00000000,             // ' ', ord 0x20
    0b10110000,             // '!', ord 0x21
    0b00100010,             // '"', ord 0x22
    RGBDIGIT_UNKNOWN_CHAR,  // '#', ord 0x23
    RGBDIGIT_UNKNOWN_CHAR,  // '$', ord 0x24
    0b01001001,             // '%', ord 0x25
    RGBDIGIT_UNKNOWN_CHAR,  // '&', ord 0x26
    0b00000010,             // ''', ord 0x27
    0b01001110,             // '(', ord 0x28
    0b01111000,             // ')', ord 0x29
    0b01000000,             // '*', ord 0x2A
    RGBDIGIT_UNKNOWN_CHAR,  // '+', ord 0x2B
    0b00010000,             // ',', ord 0x2C
    0b00000001,             // '-', ord 0x2D
    0b10000000,             // '.', ord 0x2E
    RGBDIGIT_UNKNOWN_CHAR,  // '/', ord 0x2F
    0b01111110,             // '0', ord 0x30
    0b00110000,             // '1', ord 0x31
    0b01101101,             // '2', ord 0x32
    0b01111001,             // '3', ord 0x33
    0b00110011,             // '4', ord 0x34
    0b01011011,             // '5', ord 0x35
    0b01011111,             // '6', ord 0x36
    0b01110000,             // '7', ord 0x37
    0b01111111,             // '8', ord 0x38
    0b01111011,             // '9', ord 0x39
    0b01001000,             // ':', ord 0x3A
    0b01011000,             // ';', ord 0x3B
    RGBDIGIT_UNKNOWN_CHAR,  // '<', ord 0x3C
    RGBDIGIT_UNKNOWN_CHAR,  // '=', ord 0x3D
    RGBDIGIT_UNKNOWN_CHAR,  // '>', ord 0x3E
    0b01100101,             // '?', ord 0x3F
    0b01101111,             // '@', ord 0x40
    0b01110111,             // 'A', ord 0x41
    0b00011111,             // 'B', ord 0x42
    0b01001110,             // 'C', ord 0x43
    0b00111101,             // 'D', ord 0x44
    0b01001111,             // 'E', ord 0x45
    0b01000111,             // 'F', ord 0x46
    0b01011110,             // 'G', ord 0x47
    0b00110111,             // 'H', ord 0x48
    0b00110000,             // 'I', ord 0x49
    0b00111100,             // 'J', ord 0x4A
    RGBDIGIT_UNKNOWN_CHAR,  // 'K', ord 0x4B
    0b00001110,             // 'L', ord 0x4C
    RGBDIGIT_UNKNOWN_CHAR,  // 'M', ord 0x4D
    0b00010101,             // 'N', ord 0x4E
    0b01111110,             // 'O', ord 0x4F
    0b01100111,             // 'P', ord 0x50
    0b11111110,             // 'Q', ord 0x51
    0b00000101,             // 'R', ord 0x52
    0b01011011,             // 'S', ord 0x53
    0b00000111,             // 'T', ord 0x54
    0b00111110,             // 'U', ord 0x55
    0b00111110,             // 'V', ord 0x56
    0b00111111,             // 'W', ord 0x57
    RGBDIGIT_UNKNOWN_CHAR,  // 'X', ord 0x58
    0b00100111,             // 'Y', ord 0x59
    0b01101101,             // 'Z', ord 0x5A
    0b01001110,             // '[', ord 0x5B
    RGBDIGIT_UNKNOWN_CHAR,  // '\', ord 0x5C
    0b01111000,             // ']', ord 0x5D
    RGBDIGIT_UNKNOWN_CHAR,  // '^', ord 0x5E
    0b00001000,             // '_', ord 0x5F
    0b00100000,             // '`', ord 0x60
    0b01110111,             // 'a', ord 0x61
    0b00011111,             // 'b', ord 0x62
    0b00001101,             // 'c', ord 0x63
    0b00111101,             // 'd', ord 0x64
    0b01001111,             // 'e', ord 0x65
    0b01000111,             // 'f', ord 0x66
    0b01011110,             // 'g', ord 0x67
    0b00010111,             // 'h', ord 0x68
    0b00010000,             // 'i', ord 0x69
    0b00111100,             // 'j', ord 0x6A
    RGBDIGIT_UNKNOWN_CHAR,  // 'k', ord 0x6B
    0b00001110,             // 'l', ord 0x6C
    RGBDIGIT_UNKNOWN_CHAR,  // 'm', ord 0x6D
    0b00010101,             // 'n', ord 0x6E
    0b00011101,             // 'o', ord 0x6F
    0b01100111,             // 'p', ord 0x70
    RGBDIGIT_UNKNOWN_CHAR,  // 'q', ord 0x71
    0b00000101,             // 'r', ord 0x72
    0b01011011,             // 's', ord 0x73
    0b00000111,             // 't', ord 0x74
    0b00011100,             // 'u', ord 0x75
    0b00011100,             // 'v', ord 0x76
    RGBDIGIT_UNKNOWN_CHAR,  // 'w', ord 0x77
    RGBDIGIT_UNKNOWN_CHAR,  // 'x', ord 0x78
    0b00100111,             // 'y', ord 0x79
    RGBDIGIT_UNKNOWN_CHAR,  // 'z', ord 0x7A
    0b00110001,             // '{', ord 0x7B
    0b00000110,             // '|', ord 0x7C
    0b00000111,             // '}', ord 0x7D
    0b01100011,             // '~', ord 0x7E (degree symbol)
};

}  // namespace

namespace esphome {
namespace rgbdigit {

static const char *TAG = "rgbdigit";

void RGBDigitDisplay::setup() {
    controller_ =
        new Adafruit_NeoPixel{(uint16_t)(8 * num_digits_), pin_->get_pin(), NEO_GRB + NEO_KHZ800};
    controller_->begin();
    controller_->clear();
    controller_->show();
}

void RGBDigitDisplay::dump_config() {
    ESP_LOGCONFIG(TAG, "RGBDigit:");
    LOG_PIN("  Pin: ", this->pin_);
    ESP_LOGCONFIG(TAG, "  Num Digits: %d", this->num_digits_);
    LOG_UPDATE_INTERVAL(this);
}

float RGBDigitDisplay::get_setup_priority() const { return setup_priority::PROCESSOR; }

void RGBDigitDisplay::update() {
    this->call_writer();
    controller_->show();
}

void RGBDigitDisplay::print(const Color &color, const char *str) {
    if (controller_) {
        unsigned int digit = 0;
        for (; *str != '\0'; str++) {
            uint8_t data = RGBDIGIT_UNKNOWN_CHAR;
            if (*str >= ' ' && *str <= '~')
                data = pgm_read_byte(&RGBDIGIT_ASCII_TO_RAW[*str - ' ']);

            if (data == RGBDIGIT_UNKNOWN_CHAR) {
                ESP_LOGW(TAG,
                         "Encountered character '%c' with no RGBDigit representation while "
                         "translating string!",
                         *str);
            }
            // Remap segments, for compatibility with MAX7219 segment definition
            data = ((data & 0x80) ? 0x80 : 0) |  // no move X
                   ((data & 0x40) ? 0x1 : 0) |   // A
                   ((data & 0x20) ? 0x2 : 0) |   // B
                   ((data & 0x10) ? 0x4 : 0) |   // C
                   ((data & 0x8) ? 0x8 : 0) |    // D
                   ((data & 0x4) ? 0x10 : 0) |   // E
                   ((data & 0x2) ? 0x20 : 0) |   // F
                   ((data & 0x1) ? 0x40 : 0);    // G
            if (*str == '.') {
                if (digit != 0) digit--;
                controller_->setPixelColor((digit << 3) + 7, color.raw_32);
            } else {
                if (digit >= num_digits_) {
                    ESP_LOGE(TAG, "String is too long for the display!");
                    break;
                }
                setPattern(data, digit, color.raw_32);
            }
            digit++;
        }
    }
}

void RGBDigitDisplay::print(const Color &color, const std::string &str) {
    this->print(color, str.c_str());
}

void RGBDigitDisplay::printf(const Color &color, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    char buffer[64];
    int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
    va_end(arg);
    if (ret > 0) this->print(color, buffer);
}

#ifdef USE_TIME
void RGBDigitDisplay::strftime(const Color &color, const char *format, time::ESPTime time) {
    char buffer[64];
    size_t ret = time.strftime(buffer, sizeof(buffer), format);
    if (ret > 0) this->print(color, buffer);
}
#endif

Color RGBDigitDisplay::get_color(unsigned int digit, byte segment) {
    return Color{controller_->getPixelColor((digit << 3) + segment)};
}

void RGBDigitDisplay::set_color(const Color &color) {
    for (int digit = 0; digit < num_digits_; digit++) {
        set_color(digit, color);
    }
}

void RGBDigitDisplay::set_color(unsigned int digit, const Color &color) {
    for (byte segment = 0; segment < 8; segment++) {
        set_color(digit, segment, color);
    }
}

void RGBDigitDisplay::set_color(unsigned int digit, byte segment, const Color &color) {
    const int pixel = (digit << 3) + segment;
    if (controller_->getPixelColor(pixel)) {
        controller_->setPixelColor((digit << 3) + segment, color.raw_32);
    }
}

Color RGBDigitDisplay::default_color() { return default_color_; }

void RGBDigitDisplay::set_default_color(const Color &color) { default_color_ = color; }

void RGBDigitDisplay::display() { controller_->show(); }

void RGBDigitDisplay::setPattern(byte pattern, unsigned int digit, const Color &color) {
    byte bitMask = 1;
    for (int segment = 0; segment < 8; segment++) {
        if (pattern & bitMask)
            controller_->setPixelColor((digit << 3) + segment, color.r, color.g, color.b);
        else
            controller_->setPixelColor((digit << 3) + segment, 0);
        bitMask <<= 1;
    }
}

Color hsv_to_rgb(uint8_t hue, uint8_t sat, uint8_t val) {
    // based on FastLED's hsv rainbow to rgb
    // upper 3 hue bits are for branch selection, lower 5 are for values
    const uint8_t offset8 = (hue & 0x1F) << 3;  // 0..248
    // third of the offset, 255/3 = 85 (actually only up to 82; 164)
    const uint8_t third = esp_scale8(offset8, 85);
    const uint8_t two_thirds = esp_scale8(offset8, 170);
    Color rgb(255, 255, 255, 0);
    switch (hue >> 5) {
        case 0b000:
            rgb.r = 255 - third;
            rgb.g = third;
            rgb.b = 0;
            break;
        case 0b001:
            rgb.r = 171;
            rgb.g = 85 + third;
            rgb.b = 0;
            break;
        case 0b010:
            rgb.r = 171 - two_thirds;
            rgb.g = 170 + third;
            rgb.b = 0;
            break;
        case 0b011:
            rgb.r = 0;
            rgb.g = 255 - third;
            rgb.b = third;
            break;
        case 0b100:
            rgb.r = 0;
            rgb.g = 171 - two_thirds;
            rgb.b = 85 + two_thirds;
            break;
        case 0b101:
            rgb.r = third;
            rgb.g = 0;
            rgb.b = 255 - third;
            break;
        case 0b110:
            rgb.r = 85 + third;
            rgb.g = 0;
            rgb.b = 171 - third;
            break;
        case 0b111:
            rgb.r = 170 + third;
            rgb.g = 0;
            rgb.b = 85 - third;
            break;
        default:
            break;
    }
    // low saturation -> add uniform color to orig. hue
    // high saturation -> use hue directly
    // scales with square of saturation
    // (r,g,b) = (r,g,b) * sat + (1 - sat)^2
    rgb *= sat;
    const uint8_t desat = 255 - sat;
    rgb += esp_scale8(desat, desat);
    // (r,g,b) = (r,g,b) * val
    rgb *= val;
    return rgb;
}

void rainbow_colours(RGBDigitDisplay &display, float intensity, uint8_t brightness) {
    const auto now = millis();
    const float sToOffset[8] = {0.5, 1.0, 1.0, 0.5, 0.0, 0.0, 0.5, 1.0};
    const float rainbowOffset = now * intensity / 1000.0;
    for (unsigned int d = 0; d < display.get_num_digits(); ++d) {
        for (byte s = 0; s < 8; ++s) {
            const float offset =
                (d + sToOffset[s] + rainbowOffset) / (display.get_num_digits() * 3);
            float _;
            const float hue = std::modf(offset, &_);
            const auto color = hsv_to_rgb(hue * 255, 255, brightness);
            display.set_color(d, s, color);
        }
    }
}

void random_colours(RGBDigitDisplay &display, uint8_t brightness) {
    for (unsigned int d = 0; d < display.get_num_digits(); ++d) {
        for (byte s = 0; s < 8; ++s) {
            const auto color = hsv_to_rgb(fast_random_8(), 255, brightness);
            display.set_color(d, s, color);
        }
    }
}

void fade_colours(RGBDigitDisplay &display) {
    for (unsigned int d = 0; d < display.get_num_digits(); ++d) {
        for (byte s = 0; s < 8; ++s) {
            const auto color = display.get_color(d, s).fade_to_black(230);
            display.set_color(d, s, color);
        }
    }
}

}  // namespace rgbdigit
}  // namespace esphome
