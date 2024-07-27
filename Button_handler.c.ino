#include "button_handler.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define BUTTON_PIN GPIO_NUM_0
#define DEBOUNCE_DELAY_MS 50
#define LONG_PRESS_DURATION_MS 7000

static const char *TAG = "BUTTON";

static esp_timer_handle_t press_timer = NULL;
static bool is_pressed = false;

static void button_press_timer_callback(void* arg) {
    perform_ota_update();
}

void button_isr_handler(void* arg) {
    static uint32_t last_press_time = 0;
    uint32_t current_time = esp_timer_get_time();

    if (gpio_get_level(BUTTON_PIN) == 0) { // Button pressed
        if (!is_pressed) {
            is_pressed = true;
            esp_timer_create_args_t timer_args = {
                .callback = button_press_timer_callback,
                .name = "button_press_timer"
            };
            esp_timer_create(&timer_args, &press_timer);
            esp_timer_start_once(press_timer, LONG_PRESS_DURATION_MS * 1000);
        }
    } else { // Button released
        if (is_pressed) {
            is_pressed = false;
            esp_timer_stop(press_timer);
        }
    }
}

void setup_button() {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .pull_up_en = GPIO_PULLUP_ENABLE
    };
    gpio_config(&io_conf);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);
}
