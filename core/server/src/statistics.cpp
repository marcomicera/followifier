#include "statistics.h"
#include "settings.h"
#include <iostream>

Point statistics::getPosition(std::string hash, std::unordered_map< std::string, followifier::ESP32Metadata> &messageBoardsAndMetadata) {
    std::unordered_set<Point, Point::PointHasher> listPossiblePoints;
    for (auto i = messageBoardsAndMetadata.begin(); i != messageBoardsAndMetadata.end(); i++) {
        for (auto j = std::next(i, 1); j != messageBoardsAndMetadata.end(); j++) {
            //calculate interception points
            double r1 = statistics::estimatedDistance(i->second.rsi());
            double r2 = statistics::estimatedDistance(j->second.rsi());

            //d = sqrt((x1-x2)^2 + (y1-y2)^2)
            double d = std::sqrt(
                    std::pow(Settings::configuration.boards.find(i->first)->second.getCoordinates().getX() -
                             Settings::configuration.boards.find(j->first)->second.getCoordinates().getX(), 2) +
                    std::pow(Settings::configuration.boards.find(i->first)->second.getCoordinates().getY() -
                             Settings::configuration.boards.find(j->first)->second.getCoordinates().getY(), 2));

            //l = r1^2-r2^2 + d^2 / 2d
            double l = (std::pow(r1, 2) - std::pow(r2, 2) + std::pow(d, 2)) / (2 * d);

            //h = sqrt(r1^2 - l^2)
            double h = std::sqrt(std::pow(r1, 2) - std::pow(l, 2));
                 //x = l(x2 - x1) / d + -h(y2 - y1) / d + x1
            double x1 = (l * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getX() -
                             Settings::configuration.boards.find(i->first)->second.getCoordinates().getX())) / d +
                       (h * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getY() -
                             Settings::configuration.boards.find(i->first)->second.getCoordinates().getY())) / d +
                       Settings::configuration.boards.find(i->first)->second.getCoordinates().getX();
            double x2 = (l * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getX() -
                             Settings::configuration.boards.find(i->first)->second.getCoordinates().getX())) / d -
                       (h * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getY() -
                             Settings::configuration.boards.find(i->first)->second.getCoordinates().getY())) / d +
                       Settings::configuration.boards.find(i->first)->second.getCoordinates().getX();
            //y = l(y2 - y1) / d -+ h(x2 - x1) / d + y1
            double y1 = (l * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getY() -
                              Settings::configuration.boards.find(i->first)->second.getCoordinates().getY())) / d -
                        (h * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getX() -
                              Settings::configuration.boards.find(i->first)->second.getCoordinates().getX())) / d +
                        Settings::configuration.boards.find(i->first)->second.getCoordinates().getY();
            double y2 = (l * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getY() -
                              Settings::configuration.boards.find(i->first)->second.getCoordinates().getY())) / d +
                        (h * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getX() -
                              Settings::configuration.boards.find(i->first)->second.getCoordinates().getX())) / d +
                        Settings::configuration.boards.find(i->first)->second.getCoordinates().getY();

            std::cout << d << " " << l << " " << h << std::endl;
            if (checkPoint(x1, y1, messageBoardsAndMetadata) && Point(x1,y1).isValid()) {
                listPossiblePoints.insert(Point(std::round(x1), std::round(y1)));
            }
            if (checkPoint(x2, y2, messageBoardsAndMetadata) && Point(x2,y2).isValid()) {
                listPossiblePoints.insert(Point(std::round(x2), std::round(y2)));
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
    return center;
}

double statistics::estimatedDistance(double rssi){
    double distance = std::pow(10, (ONE_METER_RSSI - rssi)/(10*2)) * 100;
    distance *= 1.1;// FIXME Make tolerance a variable
    std::cout << "Estimated distance: " << distance << std::endl; // TODO glog
    return distance ;
}

bool statistics::checkPoint(double x, double y,
                            std::unordered_map<std::string, followifier::ESP32Metadata> &boardsMedatada) {
    std::cout << "X: " << x << " Y:" << y << std::endl;
    for (auto & board : Settings::configuration.boards) {
        if ((std::pow(x - board.second.getCoordinates().getX(), 2) + std::pow(y - board.second.getCoordinates().getY(), 2) -
             std::pow(statistics::estimatedDistance(boardsMedatada.find(board.first)->second.rsi()), 2)) > 0)
            return false;
        std::cout << "OK" << std::endl;
    }
    std::cout << "YAY" << std::endl;
    return true;
}