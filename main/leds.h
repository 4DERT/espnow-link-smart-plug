#ifndef LEDS_H_
#define LEDS_H_

#include "driver/gpio.h"
#include <stdbool.h>

typedef enum { LED_POWER, LED_TRANSMISSION } led_t;

typedef enum {
  LED_HIGH,
  LED_LOW,
} led_level_e;

void leds_init();
void leds_set_level(led_t led, bool level);

#endif // LEDS_H_