#pragma once

#include "esphome/core/color.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/gpio.h"

#ifdef USE_TIME
#include "esphome/components/time/real_time_clock.h"
#endif

#include <Adafruit_NeoPixel.h>

namespace esphome {
namespace rgbdigit {

class RGBDigitDisplay : public PollingComponent {
   public:
    void set_writer(std::function<void(RGBDigitDisplay &)> &&writer) {
        this->writer_ = std::move(writer);
    }

    void set_pin_and_digits(InternalGPIOPin *pin, uint16_t num_digits) {
        pin_ = pin;
        num_digits_ = num_digits;
    }

    unsigned int get_num_digits() { return num_digits_; }

    void setup() override;
    void dump_config() override;

    float get_setup_priority() const override;

    void update() override;

    /// Print the given text
    void print(const Color &color, const char *str);
    /// Print the given string
    void print(const Color &color, const std::string &str);
    /// Evaluate the printf-format and print the text
    void printf(const Color &color, const char *format, ...) __attribute__((format(printf, 3, 4)));

#ifdef USE_TIME
    /// Evaluate the strftime-format and print the text
    void strftime(const Color &color, const char *format, time::ESPTime time)
        __attribute__((format(strftime, 3, 0)));
#endif

    Color get_color(unsigned int digit, byte segment);

    void set_color(const Color &color);
    void set_color(unsigned int digit, const Color &color);
    void set_color(unsigned int digit, byte segment, const Color &color);

    Color default_color();
    void set_default_color(const Color &color);

    void display();

   protected:
    void setPattern(byte pattern, unsigned int digit, const Color &color);
    void call_writer() { this->writer_(*this); }

    InternalGPIOPin *pin_ = nullptr;
    uint16_t num_digits_ = 0;
    Adafruit_NeoPixel *controller_ = nullptr;
    Color default_color_ = Color::WHITE;
    std::function<void(RGBDigitDisplay &)> writer_;
};

Color hsv_to_rgb(uint8_t hue, uint8_t sat, uint8_t val);
void rainbow_colours(RGBDigitDisplay &display, float intensity, uint8_t brightness);
void split_rainbow_colours(RGBDigitDisplay &display, float intensity, uint8_t brightness);
void random_colours(RGBDigitDisplay &display, uint8_t brightness);
void fade_colours(RGBDigitDisplay &display);

}  // namespace rgbdigit
}  // namespace esphome
