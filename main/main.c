#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// sdkconfig
#include "sdkconfig.h"

// FreeRTOS
#include "FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portmacro.h"

// ESP api
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs.h"
#include "nvs_flash.h"

// Project
#include "button.h"
#include "leds.h"

#define TAG "MAIN"

void init_nvs(void);

void app_main() {
  init_nvs();
  uart_set_baudrate(UART_NUM_0, 115200);

  leds_init();
  button_init();

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void init_nvs(void) {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
}

