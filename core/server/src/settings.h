#ifndef CORE_SETTINGS_H
#define CORE_SETTINGS_H

#include "board.h"
#include "point.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <bits/stdc++.h>

namespace pt = boost::property_tree;

typedef struct {

    size_t port; //server port number

    /**
     * Map of boards' MAC address to their corresponding object.
     */
    std::unordered_map<std::string, Board> boards;

    std::unordered_set<Point, Point::PointHasher> room_coordinates; //coordinates of room
} Config;

class Settings {

public:

    static Config configuration;

    static void load(const std::string &);

    static size_t get_num_boards() {
        return configuration.boards.size();
    }
};

#endif //CORE_SETTINGS_H
