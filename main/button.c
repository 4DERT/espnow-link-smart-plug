#include "button.h"

#include "FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
static TaskHandle_t btn_task_handle;

void button_isr_handler(void *arg) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  vTaskNotifyGiveFromISR(btn_task_handle, &xHigherPriorityTaskWoken);

  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();
  }
}

static void button_task(void *params) {

   uint32_t reg = 0;

    ESP_LOGI("BTN", "TASK START");

  while (1) {
    ulTaskNotifyTake(true, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(BUTTON_BOUNCE_TIME_MS));

    if(button_is_pressed()) {
        ESP_LOGI("BTN", "Pressed!");
    }
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
  button_cfg.intr_type = (BUTTON_ACTIVE_STATE) ? GPIO_INTR_POSEDGE : GPIO_INTR_NEGEDGE;
  gpio_config(&button_cfg);

  xTaskCreate(button_task, "button_task", BUTTON_TASK_STACK_DEPTH, NULL,
              BUTTON_TASK_PRIORITY, &btn_task_handle);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

  
}

bool button_is_pressed() {
  return gpio_get_level(BUTTON_GPIO) == BUTTON_ACTIVE_STATE;
}