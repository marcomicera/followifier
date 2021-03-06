#include "point.h"
#include <stdexcept>
#include <cmath>

const double Point::getX() {
    return this->x;
}

const double Point::getY() {
    return this->y;
}

bool Point::operator==(const Point &other) const {
    return this->x == other.x && this->y == other.y;
}

Point::Point(double x, double y) {
    if(x<0) {
        throw std::invalid_argument("Negative x-coordinate (" + std::to_string(x) + ")");
    }
    if (y < 0) {
        throw std::invalid_argument("Negative y-coordinate (" + std::to_string(y) + ")");
    }

    this->x = x;this->y = y;
}

bool Point::isValid() {
    if(!std::isnan(x) && !std::isnan(y))
        return true;
    return false;
}

std::ostream& operator<<(std::ostream &ostream, const Point &point) {
    return ostream << "<x = " << point.x << ", y = " << point.y << ">";
}
