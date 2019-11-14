#include <boost/asio.hpp>
#include "server.h"
#include "settings.h"

int main() {
    try {
        try{
            Settings::load("config.json");
        }catch(std::exception& e){
            std::cerr << "Invalid configuration file" << std::endl;
            return -1;
        }
        boost::asio::io_service io_service;
        server server(io_service); // TODO print server IP address
        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
