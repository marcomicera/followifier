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
#include "util/conf.h"
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

    // Last two digits in board's TAG
    char boards_last_two_digits[4];
    snprintf(boards_last_two_digits, sizeof(boards_last_two_digits), " %02x", mac_address[5]);
    strncat(BOARD_TAG, boards_last_two_digits, sizeof(BOARD_TAG) - strlen(BOARD_TAG) - 1);
}

void flush();

void flushAsNewTask();

void store_message(Followifier__ESP32Message *data) {

    // Storing the message into the buffer
    messages = realloc(messages, sizeof(Followifier__ESP32Message *) * (items + 1));
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
    ESP_LOGI(BOARD_TAG, "Sending %d messages...", items);
    batch_length = followifier__batch__get_packed_size(&batch);
    buffer = malloc(batch_length + sizeof("\n\r\n\r"));
    followifier__batch__pack(&batch, buffer);
    memcpy(buffer + batch_length, "\n\r\n\r", sizeof("\n\r\n\r"));
    ESP_LOGI(BOARD_TAG, "Board's source MAC: %s", batch.boardmac);
    free(batch.boardmac);

    // Stopping the sniffer
    if (stop) {
        ESP_ERROR_CHECK(stop_sniffer());
    }

    // Turning on the Wi-Fi
    ESP_ERROR_CHECK(start_wifi());
}

void flush(void) {

    // Flush only when there at least one message to send
    if (items > 0) {

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
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        ESP_ERROR_CHECK_JUMP_LABEL (
                setsockopt(tcp_socket, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) >= 0,
                "setsockopt failed\n",
                reactivate_sniffer);

        ESP_ERROR_CHECK_JUMP_LABEL (
                setsockopt(tcp_socket, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) >= 0,
                "setsockopt failed\n",
                reactivate_sniffer);

        ESP_LOGI(BOARD_TAG, "Socket created, connecting to %s:%d...", SERVER_ADDRESS, SERVER_PORT);

        // Creating connection to the server
        ESP_ERROR_CHECK_JUMP_LABEL(!connect(tcp_socket, (struct sockaddr *) &server_address, sizeof(server_address)),
                                   "Error while connecting to the server: discarding local packets, re-enabling sniffing mode...",
                                   closing_socket);


        // Flushing the message buffer in slices of equal length.
        u_int32_t total_len = batch_length + sizeof(DELIMITER);
        u_int16_t needed_packets = (total_len / (FLUSH_MODULUS + 1)) + 1;
        u_int16_t offset = 0;
        u_int32_t bytes_to_send = 0;

        ESP_LOGI(BOARD_TAG, "Sending batch of %d bytes in %d packets.", total_len, needed_packets);
        for (int i = 0; i < needed_packets ; i++) {
            if (((total_len - offset) / FLUSH_MODULUS)) {
                bytes_to_send = FLUSH_MODULUS;
            }
            else {
                bytes_to_send = (total_len - offset) % FLUSH_MODULUS;
            }

            ESP_ERROR_CHECK_JUMP_LABEL(send(tcp_socket, (char *) buffer + offset, bytes_to_send, 0) >= 0, 
                                    "Error while sending batch: discarding local packets, re-enabling sniffing mode...",
                                    closing_socket);
            offset += bytes_to_send;
            ESP_LOGI(BOARD_TAG, "Sent #%d packet.", i + 1);
        }
    
        // Skipping until here in case connection towards the server was unsuccessful
        closing_socket:

        // Closing socket
        ESP_LOGI(BOARD_TAG, "Shutting down socket towards %s:%d...", SERVER_ADDRESS, SERVER_PORT);
        shutdown(tcp_socket, SHUT_RDWR);
        close(tcp_socket);
        ESP_LOGI(BOARD_TAG, "...socket towards %s:%d closed.", SERVER_ADDRESS, SERVER_PORT);

        // Skipping until here in case of error while creating a socket towards the server
        reactivate_sniffer:

        // Deleting local buffer elements
        free(buffer);
        for (int i = 0; i < items; i++) {
            free(messages[i]->frame_hash.data);
            free(messages[i]->metadata->devicemac);
            free(messages[i]->metadata->ssid);
            free(messages[i]->metadata);
            free(messages[i]);
        }
        items = 0;
    }

    // Turning the Wi-Fi off
    ESP_ERROR_CHECK(stop_wifi());

    // Re-activating the sniffer
    init_sniffer();

    // Re-activating the sniffer
    ESP_ERROR_CHECK(start_sniffer());
}
