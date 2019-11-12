#include "settings.h"

Config settings::configuration;

void settings::load(const std::string &filename) {
    pt::ptree tree;

    // Parse the XML into the property tree.
    pt::read_json(filename, tree);

    // Use the throwing version of get to find the port.
    // If the port cannot be resolved, an exception is thrown.
    configuration.port = tree.get<size_t>("port");

    // Use get_child to find the node containing the modules, and iterate over
    // its children. If the path cannot be resolved, get_child throws.
    // A C++11 for-range loop would also work.
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("boards")) {
                    // The data function is used to access the data stored in a node.
                    Board b;
                    b.mac = v.second.get<std::string>("mac");
                    Point coordinates;
                    coordinates.x = v.second.get<int>("x");
                    coordinates.y = v.second.get<int>("y");
                    b.coordinates = coordinates;
                    configuration.boards.push_back(b);
                }

    if (tree.get_child("room_coordinates").size() != 4)
        throw "Invalid setting file";

    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("room_coordinates")) {
                    Point p;
                    p.x = v.second.get<int>("x");
                    p.y = v.second.get<int>("y");
                    configuration.room_coordinates.push_back(p);
                }
}