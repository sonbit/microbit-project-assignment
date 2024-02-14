#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "drivers/button.h"
#include "drivers/led.h"
#include "drivers/speaker.h"
#include "timer.h"

#define STACKSIZE 4096
#define BUTTON_TASK_PRIORITY -1
#define DISPLAY_TASK_PRIORITY 2
#define METRONOME_TASK_PRIORITY 1

#define BTN_TASK_DELAY K_MSEC(300)
#define TASK_DELAY 200
#define MET_TASK_DELAY K_MSEC(TASK_DELAY)
#define DISP_TASK_DELAY K_MSEC(TASK_DELAY / 4)

K_SEM_DEFINE(btn_A_sem, 0, 1);
K_SEM_DEFINE(btn_B_sem, 0, 1);
K_SEM_DEFINE(disp_sem, 0, 1);
K_SEM_DEFINE(met_sem, 0, 1);

int counter = 500;

void btn_A_pressed(void)
{
    counter += 10;
    k_sem_give(&btn_A_sem);
}

void btn_B_pressed(void)
{
    counter -= 10;
    k_sem_give(&btn_B_sem);
}

void timer_interrupt()
{
    printk("Timer Interrupt\n");
    k_sem_give(&disp_sem);
    k_sem_give(&met_sem);
}

void button_A_task(void)
{
    while (1)
    {
        k_sem_take(&btn_A_sem, K_FOREVER);
        speaker_play_high_note(25);
        k_sleep(BTN_TASK_DELAY);
    }
}

void button_B_task(void)
{
    while (1)
    {
        k_sem_take(&btn_B_sem, K_FOREVER);
        speaker_play_low_note(25);
        k_sleep(BTN_TASK_DELAY);
    }
}

void display_task(void)
{
    while (1)
    {
        k_sem_take(&disp_sem, K_FOREVER);
        printk("DISPLAY TASK\n");
        led_display(counter, counter >> 1, counter >> 2, counter >> 3, counter >> 4);
        k_sleep(DISP_TASK_DELAY);
        led_display(0, 0, 0, 0, 0);
        k_sleep(DISP_TASK_DELAY);
    }
}

void metronome_task(void)
{
    while (1)
    {
        k_sem_take(&met_sem, K_FOREVER);
        printk("METRONOME TASK\n");
        // speaker_play_default_note(25);
        k_sleep(K_MSEC(TASK_DELAY - DURATION_MSEC));
    }
}

K_THREAD_DEFINE(thread0_id, STACKSIZE, button_A_task, NULL, NULL, NULL, BUTTON_TASK_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACKSIZE, button_B_task, NULL, NULL, NULL, BUTTON_TASK_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread2_id, STACKSIZE, display_task, NULL, NULL, NULL, DISPLAY_TASK_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread3_id, STACKSIZE, metronome_task, NULL, NULL, NULL, METRONOME_TASK_PRIORITY, 0, 0);

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

    timer_setup(timer_interrupt);

    printk("Setup Completed Successfully\n");

    while (1)
    {
        k_sleep(K_MSEC(100));
    }
}
