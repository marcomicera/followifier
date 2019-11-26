#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio.hpp>
#include <gen/message.pb.h>
#include "settings.h"
#include "receiver.h"
#include "statistics.h"

using boost::asio::ip::tcp;

#ifndef CORE_CONNECTION_H
#define CORE_CONNECTION_H

class connection : public boost::enable_shared_from_this<connection> {

public:

    typedef boost::shared_ptr<connection> pointer;

    static pointer create(boost::asio::io_service &io_service);

    tcp::socket &socket();

    virtual ~connection();

    void async_batch_read_for_statistics() {
        async_read(&connection::statistics_packet_handler, "statistics", "");
    }

    void async_batch_read_for_calibration(const std::string &board_to_calibrate) {
        async_read(&connection::calibration_packet_handler, "calibration", board_to_calibrate);
    }

    bool get_board_calibration_status() { return board_has_sent_calibration_batch; }

    // TODO Documentation
    std::condition_variable calibration_batch_received;
    std::mutex calibration_mutex;

protected:

    typedef void(connection::*packet_handler_t)(const followifier::Batch &, const std::string &);

    connection(boost::asio::io_service &io_service);

    boost::asio::streambuf buf;

    tcp::socket socket_;

    const std::string delimiter = "\n\r\n\r";

    // TODO Documentation
    bool board_has_sent_calibration_batch = false;

    // TODO Documentation
    void statistics_packet_handler(const followifier::Batch &batch, const std::string &board_to_calibrate) {
        database database;
        if (Settings::configuration.boards.find(batch.boardmac()) == Settings::configuration.boards.end()) {
            // cerr << "Unknown source" << endl;
            return;
        }
        receiver::addBatch(batch, database);
    }

    // TODO Documentation
    void calibration_packet_handler(const followifier::Batch &batch, const std::string &board_to_calibrate) {

        std::cout << "Calibration packet handler started." << std::endl;

        /* Data structures for reading purposes */
        boost::system::error_code error; // connection may fail
        double average_rssi = 0;
        int number_of_messages_sent_by_calibration_device = 0;

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

        /* If this batch is not suitable for the calibration */
        if (    /* Case-insensitive comparison */
                !boost::iequals(batch.boardmac(), board_to_calibrate) ||

                /* There needs to be at least `Settings::configuration.min_num_calibration_messages` messages
                   in the batch in order to compute a meaningful RSSI average */
                number_of_messages_sent_by_calibration_device <
                Settings::configuration.min_num_calibration_messages) {

            /* Logging */
            std::cout << "Bad calibration batch. Sender: " << batch.boardmac() << ", (expected: " << board_to_calibrate
                      << "), number of messages sent by calibration device: "
                      << number_of_messages_sent_by_calibration_device << " (at least "
                      << Settings::configuration.min_num_calibration_messages
                      << (Settings::configuration.min_num_calibration_messages == 1 ? "is" : "are") << " required). "
                      << std::endl;
            async_batch_read_for_calibration(board_to_calibrate);

        } else { // batch was good

            /* Logging */
            std::cout << "Device " << Settings::configuration.calibration_device_mac_address.value() << " has sent "
                      << number_of_messages_sent_by_calibration_device << " calibration messages. At least "
                      << Settings::configuration.min_num_calibration_messages.value()
                      << " are required to compute a meaningful average. Waiting for another batch..." << std::endl;

            /* Store the average 1-meter-distance RSSI value */
            statistics::insert_one_meter_rssi(board_to_calibrate, average_rssi);
            std::cout << "Board " << board_to_calibrate << " detected an average RSSI of " << average_rssi
                      << " from device "
                      << Settings::configuration.calibration_device_mac_address.value() << "." << std::endl;

            /* Waking up the server thread */
            board_has_sent_calibration_batch = true;
            calibration_batch_received.notify_one();
        }
    }

    // TODO Documentation
    void async_read(packet_handler_t packet_handler,
                    const std::string &operation_message,
                    const std::string &board_to_calibrate) {

        boost::asio::async_read_until(socket_, buf, delimiter,
                                      boost::bind(&connection::async_packet_handler_wrapper, shared_from_this(),
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred,
                                                  packet_handler,
                                                  operation_message,
                                                  board_to_calibrate));
    }

    // TODO Documentation
    void async_packet_handler_wrapper(const boost::system::error_code &error,
                                      size_t transferred_bytes,
                                      packet_handler_t packet_handler,
                                      const std::string &operation,
                                      const std::string &board_to_calibrate) {

        if (error) { // an error occurred
            throw std::runtime_error("An error occurred during an asynchronous read in the " + operation +
                                     " operation failed: (error code " + std::to_string(error.value()) + " " +
                                     error.message() + ", number of transferred bytes: " +
                                     std::to_string(transferred_bytes) + ")");
        } else if (transferred_bytes == 0) { // no bytes have been transferred
            throw std::runtime_error("An asynchronous read performed during the " + operation +
                                     " operation returned no bytes at all: (error code " +
                                     std::to_string(error.value()) + " " + error.message() +
                                     ", number of transferred bytes: " + std::to_string(transferred_bytes) + ")");
        } else if (buf.size() < transferred_bytes) { // buf contains more data beyond the delimiter
            throw std::runtime_error("Buffer-related error during the " + operation + "operation: buffer size is " +
                                     std::to_string(buf.size()) + ", while " + std::to_string(transferred_bytes) +
                                     " bytes have been transferred");
        } else {

            std::cout << "Async packet handler wrapper started." << std::endl;

            /* This wrapper reads a batch */
            followifier::Batch batch;

            /* Extract up to the first delimiter */
            std::string data{
                    buffers_begin(buf.data()),
                    buffers_begin(buf.data()) + transferred_bytes - delimiter.size()
            };

            /* Consume through the first delimiter so that subsequent async_read_until will not reiterate over the same data. */
            buf.consume(transferred_bytes);

            /* Parsing the just-received batch */
            if (!batch.ParseFromString(data)) {
                throw std::runtime_error("Couldn't parse a batch of size " + std::to_string(data.length()) + ".");
            } else {
                std::cout << "Batch parsed successfully." << std::endl;
            }

            /* Calling the actual packet handler */
            (*this.*packet_handler)(batch, board_to_calibrate);
        }
    }
};

#endif //CORE_CONNECTION_H
