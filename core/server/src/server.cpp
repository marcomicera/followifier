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

        /* Start calibration */
        start_calibration();
    }

    /* Start accepting connections from boards */
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
    int board_placements_seconds_left = Settings::configuration.calibration_duration_in_seconds.value();

    std::cout << "Please place device " << Settings::configuration.calibration_device_mac_address.value()
              << " at 1 meter distance from board " << board_mac << " (" << (board_counter + 1) << "/"
              << Settings::get_num_boards() << ")." << std::endl;

    /* `INITIALIZATION_BOARD_PLACEMENT_WAITING_TIME` seconds countdown starts now */
    while (board_placements_seconds_left > 0) {
        std::cout << "\r" << board_placements_seconds_left << " second"
                  << (board_placements_seconds_left > 1 ? "s" : "") << " left...  " << std::flush;
        sleep(1);
        --board_placements_seconds_left;
    }
    std::cout << "\rReady to calibrate board " << board_mac << ". Please do not move the device any further."
              << std::endl;
}

void server::compute_average_one_meter_rssi(const std::string &board_mac) {

    /* Connection creation and batch parsing may fail */
    try {

        /* Data structures for reading purposes */
        boost::system::error_code error; // connection may fail
        followifier::Batch batch; // calibration batch
        double average_rssi = 0;
        int number_of_messages_sent_by_calibration_device = 0; // calibration batch must include

        /* Read a batch synchronously until it is from the specified board */
        do {
            connection::pointer new_connection = connection::create(
                    acceptor_.get_io_service()); // connection needed to calibrate this board

            acceptor_.accept(new_connection->socket(), &error);
            if (!error) {
                batch = new_connection->sync_read();
            } else { // an error occurred
                std::cerr << "Error upon accepting a connection: retrying..." << std::endl;
            }

            /* Count the number of messages sent by the calibration device and also
             * compute the average RSSI in the meantime
             */
            for (const auto &message: batch.messages()) {

                /* Filtering messages that are not being sent by the calibration device */
                if (boost::iequals(message.metadata().devicemac(),
                                   Settings::configuration.calibration_device_mac_address.value())) {
                    ++number_of_messages_sent_by_calibration_device;
                    average_rssi += message.metadata().rssi();
                }
            }
            average_rssi /= batch.messages().size();

            /* Logging */
            if (boost::iequals(batch.boardmac(), board_mac) && number_of_messages_sent_by_calibration_device <
                                                               Settings::configuration.min_num_calibration_messages.value()) {
                std::cout << "Device " << Settings::configuration.calibration_device_mac_address.value() << " has sent "
                          << number_of_messages_sent_by_calibration_device << " calibration messages. At least "
                          << Settings::configuration.min_num_calibration_messages.value()
                          << " are required to compute a meaningful average. Waiting for another batch..." << std::endl;
            }

        } while (/* Case-insensitive comparison */
                !boost::iequals(batch.boardmac(), board_mac) ||

                /* There needs to be at least `Settings::configuration.min_num_calibration_messages` messages
                 * in the batch in order to compute a meaningful RSSI average
                 */
                number_of_messages_sent_by_calibration_device <
                Settings::configuration.min_num_calibration_messages);

        /* Store the average 1-meter-distance RSSI value */
        statistics::insert_one_meter_rssi(board_mac, average_rssi);
        std::cout << "Board " << board_mac << " detected an average RSSI of " << average_rssi << " from device "
                  << Settings::configuration.calibration_device_mac_address.value() << "." << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Failure while parsing a batch: " << e.what() << std::endl;
    }
}

void server::start_calibration() {

    /* For every board */
    short board_counter = 0;
    for (auto &board : Settings::configuration.boards) {

        /* This board's MAC address */
        std::string board_mac_address = board.first;

        /* Wait for the user to place this board */
        server::wait_positioning(board_mac_address, board_counter++);

        /* Wait for a batch from this board */
        // TODO Do something if calibration fails for a board
        server::compute_average_one_meter_rssi(board_mac_address);
    }
    std::cout << std::endl;
}

server::~server() {
    acceptor_.close();
}
