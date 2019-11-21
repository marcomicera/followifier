/* cmd_sniffer example — declarations of command r = {0};egistration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once

#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SNIFFER_DEFAULT_CHANNEL (1)

/**
 * When true, the board only prints out the RSSI of frames received by a specific device.
 * It does not flush any data towards the core server.
 * Used for debug purposes.
 */
#define DEBUG_ONE_DEVICE_TRACKING (0)

#ifdef DEBUG_ONE_DEVICE_TRACKING

/**
 * MAC address of the device to be tracked in the "one device tracking" debug mode.
 * Used only when `DEBUG_ONE_DEVICE_TRACKING` is true.
 */
#define DEBUG_TRACKED_DEVICE_MAC "01:23:45:67:89:ab"

/**
 * Minimum and maximum RSSI values measured in a measurement period, i.e., a time interval
 * during which the distance between the device and the board is constant.
 * These value are only displayed on standard output.
 */
extern signed min_rrsi_in_measure_period;
extern signed max_rrsi_in_measure_period;

#endif

/**
 * Board flush rate in seconds.
 */
#define FLUSH_RATE_IN_SECONDS (30)

extern const char *SNIFFER_TAG;

/**
 * @brief Supported Sniffer Interface
 *
 */
typedef enum {
    SNIFFER_INTF_WLAN = 0, /*!< WLAN interface */
} sniffer_intf_t;

/**
 * @brief WLAN Sniffer Filter
 *
 */
typedef enum {
    SNIFFER_WLAN_FILTER_MGMT = 0, /*!< MGMT */
    SNIFFER_WLAN_FILTER_CTRL,     /*!< CTRL */
    SNIFFER_WLAN_FILTER_DATA,     /*!< DATA */
    SNIFFER_WLAN_FILTER_MISC,     /*!< MISC */
    SNIFFER_WLAN_FILTER_MPDU,     /*!< MPDU */
    SNIFFER_WLAN_FILTER_AMPDU,    /*!< AMPDU */
    SNIFFER_WLAN_FILTER_MAX
} sniffer_wlan_filter_t;

typedef struct {
    bool is_running;
    sniffer_intf_t interf;
    uint32_t channel;
    uint32_t filter;
    TaskHandle_t task;
    QueueHandle_t work_queue;
    SemaphoreHandle_t sem_task_over;
} sniffer_runtime_t;

/**
 * Initializes and configures the sniffer component.
 */
void init_sniffer();

/**
 * Starts the sniffer component.
 *
 * @return `ESP_OK` in case of success, something else otherwise.
 */
esp_err_t start_sniffer();

/**
 * Stops the sniffer component.
 *
 * @return `ESP_OK` in case of success, something else otherwise.
 */
esp_err_t stop_sniffer(void);

#ifdef __cplusplus
}
#endif
