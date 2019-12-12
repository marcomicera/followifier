//
// Created by simona on 22/09/19.
//

#ifndef CORE_SERVER_H
#define CORE_SERVER_H

#include <boost/asio.hpp>
#include "connection.h"
#include "database.h"
#include "settings.h"
#include "calibration.h"

class server {

    const size_t port = Settings::configuration.port;

public:

    explicit server(boost::asio::io_service &);

    virtual ~server();

    /**
 * Begins the calibration phase, during which the user is asked
 * to place boards at 1 meter from the core server, one at the time.
 */
protected:

    void start_statistics();

    void statistics_accept_handler(const connection::pointer &new_connection,
                                   const boost::system::error_code &error);

    tcp::acceptor acceptor_;

    void calibration_accept_handler(const connection::pointer &new_connection, const boost::system::error_code &error);

    void start_calibration();

};


#endif //CORE_SERVER_H
