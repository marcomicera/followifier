#include <boost/asio.hpp>
#include "server.h"

int main() {
    try {
        boost::asio::io_service io_service;
        server server(io_service); // TODO print server IP address
        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
