#include <zephyr/kernel.h>

#include "button.h"

#define BTN_A_PIN 2
#define BTN_B_PIN 3

static const struct device *gpio0;

// Button IO
static const struct gpio_dt_spec btn_A = GPIO_DT_SPEC_GET(DT_NODELABEL(buttona), gpios);
static const struct gpio_dt_spec btn_B = GPIO_DT_SPEC_GET(DT_NODELABEL(buttonb), gpios);

// Button callback
static struct gpio_callback btn_A_cb;
static struct gpio_callback btn_B_cb;

int button_setup(btn_pressed btn_A_pressed, btn_pressed btn_B_pressed)
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

    gpio_init_callback(&btn_A_cb, btn_A_pressed, BIT(btn_A.pin));
    gpio_init_callback(&btn_B_cb, btn_B_pressed, BIT(btn_B.pin));
    gpio_add_callback(btn_A.port, &btn_A_cb);
    gpio_add_callback(btn_B.port, &btn_B_cb);

    return ret;
}