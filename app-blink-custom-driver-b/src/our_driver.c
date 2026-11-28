/*
Without it, every DT_INST_* macro uses an empty compat string, matches no nodes,
and DT_INST_FOREACH_STATUS_OKAY produces no instances
*/
#define DT_DRV_COMPAT our_driver

#include <zephyr/device.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* --- 1. STRUCTS --- */

/* Static configuration (from Devicetree) */
struct our_driver_config {
  const struct device *strip;
};

/* Dynamic data (for Task 2 state tracking) */
struct our_driver_data {
  bool state;
};

/* --- 2. CUSTOM API --- */

/* Define the custom API structure */
typedef int (*our_driver_set_state_t)(const struct device *dev, bool state);
struct our_driver_api {
  our_driver_set_state_t set_state;
};

/* Implement the custom function */
static int our_driver_set_state(const struct device *dev, bool state) {
  struct our_driver_data *data = dev->data;
  data->state = state;

  printk("our driver: RGB led %s\n", state ? "true" : "false");
  return 0;
}

/* --- 3. SENSOR API --- */

static int our_driver_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan) {
  const struct our_driver_config *cfg = dev->config;
  struct our_driver_data *data = dev->data;
  struct led_rgb pixel = {.r = 0, .g = 255, .b = 0}; /* Green */

  if (chan != SENSOR_CHAN_ALL)
    return -ENOTSUP;

  data->state = true;
  return led_strip_update_rgb(cfg->strip, &pixel, 1);
}

static int our_driver_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val) {
  const struct our_driver_config *cfg = dev->config;
  struct our_driver_data *data = dev->data;
  struct led_rgb pixel = {.r = 0, .g = 0, .b = 0}; /* Off */

  if (chan != SENSOR_CHAN_ALL)
    return -ENOTSUP;

  data->state = false;
  (void)led_strip_update_rgb(cfg->strip, &pixel, 1);
  val->val1 = 0;
  val->val2 = 0;
  return 0;
}

static const struct sensor_driver_api our_sensor_api = {
    .sample_fetch = our_driver_sample_fetch,
    .channel_get = our_driver_channel_get,
};

/* --- 4. INIT AND DEVICE DEFINE --- */

static int our_driver_init(const struct device *dev) {
  printk(">>> [DRIVER] init() started\n"); 

  const struct our_driver_config *cfg = dev->config;
  struct our_driver_data *data = dev->data;

  if (!device_is_ready(cfg->strip)) {
    printk(">>> [DRIVER] ERROR: LED strip not ready\n");
    return -ENODEV;
  }

  printk(">>> [DRIVER] init() complete\n");
  data->state = false;
  return 0;
}

#define OUR_DRIVER_INIT(inst)                                                  \
  static const struct our_driver_config our_driver_config_##inst = {           \
      .strip = DEVICE_DT_GET(DT_INST_PHANDLE(inst, led_strip)),                \
  };                                                                           \
  static struct our_driver_data our_driver_data_##inst;                        \
  static const struct our_driver_api our_driver_custom_api_##inst = {          \
      .set_state = our_driver_set_state,                                       \
  };                                                                           \
  DEVICE_DT_INST_DEFINE(inst, our_driver_init, NULL, &our_driver_data_##inst,  \
                        &our_driver_config_##inst, POST_KERNEL,                \
                        CONFIG_SENSOR_INIT_PRIORITY, &our_sensor_api);         \
  /* Expose the custom API so main.c can find it */                            \
  const struct our_driver_api *our_driver_custom_api_get_##inst(void) {        \
    return &our_driver_custom_api_##inst;                                      \
  }

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_INIT)