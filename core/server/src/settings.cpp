#include "settings.h"

Config Settings::configuration;

void Settings::load(const std::string &filename) {

    pt::ptree tree;

    // Parse the XML into the property tree.
    pt::read_json(filename, tree);

    // Use the throwing version of get to find the port.
    // If the port cannot be resolved, an exception is thrown.
    configuration.port = tree.get<size_t>("port");

    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("room_coordinates")) {
                    Point p(v.second.get<int>("x"), v.second.get<int>("y"));
                    configuration.room_coordinates.insert(p);
                }
    if (configuration.room_coordinates.size() != 4) {
        throw std::invalid_argument("Duplicate room coordinates");
    }

    // Use get_child to find the node containing the modules, and iterate over
    // its children. If the path cannot be resolved, get_child throws.
    // A C++11 for-range loop would also work.
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("boards")) {
                    database database;
                    // The data function is used to access the data stored in a node.
                    // TODO Check whether boards fit into the room or not
                    Point coordinates(v.second.get<int>("x"), v.second.get<int>("y"));
                    Board b(v.second.get<std::string>("mac"), coordinates);
                    configuration.boards.insert(b);

                }
    if (tree.get_child("room_coordinates").size() != 4) {
        throw std::invalid_argument("Room must have exactly 4 coordinates");
    }
}