#include <stdio.h>
#include <string.h> // strlen
#include <errno.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "nvs_flash.h"
#include "esp_log.h"

#if CONFIG_WIFI_ALL_CHANNEL_SCAN
#define DEFAULT_SCAN_METHOD WIFI_ALL_CHANNEL_SCAN
#elif CONFIG_WIFI_FAST_SCAN
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#else
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#endif /*CONFIG_SCAN_METHOD*/

#if CONFIG_WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#elif CONFIG_WIFI_CONNECT_AP_BY_SECURITY
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SECURITY
#else
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#endif /*CONFIG_SORT_METHOD*/

#if CONFIG_FAST_SCAN_THRESHOLD
#define DEFAULT_RSSI CONFIG_FAST_SCAN_MINIMUM_SIGNAL
#if CONFIG_EXAMPLE_OPEN
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#elif CONFIG_EXAMPLE_WEP
#define DEFAULT_AUTHMODE WIFI_AUTH_WEP
#elif CONFIG_EXAMPLE_WPA
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA_PSK
#elif CONFIG_EXAMPLE_WPA2
#define DEFAULT_AUTHMODE WIFI_AUTH_WPA2_PSK
#else
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif
#else
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#endif /*CONFIG_FAST_SCAN_THRESHOLD*/

/**
 * Wi-Fi info, to be set via:
 * 'make menuconfig' -> "Component config" ->
 */
#define WIFI_SSID 			"Wi-Fi SSID"
#define WIFI_PASSWORD	 	"Wi-Fi password"

/**
 * ESP32 info
 */
#define	BOARD_PORT			4545

/**
 * Server info
 */
#define SERVER_ADDRESS	 	"192.168.1.3"
#define SERVER_PORT	 		9991

/**
 * Server address data structure
 */
struct sockaddr_in server_address;

/**
 * TCP sockets towards the server
 */
int tcp_socket;

/**
 * Message to be sent
 */
static const char* test_string = "This is a test string\n\0";

/**
 * Debugging
 */
static const char* TAG = "echo-upper-client";

/**
 * It creates a TCP/IPv4 socket  with the specified
 * address and port and connects to it.
 */
void tcp_ipv4_connect(const char* address, int port) {
	// Creating TCP socket
	if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		ESP_LOGI(TAG, "socket() error_ %s", strerror(errno));
		exit(1);
	}

	// Server address structure
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET; // IPv4
	server_address.sin_port = htons(port);
	if (inet_pton(AF_INET, address, &server_address.sin_addr) == 0) {
		ESP_LOGI(TAG, "inet_pton() error: %s", strerror(errno));
		exit(1);
	}

	// Connecting to the server
	if (connect(tcp_socket, (struct sockaddr*) &server_address,
			sizeof(server_address)) == -1) {
		ESP_LOGI(TAG, "connect() error: %s", strerror(errno));
		exit(1);
	}
	ESP_LOGI(TAG, "Connected to %s:%d successfully!",
			address, port);
}

/**
 * Event handler
 */
static esp_err_t event_handler(void *ctx, system_event_t *event) {
	ESP_LOGI(TAG, "New event: %d", event->event_id);

	switch (event->event_id) {
		case SYSTEM_EVENT_STA_START:
			ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
			ESP_ERROR_CHECK(esp_wifi_connect());
			break;
		case SYSTEM_EVENT_STA_GOT_IP:
			ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
			ESP_LOGI(TAG, "Got IP: %s\n",
					ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));

			// Connecting to the server as soon as it gets an IP address
			tcp_ipv4_connect(SERVER_ADDRESS, SERVER_PORT);

			// Sending a test string to the server
			if(send(tcp_socket, (void*)test_string, strlen(test_string) + 1, 0) == -1) {
				ESP_LOGI(TAG, "send() error: %s", strerror(errno));
			}

			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:
			ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
			ESP_ERROR_CHECK(esp_wifi_connect());
			break;
		default:
			break;
	}
	return ESP_OK;
}

/**
 * It connects to the Wi-Fi network, setting the
 * ESP32 board as a station mode (sta)
 */
static void wifi_connect(void) {
	tcpip_adapter_init();
	ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

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

/**
 * This program's entry point
 */
void app_main() {
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Connecting to Wi-Fi
	wifi_connect();
}
