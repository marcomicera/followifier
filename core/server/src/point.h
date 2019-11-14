#ifndef CORE_POINT_H
#define CORE_POINT_H

#include <cstdint>

class Point {

public:

    Point(int x, int y);

// id is returned as hash function
    bool operator==(const Point &other) const;

    struct PointHasher {
        uint64_t operator()(const Point &p) const {
            return ((uint64_t) p.x) << 32 | (uint64_t) p.y;
        }
    };

    int getX();

    int getY();

private:

    int x;

    int y;
};

#endif //CORE_POINT_H
