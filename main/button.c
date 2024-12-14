#include "button.h"

#include "FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "esp_log.h"

#include "link_handler.h"
#include "relay.h"

#define BUTTON_GPIO CONFIG_GPIO_BUTTON
#define BUTTON_ACTIVE_STATE CONFIG_GPIO_BUTTON_ACTIVE_STATE
#define BUTTON_DEBOUNCE_TIME_MS CONFIG_GPIO_BUTTON_BOUNCE_TIME_MS

static TaskHandle_t btn_task_handle;
static TimerHandle_t button_timer = NULL;

void button_isr_handler(void *arg) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  if(button_timer != NULL) {
    xTimerStartFromISR(button_timer, &xHigherPriorityTaskWoken);
  }

  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();
  }

}

void button_timer_callback(TimerHandle_t timer) {
  bool is_pressed = button_is_pressed();

  if(is_pressed) {
    relay_toggle();
    lh_send_state();
  }
}

// public
void button_init() {
  gpio_config_t button_cfg;
  button_cfg.pin_bit_mask = (1UL << BUTTON_GPIO);
  button_cfg.mode = GPIO_MODE_INPUT;
  button_cfg.pull_up_en =
      (BUTTON_ACTIVE_STATE) ? GPIO_PULLUP_DISABLE : GPIO_PULLUP_ENABLE;
  button_cfg.pull_down_en =
      (BUTTON_ACTIVE_STATE) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
  button_cfg.intr_type =
      (BUTTON_ACTIVE_STATE) ? GPIO_INTR_POSEDGE : GPIO_INTR_NEGEDGE;
  gpio_config(&button_cfg);

  // Create the debounce timer
  button_timer = xTimerCreate("button_timer",
                                pdMS_TO_TICKS(BUTTON_DEBOUNCE_TIME_MS),
                                pdFALSE, // One-shot timer
                                NULL, button_timer_callback);

  if (button_timer == NULL) {
      ESP_LOGE("button", "Failed to create debounce timer");
      return;
  }

  gpio_install_isr_service(0);
  gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);
}

bool button_is_pressed() {
  return gpio_get_level(BUTTON_GPIO) == BUTTON_ACTIVE_STATE;
}

