#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.h"

server::server(boost::asio::io_service &io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    std::cout << "Core server started on port " << acceptor_.local_endpoint().port()
              << ".\n\n"; // TODO print local address
    start_calibration();
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
    if (!error) {
        new_connection->start();
    }

    start_accept();
}

void server::wait_positioning(const std::string &board_mac, short board_counter) {

    /* How many seconds are left for the user so s/he can place the board at 1 meter distance */
    short board_placements_seconds_left = INITIALIZATION_BOARD_PLACEMENT_WAITING_TIME;

    std::cout << "Please place board " << board_mac << " (" << (board_counter + 1) << "/"
              << Settings::get_num_boards() << ") at 1 meter distance from the server." << std::endl;

    /* `INITIALIZATION_BOARD_PLACEMENT_WAITING_TIME` seconds countdown starts now */
    while (board_placements_seconds_left > 0) {
        std::cout << "\r" << board_placements_seconds_left << " second"
                  << (board_placements_seconds_left > 1 ? "s" : "") << " left...  " << std::flush;
        sleep(1);
        --board_placements_seconds_left;
    }
    std::cout << "\rReady to calibrate board " << board_mac << ". Please do not move it anymore." << std::endl;
}

void server::start_calibration() {

    /* For every board */
    short board_counter = 0;
    for (auto &board : Settings::configuration.boards) {

        /* Wait for the user to place this board */
        server::wait_positioning(board.first, board_counter++);
    }
    std::cout << std::endl;
}
