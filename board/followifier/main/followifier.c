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
#include "gen/message.pb-c.h"
#include "components/wifi.h"
#include "components/flusher.h"
#include "components/nvs.h"
#include "components/sync.h"
#include "util/misc.h"
#include "util/conf.h"

/**
 * Board event handler.
 * Manages the board's lifecycle and implement its state machine.
 */
esp_err_t board_event_handler(void *ctx, system_event_t *event) {

    switch (event->event_id) {

        case SYSTEM_EVENT_STA_START: // Board started

            // Connecting it to the Wi-Fi network
            ESP_LOGI(BOARD_TAG, "Board in station mode. Connecting it to the \"%s\" Wi-Fi network...", WIFI_SSID);
            ESP_ERROR_CHECK(esp_wifi_connect());
            ESP_LOGI(BOARD_TAG, "...board connected to the \"%s\" Wi-Fi network.", WIFI_SSID);
            break;

        case SYSTEM_EVENT_STA_GOT_IP: // Board got IP from connected AP

            // Printing it
            ESP_LOGI(BOARD_TAG, "Got IP: %s\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));

            // If time has not been set already
            if (!time_has_been_set()) {

                // Send a request to the SNTP server
                send_sntp_request();

                // Wait until time is set
                while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) {
                    vTaskDelay(2000 / portTICK_PERIOD_MS);
                }
            }

            // Board is ready to flush messages to the core server
            flush();

            break;

        case SYSTEM_EVENT_STA_DISCONNECTED: // Board got disconnected from AP

            ESP_LOGI(BOARD_TAG, "Board got disconnected from the \"%s\" Wi-Fi network.", WIFI_SSID);
            //TODO: Sometimes it would get stuck here for me. I've tried to fix it but I wasn't able to reproduce it anymore.
            if(!wifi_stopped){
                //Wifi wasn't voluntary stopped
                init_sniffer();
                start_sniffer();
            }
            break;

        // case SYSTEM_EVENT_STA_STOP: // Board stops
            // break;

        default:
            break;
    }

    return ESP_OK;
}

/**
 * Initializing board components
 */
void init_all() {

    // Initialize the flusher
    init_flusher();

    // Initialize the default NVS partition
    init_nvs();

    // Initialize board's Wi-Fi module
    init_wifi();

    // Initialize the sniffer
    init_sniffer();

    // Initialize SNTP module
    init_sntp();
}

/**
 * Entry point
 */
void app_main(void) {

    // Event loop handler
    ESP_ERROR_CHECK(esp_event_loop_init(board_event_handler, NULL)); // FIXME deprecated

    // Initialize board
    init_all();

    // Turn the Wi-Fi on
    ESP_ERROR_CHECK(start_wifi());
}
