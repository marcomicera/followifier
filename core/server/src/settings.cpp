#include "settings.h"

Config Settings::configuration;

void Settings::load(const std::string &filename) {

    pt::ptree tree;
    database db;
    db.dropBoards();
    // Parse the XML into the property tree.
    pt::read_json(filename, tree);

    // Use the throwing version of get to find the port.
    // If the port cannot be resolved, an exception is thrown.
    configuration.port = tree.get<size_t>("port");

    std::vector<Point> room_coordinates;
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("room_coordinates")) {
                    Point p(v.second.get<double>("x"), v.second.get<double>("y"));
                    room_coordinates.push_back(p);
                }
    if (room_coordinates.size() != 4) {
        throw std::invalid_argument("Missing room coordinate");
    }
    configuration.room_coordinates = Room(room_coordinates[0], room_coordinates[1], room_coordinates[2], room_coordinates[3]);

    db.drop_room_collection();
    db.insert_room_coordinate(room_coordinates[0]);
    db.insert_room_coordinate(room_coordinates[1]);
    db.insert_room_coordinate(room_coordinates[2]);
    db.insert_room_coordinate(room_coordinates[3]);

    // Use get_child to find the node containing the modules, and iterate over
    // its children. If the path cannot be resolved, get_child throws.
    // A C++11 for-range loop would also work.
    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("boards")) {
                    // The data function is used to access the data stored in a node.
                    // TODO Check whether boards fit into the room or not
                    Point coordinates(v.second.get<double>("x"), v.second.get<double>("y"));
                    Board b(v.second.get<std::string>("mac"), coordinates);
                    configuration.boards.insert(std::make_pair(b.getMac(), b));
                    db.insert_board(v.second.get<std::string>("mac"), v.second.get<double>("x"), v.second.get<double>("y"));
                }
    if (Settings::configuration.boards.size() <=1) {
        throw std::invalid_argument("At least 2 boards must be configured");
    }
}