#ifndef CORE_BOARD_H
#define CORE_BOARD_H

#include "point.h"
#include <string>

class Board {

public:

    Board(const std::string &mac, const Point &coordinates);

    std::string getMac();

    Point getCoordinates();

    bool operator==(const Board &other) const;

private:

    std::string mac; //board mac

    Point coordinates; //board coordinates
};

#endif //CORE_BOARD_H
