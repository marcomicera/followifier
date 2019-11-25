#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include "connection.h"
#include <string>
#include <google/protobuf/stubs/common.h>
#include <gen/message.pb.h>
#include "receiver.h"

using std::cout;
using std::cerr;
using std::endl;

connection::pointer connection::create(boost::asio::io_service &io_service) {
    return connection::pointer(new connection(io_service));
}

tcp::socket &connection::socket() {
    return socket_;
}

void connection::start() {
    try {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        boost::asio::async_read_until(socket_, buf, delimiter,
                                      boost::bind(&connection::handle_read, shared_from_this(),
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred));
    } catch (const std::exception &e) {
        cerr << "Failure while parsing a batch: " << e.what() << endl;
    }
}

followifier::Batch connection::sync_read() {

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    boost::system::error_code error_code;
    size_t bytes_transferred = boost::asio::read_until(socket_, buf, delimiter, error_code);
    if (!error_code && bytes_transferred != 0) {

        // FIXME Duplicate code
        followifier::Batch batch;

        /* Verify buf contains more data beyond the delimiter. (e.g. async_read_until read beyond the delimiter) */
        if (buf.size() < bytes_transferred) {
            cerr << "Buffer size is " << buf.size() << ", while " << bytes_transferred
                 << " bytes have been transferred. Terminating..." << endl;
            exit(1);
        }

        /* Extract up to the first delimiter */
        std::string data{
                buffers_begin(buf.data()),
                buffers_begin(buf.data()) + bytes_transferred - delimiter.size()
        };

        /* Consume through the first delimiter so that subsequent async_read_until will not reiterate over the same data. */
        buf.consume(bytes_transferred);

        if (!batch.ParseFromString(data)) {
            throw std::runtime_error("Couldn't parse a batch of size " + std::to_string(data.length()) + ".");
        }

        return batch;

    } else { // an error occurred
        throw std::runtime_error(
                "Something went wrong while receiving a calibration batch: (error code " +
                std::to_string(error_code.value()) + " " + error_code.message() + ", number of transferred bytes: " +
                std::to_string(bytes_transferred) + ")");
    }
}

/**
 * Handles the asynchronous reception of data from a board.
 *
 * @param error                 result of the operation.
 * @param bytes_transferred     the number of bytes in the `streambuf`'s get area up to including the delimiter.
 *                              0 in an error occurred.
 */
void connection::handle_read(const boost::system::error_code &error,
                             size_t bytes_transferred) {

    /* If the batch has not been received correctly */
    if (bytes_transferred == 0) {
        cerr << "Something went wrong while receiving a calibration batch: (error code " << error.value() << " "
             << error.message() << ", number of transferred bytes: " << bytes_transferred << ")." << endl << endl;
    } else {

        followifier::Batch batch;
        database database;

        /* Verify buf contains more data beyond the delimiter. (e.g. async_read_until read beyond the delimiter) */
        if (buf.size() < bytes_transferred) {
            cerr << "Buffer size is " << buf.size() << ", while " << bytes_transferred
                 << " bytes have been transferred. Terminating..." << endl;
            exit(1);
        }

        /* Extract up to the first delimiter */
        std::string data{
                buffers_begin(buf.data()),
                buffers_begin(buf.data()) + bytes_transferred - delimiter.size()
        };

        /* Consume through the first delimiter so that subsequent async_read_until will not reiterate over the same data. */
        buf.consume(bytes_transferred);

        if (!batch.ParseFromString(data)) {
            cerr << "Couldn't parse a batch of size " << data.length() << "." << endl;
            return;
        }
        if (Settings::configuration.boards.find(batch.boardmac()) == Settings::configuration.boards.end()) {
            // cerr << "Unknown source" << endl;
            return;
        }
        receiver::addBatch(batch, database);
    }
}

connection::connection(boost::asio::io_service &io_service) : socket_(io_service) {}

connection::~connection() {
    // TODO Is there something else to take care of?
    socket_.close();
}
