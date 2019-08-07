/* cmd_sniffer example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_console.h"
#include "esp_app_trace.h"
#include "cmd_sniffer.h"
#include "sdkconfig.h"
// #include "hash.pb-c.h"

#define SNIFFER_DEFAULT_FILE_NAME "esp-sniffer"
#define SNIFFER_FILE_NAME_MAX_LEN CONFIG_SNIFFER_PCAP_FILE_NAME_MAX_LEN
#define SNIFFER_DEFAULT_CHANNEL (1)
#define SNIFFER_PAYLOAD_FCS_LEN (4)
#define SNIFFER_PROCESS_PACKET_TIMEOUT_MS (100)
#define SNIFFER_PROCESS_APPTRACE_TIMEOUT_US (100)
#define SNIFFER_APPTRACE_RETRY  (10)

static const char *SNIFFER_TAG = "followifier";
#define SNIFFER_CHECK(a, str, goto_tag, ...)                                              \
    do                                                                                    \
    {                                                                                     \
        if (!(a))                                                                         \
        {                                                                                 \
            ESP_LOGE(SNIFFER_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag;                                                                \
        }                                                                                 \
    } while (0)

typedef struct {
    char *filter_name;
    uint32_t filter_val;
} wlan_filter_table_t;

typedef struct {
    void *payload;
    uint32_t length;
    uint32_t seconds;
    uint32_t microseconds;
} sniffer_packet_info_t;

static sniffer_runtime_t snf_rt = {0};


static uint32_t hash_func(const char *str, uint32_t max_num) {
    uint32_t ret = 0;
    char *p = (char *) str;
    while (*p) {
        ret += *p;
        p++;
    }
    return ret % max_num;
}

static void wifi_sniffer_cb(void *recv_buf, wifi_promiscuous_pkt_type_t type) {
    sniffer_packet_info_t packet_info;
    wifi_promiscuous_pkt_t *sniffer = (wifi_promiscuous_pkt_t *) recv_buf;
    /* prepare packet_info */
    packet_info.seconds = sniffer->rx_ctrl.timestamp / 1000000U;
    packet_info.microseconds = sniffer->rx_ctrl.timestamp % 1000000U;
    packet_info.length = sniffer->rx_ctrl.sig_len;
    ESP_LOGI(SNIFFER_TAG, "mgmt packet: %s: ", sniffer->payload); // TODO print packet info here?
    /* For now, the sniffer only dumps the length of the MISC type frame */
    if (type != WIFI_PKT_MISC && !sniffer->rx_ctrl.rx_state) {
        packet_info.length -= SNIFFER_PAYLOAD_FCS_LEN;
        void *backup = malloc(packet_info.length);
        if (backup) {
            memcpy(backup, sniffer->payload, packet_info.length);
            packet_info.payload = backup;
            if (snf_rt.work_queue) {
                /* send packet_info */
                if (xQueueSend(snf_rt.work_queue, &packet_info, pdMS_TO_TICKS(SNIFFER_PROCESS_PACKET_TIMEOUT_MS)) !=
                    pdTRUE) {
                    ESP_LOGE(SNIFFER_TAG, "sniffer work queue full");
                }
            }
        } else {
            ESP_LOGE(SNIFFER_TAG, "No enough memory for promiscuous packet");
        }
    }
}

static void sniffer_task(void *parameters) {
    sniffer_packet_info_t packet_info;
    sniffer_runtime_t *sniffer = (sniffer_runtime_t *) parameters;

    while (sniffer->is_running) {
        /* receive packet info from queue */
        if (xQueueReceive(sniffer->work_queue, &packet_info, pdMS_TO_TICKS(SNIFFER_PROCESS_PACKET_TIMEOUT_MS)) !=
            pdTRUE) {
            continue;
        }
        free(packet_info.payload);
    }
    /* notify that sniffer task is over */
    xSemaphoreGive(sniffer->sem_task_over);
    vTaskDelete(NULL);
}

