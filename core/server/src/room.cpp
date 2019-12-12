//
// Created by simona on 28/11/19.
//

#include "room.h"

Room::Room() : left_bottom(0, 0), left_top(0,0), right_bottom(0,0), right_top(0,0){}

Room::Room(Point &a, Point &b, Point &c, Point &d) : left_bottom(0, 0), left_top(0,0), right_bottom(0,0), right_top(0,0){

    if(a.getX()!=b.getX() && c.getX()!=d.getX()) {
        throw std::invalid_argument("Invalid X coordinates for a room");
    }
    if(a.getY()!=c.getY() && b.getY()!=d.getY()) {
        throw std::invalid_argument("Invalid Y coordinates for a room");
    }

    this->left_bottom = Point(a.getX(), a.getY());
    this->left_top = Point(b.getX(), b.getY());
    this->right_bottom = Point(c.getX(), c.getY());
    this->right_top = Point(d.getX(), d.getY());
}

Point Room::getLeftBottom() {
    return left_bottom;
}

Point Room::getLeftTop() {
    return left_top;
}

Point Room::getRightBottom() {
    return right_bottom;
}

Point Room::getRightTop() {
    return right_top;
}

bool Room::isPointInside(Point point){
    return point.getX() >= left_top.getX() && point.getX() <= right_top.getX() &&
           point.getY() >= left_bottom.getY() && point.getY() <= right_bottom.getY();
}
