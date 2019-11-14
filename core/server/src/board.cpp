#include "board.h"
#include <regex>

bool Board::operator==(const Board &other) const{
    return this->coordinates == other.coordinates &&
        this->mac == other.mac;
}
size_t Board::operator()(const Board &b) const {
    return std::hash<std::string>()(b.mac);
}

std::string Board::getMac() {
    return this->mac;
}

void Board::setMac(std::string mac) {
    std::smatch m;
    std::regex e ("(([0-0a-fA-F]{2}):){5}([0-0a-fA-F]{2})");   // mac format
    std::regex_match(mac, m, e);
    if(m.size()!=0) {
        this->mac = mac;
    } else {
        throw std::invalid_argument("Invalid MAC address: " + mac);
    }
}

Point Board::getCoordinates() {
    return this->coordinates;
}

void Board::setCoordintes(Point coordinates) {
    this->coordinates = coordinates;
}