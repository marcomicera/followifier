//
// Created by marcomicera on 8/9/19.
//

#include <stdlib.h>
#include <sys/socket.h>
#include <tcpip_adapter.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include "gen/message.pb-c.h"
#include "flusher.h"
#include "util/misc.h"
#include "wifi.h"

// Buffer
static Followifier__ESP32Message **messages;
uint8_t *buffer;
unsigned int batch_length = 0;
unsigned short items = 0;

// MAC address
uint8_t mac_address[6] = {0};

void init_flusher() {

    // Retrieving the Wi-Fi station MAC address
    // https://github.com/espressif/esp-idf/tree/master/examples/system/base_mac_address
    ESP_ERROR_CHECK(esp_efuse_mac_get_default(mac_address));
    ESP_ERROR_CHECK(esp_base_mac_addr_set(mac_address));
    ESP_ERROR_CHECK(esp_read_mac(mac_address, ESP_MAC_WIFI_STA));
    ESP_LOGI("Wi-Fi station MAC address", "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_address[0], mac_address[1], mac_address[2],
             mac_address[3], mac_address[4], mac_address[5]);
}

void flush();

void flushAsNewTask();

void store_message(Followifier__ESP32Message *data) {

    // Storing the message into the buffer
    messages = realloc(messages, sizeof(Followifier__ESP32Message *)*(items+1));
    messages[items] = malloc(sizeof(Followifier__ESP32Message));
    memcpy(messages[items], data, sizeof(Followifier__ESP32Message));
    ++items;
}

void flushAsNewTask() {
    xTaskCreate(flush, "flusher", 4096, NULL, 5, NULL);
    vTaskDelete(NULL);
}

/**
 *
 * @param sniffer sniffer to be temporarily interrupted.
 */
void prepare_to_flush(bool stop) {

    // Preparing a batch containing a set of messages
    Followifier__Batch batch = FOLLOWIFIER__BATCH__INIT;
    batch.boardmac = malloc(18);
    snprintf(batch.boardmac, 18, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
             mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
    batch.messages = messages;
    batch.n_messages = items;

    // Printing info
    ESP_LOGI(TAG, "Time to flush the message buffer. Sending %d messages...", items);
    batch_length = followifier__batch__get_packed_size(&batch);
    buffer = malloc(batch_length);
    followifier__batch__pack(&batch, buffer);
    ESP_LOGI(TAG, "Board's source MAC: %s", batch.boardmac);

    // Stopping the sniffer
    if (stop) {
        ESP_ERROR_CHECK(stop_sniffer());
    }

    // Turning on the Wi-Fi
    ESP_ERROR_CHECK(start_wifi());
}

void flush(void) {

    // Server info
    int tcp_socket;
    struct sockaddr_in server_address;
    char addr_str[128];
    int ip_protocol = IPPROTO_IP; // `IPPROTO_IPV6` for IPv6
    int addr_family = AF_INET; // `AF_INET6` for IPv6
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_ntoa_r(server_address.sin_addr, addr_str, sizeof(addr_str) - 1);

    // Creating the socket
    ESP_ERROR_CHECK_JUMP_LABEL((tcp_socket = socket(addr_family, SOCK_STREAM, ip_protocol)) >= 0,
                               "socket() error: discarding local packets, re-enabling sniffing mode...",
                               reactivate_sniffer);
    ESP_LOGI(TAG, "Socket created, connecting to %s:%d...", SERVER_ADDRESS, SERVER_PORT);

    // Creating connection to the server
    ESP_ERROR_CHECK_JUMP_LABEL(!connect(tcp_socket, (struct sockaddr *) &server_address, sizeof(server_address)),
                               "Error while connecting to the server: discarding local packets, re-enabling sniffing mode...",
                               closing_socket);

    // Flushing the message buffer
    ESP_LOGI(TAG, "Sending batch");
    ESP_ERROR_CHECK_JUMP_LABEL(send(tcp_socket, (char *) buffer, batch_length, 0) >= 0,
                               "Error while sending batch: discarding local packets, re-enabling sniffing mode...",
                               closing_socket);
    ESP_LOGI(TAG, "Sending delimiter...");
    ESP_ERROR_CHECK_JUMP_LABEL(send(tcp_socket, "\n\r\n\r", sizeof("\n\r\n\r"), 0) >= 0,
                               "Error while sending delimiter: discarding local packets, re-enabling sniffing mode...",
                               closing_socket);

    // Skipping until here in case connection towards the server was unsuccessful
    closing_socket:

    // Closing socket
    ESP_LOGI(TAG, "Shutting down socket towards %s:%d...", SERVER_ADDRESS, SERVER_PORT);
    shutdown(tcp_socket, 0);
    close(tcp_socket);
    ESP_LOGI(TAG, "...socket towards %s:%d closed.", SERVER_ADDRESS, SERVER_PORT);

    // Skipping until here in case of error while creating a socket towards the server
    reactivate_sniffer:

    // Deleting local buffer elements
    free(buffer);
    for(int i=0; i<items; i++){
        free(messages[i]);
    }
    items = 0;

    // Turning the Wi-Fi off
    ESP_ERROR_CHECK(stop_wifi());

    // Re-activating the sniffer
    init_sniffer();

    // Re-activating the sniffer
    ESP_ERROR_CHECK(start_sniffer());
}
