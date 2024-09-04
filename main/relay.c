#include "relay.h"

#include <stdbool.h>

#include "leds.h"

#define RELAY_GPIO CONFIG_GPIO_RELAY
#define RELAY_HIGH CONFIG_GPIO_RELAY_ACTIVE_STATE
#define RELAY_LOW !CONFIG_GPIO_RELAY_ACTIVE_STATE

void relay_init() {
  gpio_config_t relay_cfg;
  relay_cfg.pin_bit_mask = (1UL << RELAY_GPIO);
  relay_cfg.mode = GPIO_MODE_OUTPUT;
  relay_cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&relay_cfg);
  gpio_set_level(RELAY_GPIO, RELAY_LOW);
}

bool relay_get_state() {
  return (gpio_get_level(RELAY_GPIO) == RELAY_HIGH);
}

void relay_on() {
  gpio_set_level(RELAY_GPIO, RELAY_HIGH);
  leds_set_level(LED_POWER, LED_HIGH);
}

void relay_off() {
  gpio_set_level(RELAY_GPIO, RELAY_LOW);
  leds_set_level(LED_POWER, LED_LOW);
}

void relay_toggle() {
  const bool state = relay_get_state();
  gpio_set_level(RELAY_GPIO, !state);
  leds_set_level(LED_POWER, (!state ? LED_HIGH : LED_LOW));
}
