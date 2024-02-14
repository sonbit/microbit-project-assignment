#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

#include "speaker.h"

#define DURATION K_MSEC(DURATION_MSEC)
#define HIGH_PERIOD 4500000
#define LOW_PERIOD 1495000

static const struct pwm_dt_spec speaker = PWM_DT_SPEC_GET(DT_PATH(zephyr_user));
int first = 0;

void speaker_play(int period, int duration)
{
    pwm_set_dt(&speaker, period, period * 0.75);
    k_sleep(K_MSEC(duration));
    pwm_set_pulse_dt(&speaker, 0);
}

void speaker_play_default_note(int duration)
{
    speaker_play(speaker.period, duration);
}

void speaker_play_high_note(int duration)
{
    speaker_play(LOW_PERIOD, duration);
}

void speaker_play_low_note(int duration)
{
    speaker_play(HIGH_PERIOD, duration);
}