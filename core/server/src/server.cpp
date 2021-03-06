#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.h"
#include "debug.h"


server::server(boost::asio::io_service &io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    std::cout << "Core server started on port " << acceptor_.local_endpoint().port() << ".\n\n"; // TODO print local address
    start_accept();
}

void server::start_accept() {
    connection::pointer new_connection = connection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&::server::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
}

void server::handle_accept(const connection::pointer &new_connection,
                           const boost::system::error_code &error) {
    
    if (DEBUG) {
        std::cout << "New connection received" << std::endl;
    }

    if (!error) {
        new_connection->start();
    }

    start_accept();
}
