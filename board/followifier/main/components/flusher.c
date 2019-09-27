//
// Created by marcomicera on 8/9/19.
//

#include <stdlib.h>
#include <sys/socket.h>
#include <tcpip_adapter.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <message.pb-c.h>
#include "flusher.h"
#include "util/misc.h"
#include "wifi.h"

// Buffer
Followifier__ESP32Message * messages[FLUSH_THRESHOLD];
uint8_t *buffer;
unsigned int batch_length = 0;
unsigned short items = 0;

void flush();

void prepare_to_flush(bool stop);

void flushAsNewTask();

void store_message(Followifier__ESP32Message *serialized_data, sniffer_runtime_t sniffer) {

    // Better check it twice
    if (items == FLUSH_THRESHOLD) {
        prepare_to_flush(false);
        ESP_LOGW(TAG, "Last packet has not been sent to the server.");
        return;
    }

    // Storing the message into the buffer
    messages[items] = serialized_data;

    ++items;

    // Time to flush the message buffer
    if (items == FLUSH_THRESHOLD) {
        prepare_to_flush(true);
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
void prepare_to_flush(bool stop) {
    Followifier__Batch batch = FOLLOWIFIER__BATCH__INIT;
    batch.messages = messages;
    batch.n_messages = items;
    // Printing info
    ESP_LOGI(TAG, "Time to flush the message buffer. Sending %d messages...", items);
    batch_length = followifier__batch__get_packed_size(&batch);
    buffer = malloc(batch_length);
    followifier__batch__pack(&batch, buffer);
    ESP_LOGI(TAG, "%u", batch_length);
    ESP_LOGI(TAG, "%s", buffer);

    // Stopping the sniffer
    if (stop)
        ESP_ERROR_CHECK(stop_sniffer());

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
    MUST_NOT_HAPPEN((tcp_socket = socket(addr_family, SOCK_STREAM, ip_protocol)) < 0,
                    "socket() error");
    ESP_LOGI(TAG, "Socket created, connecting to %s:%d...", SERVER_ADDRESS, SERVER_PORT);

    // Creating connection to the server
    MUST_NOT_HAPPEN(connect(tcp_socket, (struct sockaddr *) &server_address, sizeof(server_address)) != 0,
                    "Error while connecting to the server");

    // Flushing the message buffer
    ESP_LOGI(TAG, "Sending batch");
    MUST_NOT_HAPPEN(send(tcp_socket, (char *) buffer, batch_length, 0) < 0,
                    "Error while sending batch");
    ESP_LOGI(TAG, "Sending delimiter...");
    MUST_NOT_HAPPEN(send(tcp_socket, "\n\r\n\r", sizeof("\n\r\n\r"), 0) < 0,
                    "Error while sending delimiter");

    items = 0;
    free(buffer);

    // Closing socket
    ESP_LOGI(TAG, "Shutting down socket towards %s:%d...", SERVER_ADDRESS, SERVER_PORT);
    shutdown(tcp_socket, 0);
    close(tcp_socket);
    ESP_LOGI(TAG, "...socket towards %s:%d closed.", SERVER_ADDRESS, SERVER_PORT);

    // Turning the Wi-Fi off
    ESP_ERROR_CHECK(stop_wifi());

    // Re-activating the sniffer
    init_sniffer();

    // Re-activating the sniffer
    ESP_ERROR_CHECK(start_sniffer());
}
