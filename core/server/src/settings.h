#ifndef CORE_SETTINGS_H
#define CORE_SETTINGS_H

#include "board.h"
#include "point.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <set>

namespace pt = boost::property_tree;
typedef struct {
    size_t port; //server port number
    std::vector<Board> boards; //set of boards
    std::vector<Point> room_coordinates; //coordinates of room
} Config;

class settings {
public:
    static Config configuration;

    static void load(const std::string &);
};

#endif //CORE_SETTINGS_H
