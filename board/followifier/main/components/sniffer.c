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
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_console.h"
#include "sniffer.h"
#include "sdkconfig.h"
#include "message.pb-c.h"
#include "flusher.h"
#include "util/misc.h"

#define SNIFFER_DEFAULT_CHANNEL (1)
#define SNIFFER_PAYLOAD_FCS_LEN (4) // TODO use this
#define SNIFFER_PROCESS_PACKET_TIMEOUT_MS (100)

#define PROBE_REQUEST    0x0040
#define PROBE_RESPONSE    0x0050
#define SUBTYPE_MASK 0x00F0

const char *SNIFFER_TAG = "followifier";

typedef struct {
    unsigned frame_ctrl:16;
    unsigned duration_id:16;
    uint8_t addr1[6]; /* receiver address */
    uint8_t addr2[6]; /* sender address */
    uint8_t addr3[6]; /* filtering address */
    unsigned sequence_ctrl:16;
    uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

typedef struct {
    void *payload;
    uint32_t length;
    uint32_t seconds;
    uint32_t microseconds;
} sniffer_packet_info_t;

static sniffer_runtime_t snf_rt = {0};

void init_sniffer(void) {
    snf_rt.interf = SNIFFER_INTF_WLAN;
    snf_rt.channel = SNIFFER_DEFAULT_CHANNEL;
    snf_rt.filter = WIFI_PROMIS_FILTER_MASK_MGMT;
}

void sniffer_packet_handler(void *, wifi_promiscuous_pkt_type_t);

static void sniffer_task(void *);

esp_err_t start_sniffer(void) {

    esp_log_level_set(SNIFFER_TAG, ESP_LOG_VERBOSE);
    wifi_promiscuous_filter_t wifi_filter;
    snf_rt.is_running = true;
    ESP_LOGI(SNIFFER_TAG, "Sniffer started.");
    snf_rt.work_queue = xQueueCreate(CONFIG_SNIFFER_WORK_QUEUE_LEN, sizeof(sniffer_packet_info_t));
    ESP_ERROR_CHECK_JUMP_LABEL(snf_rt.work_queue, "create work queue failed", err_queue);
    snf_rt.sem_task_over = xSemaphoreCreateBinary();
    ESP_ERROR_CHECK_JUMP_LABEL(snf_rt.sem_task_over, "create sem failed", err_sem);
    ESP_ERROR_CHECK_JUMP_LABEL(xTaskCreate(sniffer_task, "snifferT", CONFIG_SNIFFER_TASK_STACK_SIZE,
                                           &snf_rt, CONFIG_SNIFFER_TASK_PRIORITY, &snf_rt.task) == pdTRUE,
                               "create task failed", err_task);

    switch (snf_rt.interf) {

        case SNIFFER_INTF_WLAN:
            /* Start WiFi Promiscuous Mode */
            wifi_filter.filter_mask = snf_rt.filter;
            esp_wifi_set_promiscuous_filter(&wifi_filter);
            esp_wifi_set_promiscuous_rx_cb(sniffer_packet_handler);
            ESP_ERROR_CHECK_JUMP_LABEL(esp_wifi_set_promiscuous(true) == ESP_OK, "start wifi promiscuous failed",
                                       err_start);
            esp_wifi_set_channel(snf_rt.channel, WIFI_SECOND_CHAN_NONE);
            ESP_LOGI(SNIFFER_TAG, "Wi-Fi promiscuous mode started.");
            break;

        default:
            break;
    }
    return ESP_OK;
    err_start:
    vTaskDelete(snf_rt.task);
    snf_rt.task = NULL;
    err_task:
    vSemaphoreDelete(snf_rt.sem_task_over);
    snf_rt.sem_task_over = NULL;
    err_sem:
    vQueueDelete(snf_rt.work_queue);
    snf_rt.work_queue = NULL;
    err_queue:
    snf_rt.is_running = false;
    ESP_LOGI(SNIFFER_TAG, "Sniffer stopped.");

    return ESP_FAIL;
}

/**
 * Masks the frame control to only examine Subtype fields, bits [12:9].
 *
 * @param header    packet header to be examined.
 * @return          a string representation of the packet header type.
 */
const char *packet_subtype2str(wifi_ieee80211_mac_hdr_t *header) {
    switch (header->frame_ctrl & SUBTYPE_MASK) {
        case PROBE_REQUEST:
            return "PROBE_REQUEST";
        case PROBE_RESPONSE:
            return "PROBE_RESPONSE";
        default:
            return "NOTIMPL";
    }
}

int sniffer_is_probe(unsigned short fctl) {
    return ((fctl & SUBTYPE_MASK) == PROBE_REQUEST || (fctl & SUBTYPE_MASK) == PROBE_RESPONSE);
}

/**
 * Handling PROBE_REQUEST Wi-Fi management packets.
 *
 * @param buff
 * @param type
 */
void sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type) {

    // Not necessary after calling `esp_wifi_set_promiscuous_filter()`
    if (type != WIFI_PKT_MGMT)
        return;

    // Converts buffer to the correct struct definition
    const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *) buff;

    // Extracts Payload and Header with our user-defined representation
    const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *) ppkt->payload;
    const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

    ESP_LOGD(SNIFFER_TAG, "Masked:unmasked frame control of packet has value %04x:%04x\n",
             (hdr->frame_ctrl & SUBTYPE_MASK), hdr->frame_ctrl);

    if (sniffer_is_probe((unsigned short) hdr->frame_ctrl)) {

        unsigned long hash_value = hash((unsigned char *) ppkt);

        ESP_LOGI(SNIFFER_TAG, "PACKET TYPE=%s | CHAN=%02d | RSSI=%02d \n"
                              "TransmADDR=%02x:%02x:%02x:%02x:%02x:%02x |"
                              " BSSID=%02x:%02x:%02x:%02x:%02x:%02x \n"
                              "TIMESTAMP=%10d | hash=%lu | RSSI=%d\n",
                 packet_subtype2str((wifi_ieee80211_mac_hdr_t *) hdr),
                 ppkt->rx_ctrl.channel,
                 ppkt->rx_ctrl.rssi,

        // Source MAC address
                 hdr->addr2[0], hdr->addr2[1], hdr->addr2[2],
                 hdr->addr2[3], hdr->addr2[4], hdr->addr2[5],

        // BSSID
                 hdr->addr3[0], hdr->addr3[1], hdr->addr3[2],
                 hdr->addr3[3], hdr->addr3[4], hdr->addr3[5],

        // Other info
                 ppkt->rx_ctrl.timestamp,
                 hash_value,
                 ppkt->rx_ctrl.rssi
        );

        Followifier__ESP32Message message = FOLLOWIFIER__ESP32_MESSAGE__INIT;
        uint8_t *serialized_data;      // Buffer to store serialized data
        unsigned message_length;    // Length of serialized data

        message.frame_hash = hash_value;
        message.mac = malloc(sizeof(char) * (3 + 1));
        sprintf(message.mac, "mac"); // TODO MAC
        message.rsi = ppkt->rx_ctrl.rssi;
        message.ssid = malloc(sizeof(char) * (4 + 1));
        sprintf(message.ssid, "ssid"); // TODO SSID
        message.timestamp = ppkt->rx_ctrl.timestamp;

        message_length = followifier__esp32_message__get_packed_size(&message);
        serialized_data = malloc(message_length);
        followifier__esp32_message__pack(&message, serialized_data);

        // Store this message
        store_message(serialized_data, message_length, snf_rt);
    }
}

