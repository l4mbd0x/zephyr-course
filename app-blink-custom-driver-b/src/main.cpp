#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/* Use extern "C" because main.cpp is C++ */
extern "C" {
    typedef int (*our_driver_set_state_t)(const struct device *dev, bool state);
    struct our_driver_api {
        our_driver_set_state_t set_state;
    };
    const struct our_driver_api *our_driver_custom_api_get_0(void);
}

int main(void)
{
    printk(">>> [APP] main() started\n");
    
    const struct device *dev = DEVICE_DT_GET(DT_ALIAS(my_led_sensor));
    if (!device_is_ready(dev)) {
        printk(">>> [APP] ERROR: my_led_sensor not ready\n");
        return -1;
    }

    printk(">>> [APP] device is ready\n");

    const struct our_driver_api *custom_api = our_driver_custom_api_get_0();
    if (!custom_api) {
        printk(">>> [APP] ERROR: custom API is null\n");
        return -1;
    }
    
    printk(">>> [APP] custom API pointer: %p\n", custom_api);
    printk(">>> [APP] set_state pointer: %p\n", custom_api->set_state);

    while (1) {
        sensor_sample_fetch(dev);                         
        
        printk(">>> [APP] calling custom API for TRUE\n");
        custom_api->set_state(dev, true); 
        
        k_msleep(1000);

        struct sensor_value val;
        sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);   
        
        printk(">>> [APP] calling custom API for FALSE\n");
        custom_api->set_state(dev, false);
        
        k_msleep(1000);
    }
    return 0;
}