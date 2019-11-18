//
// Created by marcomicera on 11/13/19.
//

#ifndef BOARD_SNTP_H
#define BOARD_SNTP_H

/**
 * Returns whether the time has been set already or not.
 *
 * @return true if time has been set already, false otherwise.
 */
bool time_has_been_set();

/**
 * Initializes the SNTP module for time synchronization.
 */
void init_sntp();

/**
 * Sends a request to the SNTP server.
 */
void send_sntp_request();

#endif //BOARD_SNTP_H
