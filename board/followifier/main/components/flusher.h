//
// Created by marcomicera on 8/9/19.
//

#ifndef FOLLOWIFIER_FLUSHER_H
#define FOLLOWIFIER_FLUSHER_H

#include "sniffer.h"

/**
 * How many messages should the sniffer store before sending those to
 * the core server.
 */
// FIXME Implement timer + messages threshold flushing conditions (#43)
#define FLUSH_THRESHOLD 3 // number of messages to be saved before flushing

/**
 * Stores a message and eventually flushes all stored ones.
 */
void store_message(Followifier__ESP32Message *);

/**
 * Sends all stored messages to the core server.
 */
void flush();

/**
 * Board's Wi-Fi station MAC address
 */
extern uint8_t mac_address[6];

/**
 * Initializes the flusher component.
 */
void init_flusher();
/**
 * prepare to flush batch
 */
void prepare_to_flush(bool stop);
#endif //FOLLOWIFIER_FLUSHER_H
