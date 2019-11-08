//
// Created by simona on 22/09/19.
//

#ifndef CORE_SERVER_H
#define CORE_SERVER_H


#include <boost/asio.hpp>
#include "connection.h"
#include "database.h"


class server {

    const size_t port = 12345;

public:
    explicit server(boost::asio::io_service&);

private:
    void start_accept();

    void handle_accept(const connection::pointer&,
                       const boost::system::error_code &);

    tcp::acceptor acceptor_;
};


#endif //CORE_SERVER_H
