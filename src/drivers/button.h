#pragma once

#include <zephyr/drivers/gpio.h>

typedef void (*button_A_pressed_callback_t)(void);
typedef void (*button_B_pressed_callback_t)(void);

int button_setup(button_A_pressed_callback_t btn_A_pressed_cb, button_B_pressed_callback_t btn_B_pressed_cb);