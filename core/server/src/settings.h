#ifndef CORE_SETTINGS_H
#define CORE_SETTINGS_H

#include "board.h"
#include "point.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <bits/stdc++.h>

/*
 * Default minimum number of calibration messages nedded to compute the
 * average 1-meter-distance RSSI value of all boards.
 * This default value makes the its relative entry in the configuration
 * file optional.
 */
#define DEFAULT_MIN_NUM_CALIBRATION_MESSAGES 3

/*
 * Default board calibration placement duration in seconds.
 */
#define DEFAULT_CALIBRATION_PLACEMENT_DURATION_IN_SECONDS 10

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
    std::unordered_set<Point, Point::PointHasher> room_coordinates;

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

    static size_t get_num_boards() {
        return configuration.boards.size();
    }

private:

    /**
     * Loads settings regarding the initial calibration phase during which a mobile device
     * must be placed at 1 meter from each board, one at the time.
     *
     * @param root_config_tree  JSON root tree of the whole configuration file.
     */
    static void load_calibration_settings(const pt::ptree& root_config_tree);
};

#endif //CORE_SETTINGS_H
