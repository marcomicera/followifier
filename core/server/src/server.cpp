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
        start_calibration();
    }

    /* Start accepting connections from boards */
    start_statistics();
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
                                        const boost::system::error_code &error,
                                        const std::string &board_to_calibrate) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    std::cout << "Calibration: new connection." << std::endl;
    if (!error) {
        new_connection->async_batch_read_for_calibration(board_to_calibrate);
    }
}

void server::start_calibration() {

    /* For every board */
    short board_counter = 0;
    for (auto &board : Settings::configuration.boards) {

        /* This board's MAC address */
        std::string board_to_calibrate = board.first;

        /* Wait for the user to place this board */
        server::wait_placement(board_to_calibrate, board_counter++);

        /* Wait for a batch from this board */
        // TODO Do something if calibration fails for a board
        connection::pointer calibration_connection = connection::create(
                acceptor_.get_io_service()); // connection needed to calibrate this board
        acceptor_.async_accept(calibration_connection->socket(),
                               boost::bind(&::server::calibration_accept_handler, this, calibration_connection,
                                           boost::asio::placeholders::error, board_to_calibrate));

        /* Wait for the board to send the calibration batch */
        std::unique_lock<std::mutex> lk(calibration_connection->calibration_mutex);
        calibration_connection->calibration_batch_received.wait(lk, [calibration_connection] { return calibration_connection->get_board_calibration_status(); });
    }
    std::cout << std::endl;
}

server::~server() {
    acceptor_.close();
}
