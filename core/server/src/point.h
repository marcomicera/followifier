#ifndef CORE_POINT_H
#define CORE_POINT_H

#include <cstdint>
#include <ostream>

class Point {

public:

    Point(double x, double y);

    // id is returned as hash function
    bool operator==(const Point &other) const;

    struct PointHasher {
        uint64_t operator()(const Point &p) const {
            return ((uint64_t) p.x) << 32 | (uint64_t) p.y;
        }
    };

    double getX();

    double getY();
    bool isValid();

    friend std::ostream& operator<<(std::ostream&, const Point&);

private:

    double x;

    double y;
};

#endif //CORE_POINT_H
