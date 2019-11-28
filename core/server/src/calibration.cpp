#include "calibration.h"

std::string calibration::board_to_calibrate;
long int calibration::starting_timestamp;
short calibration::board_counter = 0;
bool calibration::board_has_sent_calibration_batch;


void calibration::wait_placement(const std::string &board_mac) {

/* How many seconds are left for the user so s/he can place the board at 1 meter distance */
    int board_placements_seconds_left = Settings::configuration.calibration_placement_duration_in_seconds.value();

    std::cout << "Please place device " << Settings::configuration.calibration_device_mac_address.value()
              << " at 1 meter distance from board " << board_mac << " (" << (++calibration::board_counter) << "/"
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
    /* The timestamp corresponding to the instant the device has been placed is stored so that it can be used
     * to discard messages before that instant*/
    starting_timestamp = std::time(nullptr);
}
