//
// Created by simona on 26/11/19.
//

#ifndef CORE_CALIBRATION_H
#define CORE_CALIBRATION_H


#include <string>
#include "settings.h"
#include <ctime>

class calibration {

protected:

    /**
     * How many boards have been calibrated so far.
     */
    static short board_counter;

public:

    static std::string board_to_calibrate;

    static long int starting_timestamp;

    static bool board_has_sent_calibration_batch;

    /**
     * Waits for the user to place the specified board at 1 meter distance from the server.
     *
     * @param board_mac         MAC address of the board to be placed.
     */
    static void wait_placement(const std::string &board_mac);

};

#endif //CORE_CALIBRATION_H
