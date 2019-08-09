//
// Created by marcomicera on 8/9/19.
//

#include <stdlib.h>
#include <sys/socket.h>
#include <tcpip_adapter.h>
#include <esp_wifi.h>
#include "flusher.h"
#include "util.h"

// Buffer info
#define FLUSH_THRESHOLD 5 // number of messages to be saved before flushing

// Server info
#define WIFI_SSID "Wi-Fi SSD"
#define WIFI_PASSWORD "Wi-Fi password"
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#define SERVER_ADDRESS "192.168.0.90"
#define SERVER_PORT 12345

// Buffer
void *buffer[FLUSH_THRESHOLD];
unsigned short items = 0;

// Server
int tcp_socket;
struct sockaddr_in server_address;
int opt = 1;

void initialize_flusher() {

}

void flush();

void store_message(void *serialized_data, unsigned message_length) {

    // Time to flush the message buffer
    if (items == FLUSH_THRESHOLD) {
        flush();
    }

    // Storing the message into the buffer
    buffer[items] = serialized_data;
    ++items;

    // Time to flush the message buffer
    if (items == FLUSH_THRESHOLD) {
        flush();
    }
}

static void wifi_connect();

void flush() {

    // Connecting to the Wi-fi network
    wifi_connect();

    // Connecting to the server
    MUST_NOT_HAPPEN((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0,
                 "socket() error");
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET; // IPv4
    server_address.sin_port = htons(SERVER_PORT);
    MUST_NOT_HAPPEN(inet_pton(AF_INET, SERVER_ADDRESS, &server_address.sin_addr) <= 0,
                 "Invalid address/Address not supported");
    MUST_NOT_HAPPEN(connect(tcp_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0,
                 "Error while connecting to the server");

    // Flushing the message buffer
    for (unsigned short i = 0; i < FLUSH_THRESHOLD; ++i) {
        MUST_NOT_HAPPEN(send(tcp_socket, (void*)&buffer[i], sizeof(buffer[i]), 0) == -1,
                "Error while sending message number %hu", i);
        free(buffer[i]);
    }

    items = 0;
}

/**
 * It connects to the Wi-Fi network, setting the
 * ESP32 board as a station mode (sta)
 */
static void wifi_connect(void) {

    tcpip_adapter_init();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
            .sta = {
                    .ssid = WIFI_SSID,
                    .password = WIFI_PASSWORD,
                    .scan_method = DEFAULT_SCAN_METHOD,
                    .sort_method = DEFAULT_SORT_METHOD,
                    .threshold.rssi = DEFAULT_RSSI,
                    .threshold.authmode = DEFAULT_AUTHMODE,
            },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}
