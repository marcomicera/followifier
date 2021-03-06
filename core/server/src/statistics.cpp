#include "statistics.h"
#include "settings.h"
#include <iostream>

Point statistics::getDevicePosition(std::unordered_map<std::string, followifier::ESP32Metadata> &boardMetadatas) {

    std::unordered_set<Point, Point::PointHasher> listPossiblePoints;

    for (auto i = boardMetadatas.begin(); i != boardMetadatas.end(); ++i) {
        for (auto j = std::next(i, 1); j != boardMetadatas.end(); ++j) {

            double x1 = Settings::configuration.boards.find(i->first)->second.getCoordinates().getX();
            double x2 = Settings::configuration.boards.find(j->first)->second.getCoordinates().getX();
            double y1 = Settings::configuration.boards.find(i->first)->second.getCoordinates().getY();
            double y2 = Settings::configuration.boards.find(j->first)->second.getCoordinates().getY();

            // Interception points
            double r1 = statistics::estimatedDistance(i->second.rssi());
            double r2 = statistics::estimatedDistance(j->second.rssi());
            logDeviceDistanceAnnouncement(i->first, i->second, r1);
            logDeviceDistanceAnnouncement(j->first, j->second, r2);

            // d = sqrt((x1-x2)^2 + (y1-y2)^2)
            double d = std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));

            // l = r1^2-r2^2 + d^2 / 2d
            double l = (std::pow(r1, 2) - std::pow(r2, 2) + std::pow(d, 2)) / (2 * d);

            // h = sqrt(r1^2 - l^2)
            double h = std::sqrt(std::pow(r1, 2) - std::pow(l, 2));

            /*
             * Results
             */

            // x results = l(x2 - x1) / d + -h(y2 - y1) / d + x1
            double xr1 = l * (x2 - x1) / d + h * (y2 - y1) / d + x1;
            double xr2 = l * (x2 - x1) / d - h * (y2 - y1) / d + x1;

            // y results = l(y2 - y1) / d -+ h(x2 - x1) / d + y1
            double yr1 = l * (y2 - y1) / d - h * (x2 - x1) / d + y1;
            double yr2 = l * (y2 - y1) / d + h * (x2 - x1) / d + y1;

            if (xr1 >= 0 && yr1 >= 0 && checkPoint(xr1, yr1, boardMetadatas) && Point(xr1, yr1).isValid()) {
                listPossiblePoints.insert(Point(std::round(xr1), std::round(yr1)));
            }
            if (xr2 >= 0 && yr2 >= 0 && checkPoint(xr2, yr2, boardMetadatas) && Point(xr2, yr2).isValid()) {
                listPossiblePoints.insert(Point(std::round(xr2), std::round(yr2)));
            }
        }
    }

    //calculate center
    double Px = 0, Py = 0;
    for (Point p : listPossiblePoints) {
        Px += p.getX();
        Py += p.getY();
    }

    Point center = Point(Px / listPossiblePoints.size(), Py / listPossiblePoints.size());
    if(INCLUDE_DEVICES_OUT_OF_ROOM || Settings::configuration.room_coordinates.isPointInside(center))
        return center;
    else
        return Point(nan(""), nan(""));
}

double statistics::estimatedDistance(double rssi) {
    double distance = std::pow(10, (ONE_METER_RSSI - rssi) / (10 * 2)) * 100;
    return distance * 1.0; // FIXME Make tolerance a variable
}

bool statistics::checkPoint(double x, double y,
                            std::unordered_map<std::string, followifier::ESP32Metadata> &boardMetadatas) {

    for (auto &board : Settings::configuration.boards) {

        if (std::sqrt(std::pow(x - board.second.getCoordinates().getX(), 2) +
                      std::pow(y - board.second.getCoordinates().getY(), 2)) -
            std::pow(statistics::estimatedDistance(boardMetadatas.find(board.first)->second.rssi()), 2) > 0) {

            return false;
        }
    }
    return true;
}