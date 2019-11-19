#include "statistics.h"
#include "settings.h"

Point statistics::getPosition(std::string hash, std::unordered_map< std::string, followifier::ESP32Metadata> &messageBoardsAndMetadata) {
    std::unordered_set<Point, Point::PointHasher> listPossiblePoints;
    for (auto i = messageBoardsAndMetadata.begin(); i != messageBoardsAndMetadata.end(); i++) {
        for (auto j = std::next(i, 1); j != messageBoardsAndMetadata.end(); j++) {
            //calculate interception points
            //d = sqrt((x1-x2)^2 + (y1-y2)^2)
            double d = std::sqrt(
                    std::pow(Settings::configuration.boards.find(i->first)->second.getCoordinates().getX() -
                             Settings::configuration.boards.find(j->first)->second.getCoordinates().getX(), 2) +
                    std::pow(Settings::configuration.boards.find(i->first)->second.getCoordinates().getY() -
                             Settings::configuration.boards.find(j->first)->second.getCoordinates().getY(), 2));

            //l = r1^2-r2^2 + d^2 / 2d
            double l = (std::pow(statistics::estimatedDistance(i->second.rsi()), 2) - std::pow(statistics::estimatedDistance(j->second.rsi()), 2) + std::pow(d, 2)) / (2 * d);

            //h = sqrt(r1^2 - l^2)
            double h = std::sqrt(std::pow(statistics::estimatedDistance(i->second.rsi()), 2) - std::pow(l, 2));
                 //x = l(x2 - x1) / d + -h(y2 - y1) / d + x1
            double x = (l * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getX() -
                             Settings::configuration.boards.find(i->first)->second.getCoordinates().getX())) / d +
                       (h * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getY() -
                             Settings::configuration.boards.find(i->first)->second.getCoordinates().getY())) / d +
                       Settings::configuration.boards.find(i->first)->second.getCoordinates().getX();
            //y = h(y2 - y1) / d -+ l(x2 - x1) / d + y1
            double y = (l * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getY() -
                             Settings::configuration.boards.find(i->first)->second.getCoordinates().getY())) / d -
                       (h * (Settings::configuration.boards.find(j->first)->second.getCoordinates().getX() -
                             Settings::configuration.boards.find(i->first)->second.getCoordinates().getX())) / d +
                       Settings::configuration.boards.find(i->first)->second.getCoordinates().getY();

            if (checkPoint(std::round(x), std::round(y), messageBoardsAndMetadata)) {
                listPossiblePoints.insert(Point(std::round(x), std::round(y)));
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
    return std::pow(10, (ONE_METER_RSSI - rssi)/10*2);
}

bool statistics::checkPoint(double x, double y,
                            std::unordered_map<std::string, followifier::ESP32Metadata> &boardsMedatada) {
    for (auto i = Settings::configuration.boards.begin(); i != Settings::configuration.boards.end(); i++) {
        if ((std::pow(x - i->second.getCoordinates().getX(), 2) + std::pow(y - i->second.getCoordinates().getY(), 2) -
             std::pow(statistics::estimatedDistance(boardsMedatada.find(i->first)->second.rsi()), 2)) > 0)
            return false;
    }
    return true;
}