#include <boost/asio.hpp>
#include "server/server.h"

int main()
{
    try
    {
        boost::asio::io_service io_service;
        server server(io_service);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}