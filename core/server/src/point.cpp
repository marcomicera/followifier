#include "point.h"
#include <stdexcept>
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
    if(x<0) {
        throw std::invalid_argument("Negative room x-coordinate (" + std::to_string(x) + ")");
    }

    this->x = x;
}

void Point::setY(int y) {
    if (y < 0) {
        throw std::invalid_argument("Negative room x-coordinate (" + std::to_string(y) + ")");
    }

    this->y = y;
}