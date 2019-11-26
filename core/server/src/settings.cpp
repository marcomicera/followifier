#include "settings.h"

Config Settings::configuration;
std::string Settings::board_to_calibrate;

void Settings::load_calibration_settings(const pt::ptree &tree) {

    /* Looking for the 'calibration' object */
    boost::property_tree::ptree::const_assoc_iterator calibration_iterator = tree.find("calibration");
    if (calibration_iterator == tree.not_found()) { // calibration settings have not been found
        std::cout << "Calibration settings have not been found: boards calibration will not be performed." << std::endl;
    } else { // calibration settings have been found

        /* Retrieving the calibration object */
        std::cout << "Calibration settings have been found." << std::endl;
        const boost::property_tree::ptree &calibration_settings = (*calibration_iterator).second;

        /* MAC address of the device that will be used for calibration. This field is optional. */
        configuration.calibration_device_mac_address = calibration_settings.get_optional<std::string>(
                "calibration_device");
        if (!configuration.calibration_device_mac_address) { // if the calibration device has not been specified
            std::cout << "Calibration device has not been specified: boards calibration will not be performed."
                      << std::endl;
            return;
        } else {
            std::cout << "Calibration device is " << configuration.calibration_device_mac_address.value() << "."
                      << std::endl;
        }

        /* Minimum number of calibration messages needed to compute the
         * average 1-meter-distance RSSI value of all boards
         */
        configuration.min_num_calibration_messages = calibration_settings.get_optional<int>(
                "min_num_calibration_messages");
        if (!configuration.min_num_calibration_messages) {

            /* This field is optional, so a default value will be used in case
             * it is missing in the configuration file
             */
            std::cout << "Setting the minimum number of messaged needed to perform calibration to "
                      << DEFAULT_MIN_NUM_CALIBRATION_MESSAGES << "." << std::endl;
            configuration.min_num_calibration_messages = DEFAULT_MIN_NUM_CALIBRATION_MESSAGES;
        } else {
            std::cout << "The minimum number of messaged needed to perform calibration is "
                      << configuration.min_num_calibration_messages.value() << "." << std::endl;
        }

        /* How many seconds does the user have to place boards at 1 meter
         * distance from this server, one at the time.
         */
        configuration.calibration_placement_duration_in_seconds = calibration_settings.get_optional<int>(
                "placement_duration_in_seconds");
        if (!configuration.calibration_placement_duration_in_seconds) {

            /* This field is optional, so a default value will be used in case
             * it is missing in the configuration file
             */
            configuration.calibration_placement_duration_in_seconds = DEFAULT_CALIBRATION_PLACEMENT_DURATION_IN_SECONDS;
        }
        std::cout << "The user will have " << configuration.calibration_placement_duration_in_seconds.value()
                  << " seconds to place the mobile device at 1 meter from a board, before its calibration phase begins."
                  << std::endl;
    }
}

void Settings::load(const std::string &filename) {

    pt::ptree tree;

    // Parse the XML into the property tree.
    pt::read_json(filename, tree);

    // Loading calibration settings
    Settings::load_calibration_settings(tree);

    // Use the throwing version of get to find the port.
    // If the port cannot be resolved, an exception is thrown.
    configuration.port = tree.get<size_t>("port");

    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("room_coordinates")) {
                    Point p(v.second.get<double>("x"), v.second.get<double>("y"));
                    configuration.room_coordinates.insert(p);
                }
    if (configuration.room_coordinates.size() != 4) {
        throw std::invalid_argument("Duplicate room coordinates");
    }

    // Use get_child to find the node containing the modules, and iterate over
    // its children. If the path cannot be resolved, get_child throws.
    // A C++11 for-range loop would also work.
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("boards")) {
                    // The data function is used to access the data stored in a node.
                    // TODO Check whether boards fit into the room or not
                    Point coordinates(v.second.get<double>("x"), v.second.get<double>("y"));
                    Board b(v.second.get<std::string>("mac"), coordinates);
                    configuration.boards.insert(std::make_pair(b.getMac(), b));
                }
    if (Settings::get_num_boards() <= 1) {
        throw std::invalid_argument("At least 2 boards must be configured");
    }
}