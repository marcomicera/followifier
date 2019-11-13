//
// Created by marcomicera on 11/13/19.
//

#ifndef BOARD_SNTP_H
#define BOARD_SNTP_H

/**
 * Initializes the SNTP module for time synchronization.
 */
void init_sntp();

/**
 * Obtains time from SNTP server.
 */
void obtain_time();

#endif //BOARD_SNTP_H
