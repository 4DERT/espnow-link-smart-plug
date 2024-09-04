#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdbool.h>
#include "driver/gpio.h"

void button_init();
bool button_is_pressed();
void button_notify_ignore();

#endif // BUTTON_H_