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
        cout << "new connection established. Reading batch asynchronously... " << endl;
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        boost::asio::async_read_until(socket_, buf, delimiter,
                                      boost::bind(&connection::handle_read, shared_from_this(),
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred));
    } catch (const std::exception &e) {
        cerr << "failed to parse batch." << endl; // intentionally lowercase
    }
}

/**
 * Handles the asynchronous reception of data from a board.
 *
 * @param error result of the operation.
 * @param bytes_transferred the number of bytes in the `streambuf`'s get area up to including the delimiter. 0 in an error occurred.
 */
void connection::handle_read(const boost::system::error_code &error,
                             size_t bytes_transferred) {

    // If the batch has not been received correctly
    if (!error || bytes_transferred == 0) {
        cerr << "Something went wrong while receiving a batch." << endl << "Error: " << error.message() << endl
             << "Number of transferred bytes: " << bytes_transferred << endl;
    }
    else {

        followifier::Batch batch;
        database database;

        // Verify buf contains more data beyond the delimiter. (e.g.
        // async_read_until read beyond the delimiter)
        assert(buf.size() > bytes_transferred);

        // Extract up to the first delimiter.
        cout << endl << "Buffer size: " << buf.size() << ", transferred bytes: " << bytes_transferred
             << ", delimiter size: " << delimiter.size() << endl;
        std::string data{
                buffers_begin(buf.data()),
                buffers_begin(buf.data()) + bytes_transferred - delimiter.size()
        };

        // Consume through the first delimiter so that subsequent async_read_until
        // will not reiterate over the same data.
        buf.consume(bytes_transferred);

        if (!batch.ParseFromString(data)) {
            cerr << "failed to parse batch (length" << data.length() << ")." << endl; // intentionally lowercase
            return;
        }
        receiver::addBatch(batch, database);
    }
}

connection::connection(boost::asio::io_service &io_service) : socket_(io_service) {}