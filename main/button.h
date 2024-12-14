#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdbool.h>
#include "driver/gpio.h"

void button_init();
bool button_is_pressed();

#endif // BUTTON_H_