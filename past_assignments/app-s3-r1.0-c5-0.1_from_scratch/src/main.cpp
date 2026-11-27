#include <zephyr/init.h> // Required for SYS_INIT
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// The initialization function
static int board_init_print(void) {
  printk("Board Initialized\n");
  return 0;
}

// Register the function to run during the APPLICATION phase
SYS_INIT(board_init_print, APPLICATION, 0);

int main(void) {
  printk("Hello World from Zephyr!\n");

  while (1) {
    k_msleep(1000);
  }
  return 0;
}
