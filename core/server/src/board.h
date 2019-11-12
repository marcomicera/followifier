#ifndef CORE_BOARD_H
#define CORE_BOARD_H
#include "point.h"
#include <string>

typedef struct{
    std::string mac; //board mac
    Point coordinates; //board coordinates
}Board;
#endif //CORE_BOARD_H
