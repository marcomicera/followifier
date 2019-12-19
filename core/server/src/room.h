//
// Created by simona on 28/11/19.
//

#ifndef CORE_ROOM_H
#define CORE_ROOM_H

#include "point.h"


class Room {

public:

    Room(Point &a, Point &b, Point &c, Point &d);
    Room();

    Point  getLeftTop();

    Point  getLeftBottom();

    Point  getRightTop();

    Point getRightBottom();

    bool isPointInside(Point point);


private:

    Point left_bottom; //left bottom corner
    Point left_top; //left top corner
    Point right_bottom; //right bottom corner
    Point right_top; //right top corner

};


#endif //CORE_ROOM_H
