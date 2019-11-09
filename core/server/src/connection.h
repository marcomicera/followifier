#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <gen/message.pb.h>

using boost::asio::ip::tcp;

#ifndef CORE_CONNECTION_H
#define CORE_CONNECTION_H

class connection : public boost::enable_shared_from_this<connection> {
public:
    typedef boost::shared_ptr<connection> pointer;

    static pointer create(boost::asio::io_service &io_service);

    tcp::socket &socket();

    void start();

private:
    connection(boost::asio::io_service& io_service);

    tcp::socket socket_;
    std::string message_;
};

#endif //CORE_CONNECTION_H
