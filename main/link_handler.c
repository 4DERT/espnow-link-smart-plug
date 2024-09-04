#include "link_handler.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"

#include "button.h"
#include "leds.h"
#include "link.h"
#include "relay.h"

static void on_link_command(const char *cmd);
static char *on_link_status_message_create(void);

link_config_t link_config = {
    .type = 2,
    .commands = {"ON", "OFF", "GET_STATUS"},
    .status_fmt = "%s",
    .user_command_parser_cb = on_link_command,
    .user_status_msg_cb = on_link_status_message_create,
};

void lh_init() {
  // Check if button is pressed
  vTaskDelay(pdMS_TO_TICKS(10));
  bool is_force_pair_btn_pressed = button_is_pressed();

  link_start(is_force_pair_btn_pressed);
}

inline void lh_send_state() { link_send_status_msg(); }

static void on_link_command(const char *cmd) {
  if (strcmp(cmd, "ON") == 0) {
    relay_on();
  }

  else if (strcmp(cmd, "OFF") == 0) {
    relay_off();
  }

  link_send_status_msg();
}

static char *on_link_status_message_create(void) {
  leds_set_level(LED_TRANSMISSION, LED_HIGH);
  bool is_on = relay_get_state();
  char *status = link_generate_status_message(NULL, is_on ? "ON" : "OFF");
  leds_set_level(LED_TRANSMISSION, LED_LOW);
  return status;
}
