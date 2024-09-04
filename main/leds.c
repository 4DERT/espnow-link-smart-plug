#include "leds.h"

#include "stdbool.h"

#define LED_POWER_GPIO CONFIG_GPIO_POWER_LED
#define LED_TRANSMISSION_GPIO CONFIG_GPIO_TRANSMISSION_LED

#define LED_POWER_ACTIVE CONFIG_GPIO_POWER_LED_ACTIVE_STATE
#define LED_TRANSMISSION_ACTIVE CONFIG_GPIO_TRANSMISSION_LED_ACTIVE_STATE

void leds_init() {
  // LED_POWER_GPIO
  gpio_config_t led_blue_cfg;
  led_blue_cfg.pin_bit_mask = (1UL << LED_POWER_GPIO);
  led_blue_cfg.mode = GPIO_MODE_OUTPUT;
  led_blue_cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&led_blue_cfg);
  gpio_set_level(LED_POWER_GPIO, !LED_POWER_ACTIVE);

  // LED_TRANSMISSION_GPIO
  gpio_config_t led_red_cfg;
  led_red_cfg.pin_bit_mask = (1UL << LED_TRANSMISSION_GPIO);
  led_red_cfg.mode = GPIO_MODE_OUTPUT;
  led_red_cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&led_red_cfg);
  gpio_set_level(LED_TRANSMISSION_GPIO, LED_TRANSMISSION_ACTIVE);
}

void leds_set_level(led_t led, led_level_e level) {
  switch (led) {
  case LED_POWER:
    gpio_set_level(LED_POWER_GPIO,
                   (level == LED_HIGH) ? LED_POWER_ACTIVE : !LED_POWER_ACTIVE);
    break;

  case LED_TRANSMISSION:
    gpio_set_level(LED_TRANSMISSION_GPIO, (level == LED_HIGH)
                                              ? LED_TRANSMISSION_ACTIVE
                                              : !LED_TRANSMISSION_ACTIVE);
    break;

  default:
    break;
  }
}