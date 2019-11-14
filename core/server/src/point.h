#ifndef CORE_POINT_H
#define CORE_POINT_H

#include <cstdint>

class Point {
public:
    // id is returned as hash function
    uint64_t operator()(const Point& p) const;
    bool operator==(const Point &other) const;

    int getX();
    void setX(int);
    int getY();
    void setY(int);
private:
    int x;
    int y;
};
#endif //CORE_POINT_H
