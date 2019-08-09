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

esp_err_t sniffer_start(sniffer_runtime_t*);

#ifdef __cplusplus
}
#endif
