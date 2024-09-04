#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdbool.h>
#include "driver/gpio.h"

#define BUTTON_GPIO CONFIG_GPIO_BUTTON
#define BUTTON_ACTIVE_STATE CONFIG_GPIO_BUTTON_ACTIVE_STATE
#define BUTTON_BOUNCE_TIME_MS CONFIG_GPIO_BUTTON_BOUNCE_TIME_MS
#define BUTTON_TASK_STACK_DEPTH 2048
#define BUTTON_TASK_PRIORITY 1

void button_init();
bool button_is_pressed();

#endif // BUTTON_H_