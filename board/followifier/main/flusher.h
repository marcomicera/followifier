//
// Created by marcomicera on 8/9/19.
//

#ifndef FOLLOWIFIER_FLUSHER_H
#define FOLLOWIFIER_FLUSHER_H

// Server info
#define WIFI_SSID "Wi-Fi SSID" // FIXME
#define WIFI_PASSWORD "Wi-Fi password"  // FIXME
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#define SERVER_ADDRESS "Server address"  // FIXME
#define SERVER_PORT 12345  // FIXME

bool hasGotIp;

void store_message(void *, unsigned);

#endif //FOLLOWIFIER_FLUSHER_H
