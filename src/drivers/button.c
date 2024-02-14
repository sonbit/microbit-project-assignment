#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "button.h"

#define BTN_A_PIN 2
#define BTN_B_PIN 3

static const struct device *gpio0;
static const struct gpio_dt_spec btn_A = GPIO_DT_SPEC_GET(DT_NODELABEL(buttona), gpios);
static const struct gpio_dt_spec btn_B = GPIO_DT_SPEC_GET(DT_NODELABEL(buttonb), gpios);
static struct gpio_callback btn_A_cb;
static struct gpio_callback btn_B_cb;
static button_A_pressed_callback_t btn_A_pressed_callback = NULL;
static button_A_pressed_callback_t btn_B_pressed_callback = NULL;

void button_A_pressed_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_toggle(gpio0, BTN_A_PIN);
    if (btn_A_pressed_callback != NULL)
        btn_A_pressed_callback();
}

void button_B_pressed_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_toggle(gpio0, BTN_B_PIN);
    if (btn_B_pressed_callback != NULL)
        btn_B_pressed_callback();
}

int button_setup(button_A_pressed_callback_t btn_A_pressed_cb, button_B_pressed_callback_t btn_B_pressed_cb)
{
    gpio0 = DEVICE_DT_GET(DT_NODELABEL(gpio0));
    if (!gpio0)
    {
        printk("Error when getting GPIO0\n");
        return -1;
    }

    int ret;
    ret = gpio_pin_configure(gpio0, BTN_A_PIN, GPIO_OUTPUT);
    ret = gpio_pin_configure(gpio0, BTN_B_PIN, GPIO_OUTPUT);
    ret = gpio_pin_configure_dt(&btn_A, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_configure_dt(&btn_B, GPIO_INPUT | GPIO_PULL_UP);
    ret = gpio_pin_interrupt_configure_dt(&btn_A, GPIO_INT_EDGE_TO_ACTIVE);
    ret = gpio_pin_interrupt_configure_dt(&btn_B, GPIO_INT_EDGE_TO_ACTIVE);

    if (ret < 0)
        return ret;

    btn_A_pressed_callback = btn_A_pressed_cb;
    btn_B_pressed_callback = btn_B_pressed_cb;

    gpio_init_callback(&btn_A_cb, button_A_pressed_callback, BIT(btn_A.pin));
    gpio_init_callback(&btn_B_cb, button_B_pressed_callback, BIT(btn_B.pin));
    gpio_add_callback(btn_A.port, &btn_A_cb);
    gpio_add_callback(btn_B.port, &btn_B_cb);

    return ret;
}