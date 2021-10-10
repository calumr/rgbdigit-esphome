# RGBDigit numeric display

A component for [ESPHome](https://esphome.io/index.html) that enables use of the [RGBDigit](https://www.rgbdigit.com/). Similar to a [TM1637](https://esphome.io/components/display/tm1637.html), but with RGB color for each segment.

Example:
```yaml
external_components:
    - source: github://calumr/rgbdigit-esphome@master

# Example configuration entry
display:
    - platform: rgbdigit
    pin: 4
    num_digits: 4
    update_interval: 0.1s
    lambda: |-
        const auto now = id(sntp_time).now();
        const char * fmt = now.second % 2 ? "%l.%M" : "%l%M";
        it.strftime(Color::WHITE, fmt, now);
        rainbow_colours(it, 4.0f, 255);
```

## Configuration variables:

- **pin** (**Required**): The pin you have the data line hooked up to.
- **num_digits** (**Required**, integer): The number of RGBDigits.
- **lambda** (*Optional*): The lambda to use for rendering the content on the RGBDigits.
  See "Rendering Lambda" below for more information.
- **update_interval** (*Optional*): The interval to re-draw the screen. Defaults to ``1s``.
- **id** (*Optional*): Manually specify the ID used for code generation.

## Rendering Lambda

The RGBDigit has a similar API to the fully fledged [Display](https://esphome.io/components/display/index.html), but it's only a subset as the RGBDigit
7-segment displays don't have a concept of individual pixels. In the lambda you're passed a variable called ``it``
as with all other displays. In this case however, ``it`` is a RGBDigitDisplay instance (see API Reference).

```yaml
display:
    - platform: rgbdigit
    # ...
    lambda: |-
        it.print(Color{255, 0, 0}, "12.34");    // Prints "12.34" in red
        it.set_color(2, Color{0, 255, 0});      // Sets digit 2 to green
        it.set_color(1, 7, Color{0, 0, 255});   // Sets the decimal point (segment 7) to blue
```

Please see [Formatted Text](https://esphome.io/components/display/index.html#formatted-text)] for a quick introduction into the ``printf`` formatting rules and
[Displaying Time](https://esphome.io/components/display/index.html#displaying-time) for an introduction into the ``strftime`` time formatting.