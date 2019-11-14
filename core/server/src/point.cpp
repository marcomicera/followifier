#include "point.h"

uint64_t Point::operator()(const Point &p) const {
    return ((uint64_t)p.x)<<32 | (uint64_t)p.y;
}

int Point::getX() {
    return this->x;
}

int Point::getY() {
    return this->y;
}

bool Point::operator==(const Point &other) const {
    return this->x == other.x && this->y == other.y;
}

void Point::setX(int x) {
    this->x = x;
}

void Point::setY(int y) {
    this->y = y;
}