#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio.hpp>
#include <gen/message.pb.h>
#include "settings.h"
#include "receiver.h"
#include "statistics.h"
#include "calibration.h"

using boost::asio::ip::tcp;

#ifndef CORE_CONNECTION_H
#define CORE_CONNECTION_H


class connection : public boost::enable_shared_from_this<connection> {

public:


    typedef boost::shared_ptr<connection> pointer;

    static pointer create(boost::asio::io_service &io_service);

    tcp::socket &socket();

    virtual ~connection();

    /**
     * Async read of socket for statitic purposes
     */
    void async_batch_read_for_statistics();

    /**
     * Async read of socket for calibration purposes
     */
    void async_batch_read_for_calibration();

    // TODO Documentation

protected:

    typedef void(connection::*packet_handler_t)(const followifier::Batch &);

    connection(boost::asio::io_service &io_service);

    boost::asio::streambuf buf;

    tcp::socket socket_;

    const std::string delimiter = "\n\r\n\r";


    /**
     * Handles the batch received during the statistics step
     *
     * @param batch
     */
    void statistics_packet_handler(const followifier::Batch &batch);


    /**
     * Handles the batch received during the calibration step
     *
     * @param batch
     */
    void calibration_packet_handler(const followifier::Batch &batch);

    // TODO Documentation
    void async_read(packet_handler_t packet_handler,
                    const std::string &operation_message);

    // TODO Documentation
    void async_packet_handler_wrapper(const boost::system::error_code &error,
                                      size_t transferred_bytes,
                                      packet_handler_t packet_handler,
                                      const std::string &operation);

};

#endif //CORE_CONNECTION_H
