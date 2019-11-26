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

connection::connection(boost::asio::io_service &io_service) : socket_(io_service) {}

connection::~connection() {
    // TODO Is there something else to take care of?
    socket_.close();
}
