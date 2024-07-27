#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "button_handler.h"
#include "ota_update.h"

void ota_task(void* pvParameter) {
    while (true) {
        // Periodic checks or other OTA related tasks
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void button_task(void* pvParameter) {
    while (true) {
        // Button handling logic
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main() {
    setup_button(); // Initialize button handling
    xTaskCreate(ota_task, "ota_task", 4096, NULL, 5, NULL);
    xTaskCreate(button_task, "button_task", 2048, NULL, 5, NULL);
}
