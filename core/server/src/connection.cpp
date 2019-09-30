#include <boost/enable_shared_from_this.hpp>
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
    cout << "start connection" <<endl;
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    boost::asio::streambuf buf;
    followifier::Batch batch;
    boost::asio::read_until(socket_, buf, delimiter);
    std::string data{
            boost::asio::buffers_begin(buf.data()),
            boost::asio::buffers_begin(buf.data()) + buf.size() - delimiter.size()
    };
    cout << data << endl;
    data = data.substr(0, data.size()-1);
    if (!batch.ParseFromString(data)){
        cerr << "Failed to parse batch" << endl;
        return;
    }

    receiver::addBatch(batch);
}

connection::connection(boost::asio::io_service &io_service) : socket_(io_service) {}