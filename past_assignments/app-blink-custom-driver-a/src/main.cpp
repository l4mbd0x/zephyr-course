#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_ALIAS(my_led_sensor));
    struct sensor_value val;

    if (!device_is_ready(dev)) {
        printk("my_led_sensor not ready\n");
        return -1;
    }

    while (1) {
        sensor_sample_fetch(dev);                         /* LED on (green) */
        k_msleep(1000);
        sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);   /* LED off */
        k_msleep(1000);
    }
    return 0;
}
