#pragma once

#include <zephyr/drivers/gpio.h>

void led_display(uint32_t row1, uint32_t row2, uint32_t row3, uint32_t row4, uint32_t row5);
void led_display_thread(void);
int led_setup();