#include "board.h"

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
    this->mac = mac;
}

Point Board::getCoordinates() {
    return this->coordinates;
}

void Board::setCoordintes(Point coordinates) {
    this->coordinates = coordinates;
}