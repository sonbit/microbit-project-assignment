#pragma once

typedef void (*timer_callback_t)(void);

void timer_setup(timer_callback_t callback);