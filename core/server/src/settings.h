#ifndef CORE_SETTINGS_H
#define CORE_SETTINGS_H

#include "board.h"
#include "point.h"
#include "database.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <bits/stdc++.h>
#include "room.h"

namespace pt = boost::property_tree;
typedef struct {
    /**
     * This server's port number towards boards.
     */
    size_t port;

    /**
     * Map of boards' MAC address to their corresponding object.
     */
    std::unordered_map<std::string, Board> boards;

    /**
     * Room vertexes
     */
    Room room_coordinates;

    /**
     * MAC address of the mobile device used for boards calibration.
     * This field is optional (in case it is missing, no calibration will be performed).
     */
    boost::optional<std::string> calibration_device_mac_address;

    /**
     * Minimum number of calibration messages needed to compute the
     * average 1-meter-distance RSSI value of all boards.
     * This field is optional (in case it is missing, a default value will be used).
     */
    boost::optional<int> min_num_calibration_messages;

    /**
     * How many seconds does the user have to place the mobile device at 1 meter
     * distance from boards, one at the time.
     */
    boost::optional<int> calibration_placement_duration_in_seconds;

} Config;

class Settings {
public:
    static Config configuration;
    static void load(const std::string &);
};

#endif //CORE_SETTINGS_H
