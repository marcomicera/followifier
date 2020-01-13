#include <stdlib.h>
#include <tcpip_adapter.h>
#include <esp_log.h>
#include "util/misc.h"
#include "sniffer.h"

char* WATCHDOG_TAG = "watchdog";

void watchdog(void *pvParameters) {

    // Waiting for the whole system to initialize
    const signed short initial_seconds_sleep_time = 30;
    vTaskDelay((initial_seconds_sleep_time * 1000) / portTICK_PERIOD_MS);

    const signed short seconds_sleep_time = 30;

    ESP_LOGI(WATCHDOG_TAG, "Watchdog started.");

    for (;;) {

        uint64_t TMP_RECEIVED_WIFI_PACKETS = RECEIVED_WIFI_PACKETS;
        ESP_LOGI(WATCHDOG_TAG, "Watchdog received %llu Wi-Fi packets. Going to sleep...",
                 TMP_RECEIVED_WIFI_PACKETS);

        vTaskDelay((seconds_sleep_time * 1000) / portTICK_PERIOD_MS);

        // Main thread is dead
        if (TMP_RECEIVED_WIFI_PACKETS == RECEIVED_WIFI_PACKETS) {
            ESP_LOGE(WATCHDOG_TAG, "Main thread is dead: from %llu packets to %llu received in %u seconds.",
                     TMP_RECEIVED_WIFI_PACKETS, RECEIVED_WIFI_PACKETS, seconds_sleep_time);
            ESP_LOGE(WATCHDOG_TAG, "Restarting main thread...");
            esp_restart();
        } else {
            ESP_LOGI(WATCHDOG_TAG, "Main thread is not dead: from %llu packets to %llu received in %u seconds.",
                     TMP_RECEIVED_WIFI_PACKETS, RECEIVED_WIFI_PACKETS, seconds_sleep_time);
            // RECEIVED_WIFI_PACKETS = 0;
        }
    }
}

void start_watchdog(void) {

    TaskHandle_t xHandle = NULL;

    // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
    // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
    // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
    // the new task attempts to access it.
    xTaskCreate(watchdog, "watchdog", CONFIG_SNIFFER_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle);
    ESP_LOGI(BOARD_TAG, "Watchdog's task has been created.");
    configASSERT(xHandle);

//    // Use the handle to delete the task.
//    if (xHandle != NULL) {
//        vTaskDelete(xHandle);
//    }
}
