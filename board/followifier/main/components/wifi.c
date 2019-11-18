//
// Created by marcomicera on 9/12/19.
//

#include <tcpip_adapter.h>
#include <esp_wifi.h>
#include "util/conf.h"
#include "wifi.h"

bool hasGotIp = false;

/**
 * Initializes the Wi-Fi module.
 */
void init_wifi(void) {

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
}

esp_err_t start_wifi(void) {
    return esp_wifi_start();
}

esp_err_t stop_wifi(void) {
    return esp_wifi_stop();
}