//
// Created by marcomicera on 8/9/19.
//

#ifndef FOLLOWIFIER_FLUSHER_H
#define FOLLOWIFIER_FLUSHER_H

#include "sniffer.h"

/**
 * Server info
 */
#define SERVER_ADDRESS "192.168.1.105"  // FIXME
#define SERVER_PORT 12345  // FIXME

/**
 * How many messages should the sniffer store before sending those to
 * the core server.
 */
#define FLUSH_THRESHOLD 3 // number of messages to be saved before flushing

/**
 * Stores a message and eventually flushes all stored ones.
 */
void store_message(uint8_t *, unsigned, sniffer_runtime_t);

/**
 * Sends all stored messages to the core server.
 */
void flush();

#endif //FOLLOWIFIER_FLUSHER_H
