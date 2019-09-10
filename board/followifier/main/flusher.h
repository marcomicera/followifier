//
// Created by marcomicera on 8/9/19.
//

#ifndef FOLLOWIFIER_FLUSHER_H
#define FOLLOWIFIER_FLUSHER_H

/*
 * TODO
 *  Read "Establishing Wi-Fi or Ethernet Connection" section in
 *  esp-idf/examples/protocols/README.md for more information about this function.
 */

// Server info
#include "sniffer.h"

#define WIFI_SSID "Sgt. Pepper's Wi-Fi" // FIXME
#define WIFI_PASSWORD "y^?8_&E@e)4x<9/B"  // FIXME
#define DEFAULT_SCAN_METHOD WIFI_FAST_SCAN
#define DEFAULT_SORT_METHOD WIFI_CONNECT_AP_BY_SIGNAL
#define DEFAULT_RSSI -127
#define DEFAULT_AUTHMODE WIFI_AUTH_OPEN
#define SERVER_ADDRESS "192.168.1.13"  // FIXME
#define SERVER_PORT 12345  // FIXME

// Buffer info
#define FLUSH_THRESHOLD 3 // number of messages to be saved before flushing

bool hasGotIp;

void store_message(void *, unsigned, sniffer_runtime_t);

#endif //FOLLOWIFIER_FLUSHER_H
