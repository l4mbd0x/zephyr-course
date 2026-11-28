/*
Without it, every DT_INST_* macro uses an empty compat string, matches no nodes,
and DT_INST_FOREACH_STATUS_OKAY produces no instances
*/
#define DT_DRV_COMPAT our_driver

#include <zephyr/device.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>

struct our_driver_config {
  const struct device *strip;
};

struct our_driver_data {
  /* no persistent state needed for a simple color toggle */
};

static int our_driver_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan) {
  const struct our_driver_config *cfg = dev->config;
  struct led_rgb pixel = {.r = 0, .g = 255, .b = 0}; /* Green */

  if (chan != SENSOR_CHAN_ALL) {
    return -ENOTSUP;
  }

  return led_strip_update_rgb(cfg->strip, &pixel, 1);
}

static int our_driver_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val) {
  const struct our_driver_config *cfg = dev->config;
  struct led_rgb pixel = {.r = 0, .g = 0, .b = 0}; /* Off */

  if (chan != SENSOR_CHAN_ALL) {
    return -ENOTSUP;
  }

  (void)led_strip_update_rgb(cfg->strip, &pixel, 1);
  val->val1 = 0;
  val->val2 = 0;
  return 0;
}

static const struct sensor_driver_api our_driver_api = {
    .sample_fetch = our_driver_sample_fetch,
    .channel_get = our_driver_channel_get,
};

static int our_driver_init(const struct device *dev) {
  const struct our_driver_config *cfg = dev->config;

  if (!device_is_ready(cfg->strip)) {
    return -ENODEV;
  }
  return 0;
}

#define OUR_DRIVER_INIT(inst)                                                  \
  static const struct our_driver_config our_driver_config_##inst = {           \
      .strip = DEVICE_DT_GET(DT_INST_PHANDLE(inst, led_strip)),                \
  };                                                                           \
  DEVICE_DT_INST_DEFINE(inst, our_driver_init, NULL, NULL,                     \
                        &our_driver_config_##inst, POST_KERNEL,                \
                        CONFIG_SENSOR_INIT_PRIORITY, &our_driver_api);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_INIT)
