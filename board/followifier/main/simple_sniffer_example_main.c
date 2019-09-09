/* Sniffer example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "tcpip_adapter.h"
#include "esp_console.h"
#include "esp_event.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "esp_wifi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "nvs_flash.h"
#include "sdmmc_cmd.h"
#include "cmd_decl.h"
#include "sdkconfig.h"
#include "message.pb-c.h"
#include "flusher.h"
#include "util.h"

#define SNIFFER_DEFAULT_CHANNEL (1)

typedef struct {
    void *payload;
    uint32_t length;
    uint32_t seconds;
    uint32_t microseconds;
} sniffer_packet_info_t;

static sniffer_runtime_t sniffer_runtime = {0};

/**
 * Board event handler
 */
static esp_err_t event_handler(void *ctx, system_event_t *event) {

    switch (event->event_id) {

        // Board started
        case SYSTEM_EVENT_STA_START:

            // Connecting it to the Wi-Fi network
            ESP_LOGI(TAG, "Board has started. Connecting it to %s...", WIFI_SSID);
            ESP_ERROR_CHECK(esp_wifi_connect());
            ESP_LOGI(TAG, "...board connected to %s.", WIFI_SSID);
            break;

        // Board got IP from connected AP
        case SYSTEM_EVENT_STA_GOT_IP:

            // Printing it
            ESP_LOGI(TAG, "Got IP: %s\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
            hasGotIp = true;
            break;

        // Board got disconnected from AP
        case SYSTEM_EVENT_STA_DISCONNECTED:

            hasGotIp = false;
            ESP_LOGI(TAG, "Board got disconnected. Connecting it again to %s...", WIFI_SSID);
            ESP_ERROR_CHECK(esp_wifi_connect());
            ESP_LOGI(TAG, "...board connected to %s.", WIFI_SSID);
            break;

        // Board stops
        case SYSTEM_EVENT_STA_STOP:

            hasGotIp = false;

            // Stop sniffing packets
            ESP_ERROR_CHECK(sniffer_stop(&sniffer_runtime));
            break;

        default:
            break;
    }

    return ESP_OK;
}

static void initialize_nvs(void) {

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

static void initialize_wifi(void) {

    hasGotIp = false;
    tcpip_adapter_init();

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

    wifi_config_t wifi_config = {
            .sta = {
                    .ssid = WIFI_SSID,
                    .password = WIFI_PASSWORD,
                    .scan_method = DEFAULT_SCAN_METHOD,
                    .sort_method = DEFAULT_SORT_METHOD,
                    .threshold.rssi = DEFAULT_RSSI,
                    .threshold.authmode = DEFAULT_AUTHMODE,
            },
    };
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

/**
 * Initializing board components
 */
void init() {

    // Event loop handler
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    // Initialize the default NVS partition
    initialize_nvs();

    // Initialize board's Wi-Fi module
    initialize_wifi();
}

/* Entry point */
void app_main(void) {

    // Initialize board
    init();

    // Start sniffing packets
    sniffer_runtime.interf = SNIFFER_INTF_WLAN;
    sniffer_runtime.channel = SNIFFER_DEFAULT_CHANNEL;
    sniffer_runtime.filter = WIFI_PROMIS_FILTER_MASK_MGMT;
    ESP_ERROR_CHECK(sniffer_start(&sniffer_runtime));
}
