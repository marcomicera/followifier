#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.h"

server::server(boost::asio::io_service &io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    std::cout << "Core server started on " << acceptor_.local_endpoint().address() << ":"
              << acceptor_.local_endpoint().port() << ".\n\n";
    start_accept();
}

void server::start_accept() {
    std::cout << "Ready to accept a new connection." << std::endl;
    connection::pointer new_connection = connection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&::server::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
}

void server::handle_accept(const connection::pointer &new_connection,
                           const boost::system::error_code &error) {
    if (!error) {
        std::cout << "Establishing new connection... ";
        new_connection->start();
    }

    start_accept();
}