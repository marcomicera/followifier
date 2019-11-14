#include "point.h"
#include <stdexcept>

int Point::getX() {
    return this->x;
}

int Point::getY() {
    return this->y;
}

bool Point::operator==(const Point &other) const {
    return this->x == other.x && this->y == other.y;
}

Point::Point(int x, int y) {
    if(x<0) {
        throw std::invalid_argument("Negative room x-coordinate (" + std::to_string(x) + ")");
    }
    if (y < 0) {
        throw std::invalid_argument("Negative room x-coordinate (" + std::to_string(y) + ")");
    }

    this->x = x;this->y = y;
}
