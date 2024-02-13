#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/display.h>

#include "led.h"

// Code is based on https://github.com/zephyrproject-rtos/zephyr/blob/main/samples/boards/nrf/nrf_led_matrix/src/main.c

#define BUF_SIZE 5

static const struct device *device;
static struct display_capabilities caps;
static uint8_t buf[BUF_SIZE];
static const struct display_buffer_descriptor buf_desc = {
    .buf_size = BUF_SIZE,
    .width = BUF_SIZE,
    .height = BUF_SIZE,
    .pitch = 8,
};

void led_display(uint32_t row1, uint32_t row2, uint32_t row3, uint32_t row4, uint32_t row5)
{
    int ret;
    ret = display_set_brightness(device, 0x7F);
    if (ret < 0)
        printk("Display Set Brightness Error\n");

    buf[0] = row1;
    buf[1] = row2;
    buf[2] = row3;
    buf[3] = row4;
    buf[4] = row5;

    ret = display_write(device, 0, 0, &buf_desc, buf);
    if (ret < 0)
        printk("Display Write Error\n");

    ret = display_blanking_off(device);
    if (ret < 0)
        printk("Display Blanking Off Error\n");
}

int led_setup()
{
    device = DEVICE_DT_GET_ONE(nordic_nrf_led_matrix);
    if (!device)
    {
        printk("Error when getting LED_MATRIX\n");
        return -1;
    }

    display_get_capabilities(device, &caps);
    if (!(caps.supported_pixel_formats & PIXEL_FORMAT_MONO01))
    {
        printk("Pixel Format Not Supported\n");
        return -1;
    }

    if (display_set_pixel_format(device, PIXEL_FORMAT_MONO01) < 0)
    {
        printk("Pixel Set Format Error\n");
        return -2;
    }

    return 0;
}