#include "leds.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdbool.h>

#define LED_POWER_GPIO CONFIG_GPIO_POWER_LED
#define LED_TRANSMISSION_GPIO CONFIG_GPIO_TRANSMISSION_LED

#define LED_POWER_ACTIVE CONFIG_GPIO_POWER_LED_ACTIVE_STATE
#define LED_TRANSMISSION_ACTIVE CONFIG_GPIO_TRANSMISSION_LED_ACTIVE_STATE

#define LED_TRANSMISSION_BLINK_TIME_MS 50
#define LED_TRANSMISSION_BLINK_REQUEST 0
#define LED_TRANSMISSION_QUEUE_SIZE 5
#define LED_TRANSMISSION_TASK_STACK_DEPTH 2048
#define LED_TRANSMISSION_TASK_PRIORITY 1
typedef uint8_t led_transmission_request_t;
static QueueHandle_t transmission_led_queue;

void transmission_led_task(void *params) {
  led_transmission_request_t request;

  while (1) {
    xQueueReceive(transmission_led_queue, &request, portMAX_DELAY);

    // blink
    if (request == LED_TRANSMISSION_BLINK_REQUEST) {
      gpio_set_level(LED_TRANSMISSION_GPIO, LED_TRANSMISSION_ACTIVE);
      vTaskDelay(pdMS_TO_TICKS(LED_TRANSMISSION_BLINK_TIME_MS));
      gpio_set_level(LED_TRANSMISSION_GPIO, !LED_TRANSMISSION_ACTIVE);
    }
  }
}

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

  transmission_led_queue = xQueueCreate(LED_TRANSMISSION_QUEUE_SIZE,
                                        sizeof(led_transmission_request_t));
  xTaskCreate(transmission_led_task, "transmission_led_task",
              LED_TRANSMISSION_TASK_STACK_DEPTH, NULL,
              LED_TRANSMISSION_TASK_PRIORITY, NULL);
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

void leds_transmission_led_blink() {
  led_transmission_request_t request = LED_TRANSMISSION_BLINK_REQUEST;
  BaseType_t ret = xQueueSend(transmission_led_queue, &request, 0);

  if (ret != pdTRUE) {
    ESP_LOGW("Leds", "Failed to send LED transmission blink request to queue.");
  }
}
