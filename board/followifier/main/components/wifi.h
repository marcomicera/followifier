//
// Created by marcomicera on 9/12/19.
//

#ifndef FOLLOWIFIER_WIFI_H
#define FOLLOWIFIER_WIFI_H

#include <stdbool.h>

/**
 * Wi-Fi info
 */
/*
 * TODO
 *  Read "Establishing Wi-Fi or Ethernet Connection" section in
 *  esp-idf/examples/protocols/README.md for more information about this function.
 */
#define WIFI_SSID "accomazzi" // FIXME
#define WIFI_PASSWORD "WiFiAccomair2017"  // FIXME
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN

/**
 * Set to true upon receiving an IP address from the AP.
 */
extern bool hasGotIp;

/**
 * Initializes the Wi-Fi module.
 */
void init_wifi();

/**
 * Turns on the Wi-Fi module.
 * To be called after `init_wifi()`.
 *
 * @return
  *    - ESP_OK: succeed
  *    - ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
  *    - ESP_ERR_INVALID_ARG: invalid argument
  *    - ESP_ERR_NO_MEM: out of memory
  *    - ESP_ERR_WIFI_CONN: WiFi internal error, station or soft-AP control block wrong
  *    - ESP_FAIL: other WiFi internal errors
 */
esp_err_t start_wifi();

/**
 * Stops the Wi-Fi module.
 *
 * @return
 *    - ESP_OK: succeed
 *    - ESP_ERR_WIFI_NOT_INIT: WiFi is not initialized by esp_wifi_init
 */
esp_err_t stop_wifi();

#endif //FOLLOWIFIER_WIFI_H
