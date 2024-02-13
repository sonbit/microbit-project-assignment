#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>

#include "drivers/button.h"
#include "drivers/led.h"
#include "drivers/speaker.h"

#define STACKSIZE 1024
#define DISPLAY_TASK_PRIORITY -1
#define METRONOME_TASK_PRIORITY 1

// Semaphore to ensure setup has completed before starting thread
K_SEM_DEFINE(startup_sem, 0, 1);

int metronome_counter = 500;

void btn_A_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    metronome_counter += 10;
}

void btn_B_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    metronome_counter -= 10;
}

void led_display_thread(void)
{
    k_sem_take(&startup_sem, K_FOREVER);

    while (1)
    {
        led_display(metronome_counter, metronome_counter >> 1, metronome_counter >> 2, metronome_counter >> 3, metronome_counter >> 4);
        k_sleep(K_MSEC(metronome_counter / 2));
        led_display(0, 0, 0, 0, 0);
        k_sleep(K_MSEC(metronome_counter / 2));
    }
}

void metronome_task(void)
{
    k_sem_take(&startup_sem, K_FOREVER);

    while (1)
    {
        // speaker_play();
        k_sleep(K_MSEC(metronome_counter - DURATION_MSEC));
    }
}

K_THREAD_DEFINE(thread0_id, STACKSIZE, led_display_thread, NULL, NULL, NULL, DISPLAY_TASK_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACKSIZE, metronome_task, NULL, NULL, NULL, METRONOME_TASK_PRIORITY, 0, 0);

int main(void)
{
    printk("Starting Application\n");

    if (button_setup(btn_A_pressed, btn_B_pressed) < 0)
    {
        printk("Error when setting up buttons\n");
        return -1;
    }

    if (led_setup() < 0)
    {
        printk("Error when setting up leds\n");
        return -1;
    }

    printk("Setup Completed Successfully\n");
    k_sem_give(&startup_sem);
    k_sem_give(&startup_sem);

    while (1)
    {
        k_sleep(K_MSEC(1));
    }
}
