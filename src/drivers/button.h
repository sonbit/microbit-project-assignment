#pragma once

#include <zephyr/drivers/gpio.h>

// Button pressed function signature
typedef void (*btn_pressed)(const struct device *dev, struct gpio_callback *cb, uint32_t pins);

// Button setup with gpio and callbacks
int button_setup(btn_pressed btn_A_pressed, btn_pressed btn_B_pressed);