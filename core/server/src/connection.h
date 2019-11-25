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

    // TODO Documentation
    // TODO Refractor name
    void start();

    // TODO Documentation
    followifier::Batch sync_read();

    // TODO Documentation
    // TODO Refractor name
    void handle_read(const boost::system::error_code &, size_t);

    virtual ~connection();

private:

    connection(boost::asio::io_service &io_service);

    boost::asio::streambuf buf;

    tcp::socket socket_;

    std::string message_;

    const std::string delimiter = "\n\r\n\r";
};

#endif //CORE_CONNECTION_H
