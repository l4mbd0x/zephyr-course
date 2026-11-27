#include <zephyr/device.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define STRIP_NODE DT_ALIAS(app_led)

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

int main(void) {
  if (!device_is_ready(strip)) {
    LOG_ERR("LED strip not ready");
    return 0;
  }

  struct led_rgb colors[3] = {
      {.r = 0x10, .g = 0x00, .b = 0x00}, // red
      {.r = 0x00, .g = 0x00, .b = 0x10}, // blue
      {.r = 0x00, .g = 0x00, .b = 0x00}, // off
  };
  int idx = 0;

  while (1) {
    // Apply to the rgb led red, blue, off order as blinking pattern
    int ret = led_strip_update_rgb(strip, &colors[idx], 1);
    LOG_INF("idx=%d ret=%d", idx, ret);
    idx = (idx + 1) % 3;
    k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
  }

  return 0;
}
