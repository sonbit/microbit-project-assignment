#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

#include "speaker.h"

#define DURATION K_MSEC(DURATION_MSEC)

static const struct pwm_dt_spec speaker = PWM_DT_SPEC_GET(DT_PATH(zephyr_user));
int first = 0;
int period;

void speaker_play()
{
    if (first == 0)
    {
        period = speaker.period;
        first = 1;
    }

    // period += PWM_USEC(50);
    pwm_set_dt(&speaker, period, period / 2U);
    k_sleep(DURATION);
    pwm_set_pulse_dt(&speaker, 0);
}