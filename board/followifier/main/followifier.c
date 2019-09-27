/* Sniffer example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include "tcpip_adapter.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "message.pb-c.h"
#include "components/wifi.h"
#include "components/flusher.h"
#include "components/nvs.h"
#include "util/misc.h"

/**
 * Board event handler.
 * Manages the board's lifecycle and implement its state machine.
 */
esp_err_t board_event_handler(void *ctx, system_event_t *event) {

    switch (event->event_id) {

        // Board started
        case SYSTEM_EVENT_STA_START:

            // Connecting it to the Wi-Fi network
            ESP_LOGI(TAG, "Board in station mode. Connecting it to the \"%s\" Wi-Fi network...", WIFI_SSID);
            ESP_ERROR_CHECK(esp_wifi_connect());
            ESP_LOGI(TAG, "...board connected to the \"%s\" Wi-Fi network.", WIFI_SSID);
            break;

            // Board got IP from connected AP
        case SYSTEM_EVENT_STA_GOT_IP:

            // Printing it
            ESP_LOGI(TAG, "Got IP: %s\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            hasGotIp = true;

            // Board is ready to flush messages to the core server
            flush();

            break;

            // Board got disconnected from AP
        case SYSTEM_EVENT_STA_DISCONNECTED:

            hasGotIp = false;
            ESP_LOGI(TAG, "Board got disconnected from the \"%s\" Wi-Fi network.", WIFI_SSID);
            break;

            // Board stops
        case SYSTEM_EVENT_STA_STOP:

            hasGotIp = false;
            break;

        default:
            break;
    }

    return ESP_OK;
}

/**
 * Initializing board components
 */
void init_all() {

    // Event loop handler
    ESP_ERROR_CHECK(esp_event_loop_init(board_event_handler, NULL)); // FIXME deprecated

    // Initialize the flusher
    init_flusher();

    // Initialize the default NVS partition
    init_nvs();

    // Initialize board's Wi-Fi module
    init_wifi();

    // Initialize the sniffer
    init_sniffer();
}

/**
 * Entry point
 */
void app_main(void) {

    // Initialize board
    init_all();

    // Start capturing packets
    ESP_ERROR_CHECK(start_sniffer());
}