void sniffer_task(void *parameters) {
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

esp_err_t stop_sniffer(void) {
    ESP_ERROR_CHECK_JUMP_LABEL(snf_rt.is_running, "sniffer is already stopped", err);

    switch (snf_rt.interf) {
        case SNIFFER_INTF_WLAN:
            /* Disable wifi promiscuous mode */
            ESP_ERROR_CHECK_JUMP_LABEL(esp_wifi_set_promiscuous(false) == ESP_OK, "stop wifi promiscuous failed", err);
            break;
        default:
            ESP_ERROR_CHECK_JUMP_LABEL(false, "unsupported interface", err);
            break;
    }
    ESP_LOGI(SNIFFER_TAG, "stop WiFi promiscuous ok");

    /* stop sniffer local task */
    snf_rt.is_running = false;
    ESP_LOGI(SNIFFER_TAG, "Sniffer stopped.");
    /* wait for task over */
    xSemaphoreTake(snf_rt.sem_task_over, portMAX_DELAY);
    vSemaphoreDelete(snf_rt.sem_task_over);
    snf_rt.sem_task_over = NULL;
    /* make sure to free all resources in the left items */
    UBaseType_t left_items = uxQueueMessagesWaiting(snf_rt.work_queue);
    sniffer_packet_info_t packet_info;
    while (left_items--) {
        xQueueReceive(snf_rt.work_queue, &packet_info, pdMS_TO_TICKS(SNIFFER_PROCESS_PACKET_TIMEOUT_MS));
        free(packet_info.payload);
    }
    vQueueDelete(snf_rt.work_queue);
    snf_rt.work_queue = NULL;
    return ESP_OK;
    err:
    return ESP_FAIL;
}
