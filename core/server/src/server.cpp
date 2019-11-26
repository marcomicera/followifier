#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.h"
#include "statistics.h"
#include <boost/algorithm/string/predicate.hpp>

std::unordered_map<std::string, double> statistics::boards_one_meter_distance_rssi_values;

server::server(boost::asio::io_service &io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {

    /* Intro message displayed on server startup */
    std::cout << "Core server started on port " << acceptor_.local_endpoint().port()
              << ".\n\n"; // TODO print local address

    /* If a calibration device is set */
    if (Settings::configuration.calibration_device_mac_address) {
        /* Start calibrating boards one at the time */
        board_has_sent_calibration_batch = true;
        start_calibration();
    }else{
        /* Start accepting connections from boards */
        start_statistics();
    }
}

void server::statistics_accept_handler(const connection::pointer &new_connection,
                                       const boost::system::error_code &error) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    if (!error) {
        new_connection->async_batch_read_for_statistics();;
    }
    start_statistics();
}

void server::start_statistics() {
    connection::pointer new_connection = connection::create(acceptor_.get_io_service());
    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&::server::statistics_accept_handler, this, new_connection,
                                       boost::asio::placeholders::error));
}

void server::calibration_accept_handler(const connection::pointer &new_connection,
                                        const boost::system::error_code &error) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    if (!error) {
        new_connection->async_batch_read_for_calibration();
    }
    start_calibration();
}

void server::start_calibration() {

    /* For every board */
    short board_counter = 0;
    for (auto &board : Settings::configuration.boards) {

        /* This board's MAC address */
        Settings::board_to_calibrate = board.first;

        if (!statistics::has_been_calibrated(Settings::board_to_calibrate)) {
            /* Wait for the user to place this board */
            if(board_has_sent_calibration_batch){
                calibration::wait_placement(Settings::board_to_calibrate, board_counter++);
                board_has_sent_calibration_batch = false;
            }

            /* Wait for the board to send the calibration batch */
            connection::pointer calibration_connection = connection::create(
                    acceptor_.get_io_service()); // connection needed to calibrate this board
            acceptor_.async_accept(calibration_connection->socket(),
                                   boost::bind(&::server::calibration_accept_handler, this, calibration_connection,
                                               boost::asio::placeholders::error));
            return;
        }
        board_counter++;
    }
    std::cout << "Calibration has been completed. Statistics may not be calculated right away" << std::endl;
    /* All boards have been calibrated */
    start_statistics();
}

server::~server() {
    acceptor_.close();
}
