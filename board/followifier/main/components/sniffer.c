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
#include <pthread.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_console.h"
#include "sniffer.h"
#include "sdkconfig.h"
#include "gen/message.pb-c.h"
#include "flusher.h"
#include "util/misc.h"
#include "util/conf.h"
#include "wifi.h"

#define SNIFFER_DEFAULT_CHANNEL (1)
#define SNIFFER_PROCESS_PACKET_TIMEOUT_MS (100)

#define PROBE_REQUEST    0x0040
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

static void *sniffer_timer(void *);

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
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, sniffer_timer, NULL); // starting flush timer

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
        default:
            return "NOTIMPL";
    }
}

int sniffer_is_probe(unsigned short fctl) {
    return ((fctl & SUBTYPE_MASK) == PROBE_REQUEST);
}

void hexDump(char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char *) addr;

    // Output description if given.
    if (desc != NULL)
        printf("%s:\n", desc);

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0)
                printf("  %s\n", buff);

            // Output the offset.
            printf("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf(" %02x", pc[i]);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = pc[i];
        }

        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }

    // And print the final ASCII bit.
    printf("  %s\n", buff);
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

        unsigned char hash_value[33];
        hash(ppkt, hash_value);
        hash_value[32] = '\0';

        time_t now = 0; // wait for time to be set
        struct tm timeinfo = {0};
        time(&now);
        localtime_r(&now, &timeinfo);

        ESP_LOGI(SNIFFER_TAG, "PACKET TYPE=%s | CHAN=%02d\n"
                              "TransmADDR=%02x:%02x:%02x:%02x:%02x:%02x | BSSID=%02x:%02x:%02x:%02x:%02x:%02x\n"
                              "HASH=%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n"
                              "TIMESTAMP=%lu | RSSI=%d",
                 packet_subtype2str((wifi_ieee80211_mac_hdr_t *) hdr),
                 ppkt->rx_ctrl.channel,
                 hdr->addr2[0], hdr->addr2[1], hdr->addr2[2], // Source MAC address
                 hdr->addr2[3], hdr->addr2[4], hdr->addr2[5],
                 hdr->addr3[0], hdr->addr3[1], hdr->addr3[2], // BSSID
                 hdr->addr3[3], hdr->addr3[4], hdr->addr3[5],
                 hash_value[0], hash_value[1], hash_value[2], // frame hash
                 hash_value[3], hash_value[4], hash_value[5],
                 hash_value[6], hash_value[7], hash_value[8],
                 hash_value[9], hash_value[10], hash_value[11],
                 hash_value[12], hash_value[13], hash_value[14],
                 hash_value[15], hash_value[16], hash_value[17],
                 hash_value[18], hash_value[19], hash_value[20],
                 hash_value[21], hash_value[22], hash_value[23],
                 hash_value[24], hash_value[25], hash_value[26],
                 hash_value[27], hash_value[28], hash_value[29],
                 hash_value[30], hash_value[31],
                 now,
                 ppkt->rx_ctrl.rssi
        );
        // hexDump(NULL, ppkt->payload, ppkt->rx_ctrl.sig_len);
        printf("\n");
        Followifier__ESP32Message message = FOLLOWIFIER__ESP32_MESSAGE__INIT;
        Followifier__ESP32Metadata metadata = FOLLOWIFIER__ESP32_METADATA__INIT;

        // Forming the MAC source address string
        char apMacString[18];
        snprintf(apMacString, sizeof(apMacString), "%02x:%02x:%02x:%02x:%02x:%02x",
                 hdr->addr2[0], hdr->addr2[1], hdr->addr2[2], hdr->addr2[3], hdr->addr2[4], hdr->addr2[5]);

        message.frame_hash.len = sizeof(hash_value);
        message.frame_hash.data = (uint8_t *) malloc(sizeof(hash_value));
        for (unsigned long i = 0; i < sizeof(hash_value); ++i) {
            message.frame_hash.data[i] = (uint8_t) hash_value[i];
        }
        metadata.apmac = malloc(sizeof(apMacString));
        sprintf(metadata.apmac, "%s", apMacString);
        metadata.rsi = ppkt->rx_ctrl.rssi;
        metadata.ssid = malloc(sizeof(WIFI_SSID));
        sprintf(metadata.ssid, "%s", WIFI_SSID);
        metadata.timestamp = now;

        message.metadata = malloc(sizeof(metadata));
        memcpy(message.metadata, &metadata, sizeof(metadata));

        // Store this message
        store_message(&message);
    }
}

void *sniffer_timer(void *args) {
    ESP_LOGI(TAG, "Flush timer started.");
    vTaskDelay(portTICK_PERIOD_MS * FLUSH_RATE_IN_SECONDS * 10); // in deci-seconds (0.1 seconds)
    ESP_LOGI(TAG, "Flush timer expired (%d seconds): time to flush the batch.", FLUSH_RATE_IN_SECONDS);
    prepare_to_flush(true);
    return NULL;
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
    ESP_ERROR_CHECK_JUMP_LABEL(snf_rt.is_running, "Sniffer is already stopped.", err);

    switch (snf_rt.interf) {
        case SNIFFER_INTF_WLAN:
            /* Disable wifi promiscuous mode */
            ESP_ERROR_CHECK_JUMP_LABEL(esp_wifi_set_promiscuous(false) == ESP_OK, "stop wifi promiscuous failed", err);
            break;
        default:
            ESP_ERROR_CHECK_JUMP_LABEL(false, "unsupported interface", err);
            break;
    }
    ESP_LOGI(SNIFFER_TAG, "Wi-Fi promiscuous mode stopped.");

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
