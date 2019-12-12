#include "statistics.h"
#include "settings.h"
#include <iostream>

Point statistics::getDevicePosition(std::unordered_map<std::string, followifier::ESP32Metadata> &board_metadatas) {

    std::unordered_set<Point, Point::PointHasher> listPossiblePoints;

    /* Logging announced distances */
    for (auto & board_metadata : board_metadatas) {
        std::string board_mac = board_metadata.first;
        double estimated_distance = statistics::estimatedDistance(board_mac, board_metadata.second.rssi());
        logDeviceDistanceAnnouncement(board_mac, board_metadata.second, estimated_distance);
    }

    for (auto i = board_metadatas.begin(); i != board_metadatas.end(); ++i) {
        for (auto j = std::next(i, 1); j != board_metadatas.end(); ++j) {

            std::string first_board_mac = i->first;
            std::string second_board_mac = j->first;

            double x1 = Settings::configuration.boards.find(first_board_mac)->second.getCoordinates().getX();
            double x2 = Settings::configuration.boards.find(second_board_mac)->second.getCoordinates().getX();
            double y1 = Settings::configuration.boards.find(first_board_mac)->second.getCoordinates().getY();
            double y2 = Settings::configuration.boards.find(second_board_mac)->second.getCoordinates().getY();

            // Interception points
            // TODO Implement tolerance?
            double r1 = statistics::estimatedDistance(first_board_mac, i->second.rssi());
            double r2 = statistics::estimatedDistance(second_board_mac, j->second.rssi());

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

            if (xr1 >= 0 && yr1 >= 0 && checkPoint(xr1, yr1, board_metadatas) && Point(xr1, yr1).isValid()) {
                listPossiblePoints.insert(Point(std::round(xr1), std::round(yr1)));
            }
            if (xr2 >= 0 && yr2 >= 0 && checkPoint(xr2, yr2, board_metadatas) && Point(xr2, yr2).isValid()) {
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
    if(Settings::configuration.room_coordinates.isPointInside(center))
        return center;
    else
        return Point(nan(""), nan(""));
}

double statistics::estimatedDistance(const std::string &board_mac, double rssi_value) {
    double one_meter_rssi = statistics::get_one_meter_rssi_or_default(board_mac);
    return std::pow(10, (one_meter_rssi - rssi_value) / (10 * 2)) * 100;
}

bool statistics::checkPoint(double x, double y,
                            std::unordered_map<std::string, followifier::ESP32Metadata> &boardMetadatas) {

    for (auto &board : Settings::configuration.boards) {

        if (std::sqrt(std::pow(x - board.second.getCoordinates().getX(), 2) +
                      std::pow(y - board.second.getCoordinates().getY(), 2)) -
            std::pow(statistics::estimatedDistance(board.first, boardMetadatas.find(board.first)->second.rssi()), 2) > 0) {

            return false;
        }
    }
    return true;
}