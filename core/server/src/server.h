//
// Created by simona on 22/09/19.
//

#ifndef CORE_SERVER_H
#define CORE_SERVER_H

#include <boost/asio.hpp>
#include "connection.h"
#include "database.h"
#include "settings.h"

/**
 * How many seconds does the user have to place boards at 1 meter
 * distance from this server, one at the time.
 */
// TODO Make it a configuration parameter
#define INITIALIZATION_BOARD_PLACEMENT_WAITING_TIME 10

class server {

    const size_t port = Settings::configuration.port;

public:

    explicit server(boost::asio::io_service &);

    virtual ~server();

protected:

    /**
     * Begins the calibration phase, during which the user is asked
     * to place boards at 1 meter from the core server, one at the time.
     */
    void start_calibration();

    void start_accept();

    void handle_accept(const connection::pointer &,
                       const boost::system::error_code &);

    tcp::acceptor acceptor_;

private:

    /**
     * Waits for the user to place the specified board at 1 meter distance from the server.
     *
     * @param board_mac         MAC address of the board to be placed.
     * @param board_counter     indicates how many boards have completed this process (printing purposes only).
     */
    static void wait_positioning(const std::string& board_mac, short board_counter);

    /**
     * Waits for a "calibration batch" and computes the average 1-meter-distance RSSI based on messages contained
     * in it. Messages in this batch will be then discarded.
     *
     * @param board_mac         MAC address of the board being calibrated.
     */
    void compute_average_one_meter_rssi(const std::string& board_mac);
};


#endif //CORE_SERVER_H
