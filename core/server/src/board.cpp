#include "board.h"
#include <regex>

bool Board::operator==(const Board &other) const{
    return this->coordinates == other.coordinates &&
        this->mac == other.mac;
}

std::string Board::getMac() {
    return this->mac;
}

Point Board::getCoordinates() {
    return this->coordinates;
}

Board::Board(const std::string &mac, const Point &coordinates) : coordinates(coordinates) {
    std::smatch m;
    std::regex e ("(([0-9a-fA-F]{2}):){5}([0-9a-fA-F]{2})");   // mac format
    std::regex_match(mac, m, e);
    if(m.size()!=0) {
        this->mac = mac;
    } else {
        throw std::invalid_argument("Invalid MAC address: " + mac);
    }
}
