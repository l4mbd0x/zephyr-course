#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void) {
  bool led_state = true;

  while (1) {
    // ESP32-WROOM32D doesn't have an exposed internal LED
    led_state = !led_state;
    LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
    k_msleep(SLEEP_TIME_MS);
  }
  return 0;
}