static esp_err_t sniffer_stop(sniffer_runtime_t *sniffer) {
    SNIFFER_CHECK(sniffer->is_running, "sniffer is already stopped", err);

    switch (sniffer->interf) {
        case SNIFFER_INTF_WLAN:
            /* Disable wifi promiscuous mode */
            SNIFFER_CHECK(esp_wifi_set_promiscuous(false) == ESP_OK, "stop wifi promiscuous failed", err);
            break;
        default:
            SNIFFER_CHECK(false, "unsupported interface", err);
            break;
    }
    ESP_LOGI(SNIFFER_TAG, "stop WiFi promiscuous ok");

    /* stop sniffer local task */
    sniffer->is_running = false;
    /* wait for task over */
    xSemaphoreTake(sniffer->sem_task_over, portMAX_DELAY);
    vSemaphoreDelete(sniffer->sem_task_over);
    sniffer->sem_task_over = NULL;
    /* make sure to free all resources in the left items */
    UBaseType_t left_items = uxQueueMessagesWaiting(sniffer->work_queue);
    sniffer_packet_info_t packet_info;
    while (left_items--) {
        xQueueReceive(sniffer->work_queue, &packet_info, pdMS_TO_TICKS(SNIFFER_PROCESS_PACKET_TIMEOUT_MS));
        free(packet_info.payload);
    }
    vQueueDelete(sniffer->work_queue);
    sniffer->work_queue = NULL;
    return ESP_OK;
    err:
    return ESP_FAIL;
}

#if CONFIG_SNIFFER_PCAP_DESTINATION_JTAG
static int trace_writefun(void *cookie, const char *buf, int len)
{
    return esp_apptrace_write(ESP_APPTRACE_DEST_TRAX, buf, len, SNIFFER_PROCESS_APPTRACE_TIMEOUT_US) == ESP_OK ? len : -1;
}

static int trace_closefun(void *cookie)
{
    return esp_apptrace_flush(ESP_APPTRACE_DEST_TRAX, ESP_APPTRACE_TMO_INFINITE) == ESP_OK ? 0 : -1;
}
#endif

esp_err_t sniffer_start(sniffer_runtime_t *sniffer) {
    wifi_promiscuous_filter_t wifi_filter;

    sniffer->is_running = true;
    sniffer->work_queue = xQueueCreate(CONFIG_SNIFFER_WORK_QUEUE_LEN, sizeof(sniffer_packet_info_t));
    SNIFFER_CHECK(sniffer->work_queue, "create work queue failed", err_queue);
    sniffer->sem_task_over = xSemaphoreCreateBinary();
    SNIFFER_CHECK(sniffer->sem_task_over, "create sem failed", err_sem);
    SNIFFER_CHECK(xTaskCreate(sniffer_task, "snifferT", CONFIG_SNIFFER_TASK_STACK_SIZE,
                              sniffer, CONFIG_SNIFFER_TASK_PRIORITY, &sniffer->task) == pdTRUE,
                  "create task failed", err_task);

    switch (sniffer->interf) {
        case SNIFFER_INTF_WLAN:
            /* Start WiFi Promicuous Mode */
            wifi_filter.filter_mask = sniffer->filter;
            esp_wifi_set_promiscuous_filter(&wifi_filter);
            esp_wifi_set_promiscuous_rx_cb(wifi_sniffer_cb);
            SNIFFER_CHECK(esp_wifi_set_promiscuous(true) == ESP_OK, "start wifi promiscuous failed", err_start);
            esp_wifi_set_channel(sniffer->channel, WIFI_SECOND_CHAN_NONE);
            ESP_LOGI(SNIFFER_TAG, "start WiFi promiscuous ok");
            break;
        default:
            break;
    }
    return ESP_OK;
    err_start:
    vTaskDelete(sniffer->task);
    sniffer->task = NULL;
    err_task:
    vSemaphoreDelete(sniffer->sem_task_over);
    sniffer->sem_task_over = NULL;
    err_sem:
    vQueueDelete(sniffer->work_queue);
    sniffer->work_queue = NULL;
    err_queue:
    sniffer->is_running = false;

    return ESP_FAIL;
}
