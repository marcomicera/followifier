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
        cout << "new connection established.\nReady to receive a new batch... " << std::flush;
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        boost::asio::async_read_until(socket_, buf, delimiter,
                                      boost::bind(&connection::handle_read, shared_from_this(),
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred));
        //        std::string data{
//                boost::asio::buffers_begin(buf.data()),
//                boost::asio::buffers_begin(buf.data()) + buf.size() - delimiter.size()
//        };
//        data = data.substr(0, data.size() - 1);
//        if (!batch.ParseFromString(data)) {
//            cerr << "failed to parse batch (length" << data.length() << ")." << endl; // intentionally lowercase
//            return;
//        }
//        receiver::addBatch(batch, database);
    } catch (const std::exception &e) {
        cerr << "failed to parse batch." << endl; // intentionally lowercase
    }
}

void connection::handle_read(const boost::system::error_code &error,
                             size_t bytes_transferred) {
    followifier::Batch batch;
    database database;
    // Verify buf contains more data beyond the delimiter. (e.g.
    // async_read_until read beyond the delimiter)
    cout << buf.data().size() << endl;
    cout << bytes_transferred << endl;
    // Extract up to the first delimiter.
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

connection::connection(boost::asio::io_service &io_service) : socket_(io_service) {}