#ifndef CORE_BOARD_H
#define CORE_BOARD_H

#include "point.h"
#include <string>

class Board {
public:
    std::string getMac();
    void setMac(std::string mac);
    Point getCoordinates();
    void setCoordintes(Point coordinates);

    bool operator==(const Board &other) const;

    // mac is returned as hash function
    size_t operator()(const Board &b) const;

private:
    std::string mac; //board mac
    Point coordinates; //board coordinates
};
#endif //CORE_BOARD_H
