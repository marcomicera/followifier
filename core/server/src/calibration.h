//
// Created by simona on 26/11/19.
//

#ifndef CORE_CALIBRATION_H
#define CORE_CALIBRATION_H


#include <string>
#include "settings.h"

class calibration {
public:
/**
 * Waits for the user to place the specified board at 1 meter distance from the server.
 *
 * @param board_mac         MAC address of the board to be placed.
 * @param board_counter     indicates how many boards have completed this process (printing purposes only).
 */
    static void wait_placement(const std::string &board_mac, short board_counter) {

        /* How many seconds are left for the user so s/he can place the board at 1 meter distance */
        int board_placements_seconds_left = Settings::configuration.calibration_placement_duration_in_seconds.value();

        std::cout << "Please place device " << Settings::configuration.calibration_device_mac_address.value()
                  << " at 1 meter distance from board " << board_mac << " (" << (board_counter + 1) << "/"
                  << Settings::get_num_boards() << ")." << std::endl;

        /* `INITIALIZATION_BOARD_PLACEMENT_WAITING_TIME` seconds countdown starts now */
        while (board_placements_seconds_left > 0) {
            std::cout << "\r" << board_placements_seconds_left << " second"
                      << (board_placements_seconds_left > 1 ? "s" : "") << " left...  " << std::flush;
            sleep(1);
            --board_placements_seconds_left;
        }
        std::cout << "\rReady to calibrate board " << board_mac << ". Please do not move the device any further."
                  << std::endl;
    }
};


#endif //CORE_CALIBRATION_H
