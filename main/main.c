// sdkconfig
#include "sdkconfig.h"

// FreeRTOS
#include "FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portmacro.h"

// ESP api
#include "driver/uart.h"

// Project
#include "button.h"
#include "leds.h"
#include "link_handler.h"
#include "nvs_manager.h"
#include "relay.h"

void app_main() {
  nvsm_init();
  uart_set_baudrate(UART_NUM_0, 115200);

  leds_init();
  relay_init();
  button_init();

  lh_init();

  while (1) {
    vTaskDelay(10);
  }
}
