#include "settings.h"

Config Settings::configuration;

void Settings::load(const std::string &filename) {

    pt::ptree tree;

    // Parse the XML into the property tree.
    pt::read_json(filename, tree);

    // MAC address of the device that will be used for calibration. This field is optional.
    configuration.calibration_device_mac_address = tree.get_optional<std::string>("calibration_device");

    // Minimum number of calibration messages needed to compute the average 1-meter-distance RSSI value of all boards.
    configuration.min_num_calibration_messages = tree.get_optional<int>("min_num_calibration_messages");
    if (!configuration.min_num_calibration_messages) {

        // This field is optional, so a default value will be used in case it is missing in the configuration file.
        configuration.min_num_calibration_messages = DEFAULT_MIN_NUM_CALIBRATION_MESSAGES;
    }

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