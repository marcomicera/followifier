//
// Created by marcomicera on 8/9/19.
//

#include <stdlib.h>
#include <sys/socket.h>
#include <tcpip_adapter.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include "flusher.h"
#include "util.h"

// Buffer
uint8_t *buffer[FLUSH_THRESHOLD];
unsigned short items = 0;

void flush();

void flushAsNewTask();

void store_message(uint8_t *serialized_data, unsigned message_length, sniffer_runtime_t sniffer) {

    // Checking whether the board has an IP address or not
    if (hasGotIp) {

        // Better check it twice
        if (items == FLUSH_THRESHOLD) {
            flush(sniffer);
            ESP_LOGW(TAG, "Last packet has not been sent to the server.");
            return;
        }

        // Storing the message into the buffer
        buffer[items] = serialized_data;
        ++items;

        // Time to flush the message buffer
        if (items == FLUSH_THRESHOLD) {
            flush(sniffer);
        }
    }
}

void flushAsNewTask() {
    xTaskCreate(flush, "flusher", 4096, NULL, 5, NULL);
    vTaskDelete(NULL);
}

/**
 *
 * @param sniffer sniffer to be temporarily interrupted.
 */
void flush(sniffer_runtime_t sniffer) {

    // Printing info
    ESP_LOGI(TAG, "Time to flush the message buffer. Sending %d messages...", FLUSH_THRESHOLD);
    for (unsigned short i = 0; i < FLUSH_THRESHOLD; ++i) {
        ESP_LOGI(TAG, "Message %d: length %d", i, sizeof(buffer[i]));
    }

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
    MUST_NOT_HAPPEN((tcp_socket = socket(addr_family, SOCK_STREAM, ip_protocol)) < 0,
                    "socket() error");
    ESP_LOGI(TAG, "Socket created, connecting to %s:%d...", SERVER_ADDRESS, SERVER_PORT);

    // Temporarily disable promiscuous mode
    if (sniffer.is_running) {
        ESP_ERROR_CHECK(sniffer_stop(&sniffer));
    }

    // Creating connection to the server
    MUST_NOT_HAPPEN(connect(tcp_socket, (struct sockaddr *) &server_address, sizeof(server_address)) != 0,
                    "Error while connecting to the server");

    // Flushing the message buffer
    for (unsigned short i = 0; i < FLUSH_THRESHOLD; ++i) {
        MUST_NOT_HAPPEN(send(tcp_socket, (void *) &buffer[i], sizeof(buffer[i]), 0) < 0,
                        "Error while sending message number %hu", i);
        free(buffer[i]);
    }
    items = 0;

    // Re-enable promiscuous mode
    if (!sniffer.is_running) {
        ESP_ERROR_CHECK(sniffer_start(&sniffer));
    }

    // Closing socket
    ESP_LOGI(TAG, "Shutting down socket towards %s:%d...", SERVER_ADDRESS, SERVER_PORT);
    shutdown(tcp_socket, 0);
    close(tcp_socket);
    ESP_LOGI(TAG, "...socket towards %s:%d closed.", SERVER_ADDRESS, SERVER_PORT);
}
