#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include "../gen/message.pb.h"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::fstream;
using std::istream;
using std::ios;

const string delimiter = "\n\r\n\r";

void ListMessages(const followifier::Batch &batch) {
    for (int i = 0; i < batch.messages_size(); i++) {
        const followifier::ESP32Message &msg = batch.messages(i);

        cout << "MAC address: " << msg.mac() << endl;
        cout << "SSID: " << msg.ssid() << endl;
        cout << "HASH: " << msg.frame_hash() << endl;
        cout << "timestamp: " << msg.timestamp() << endl;
        cout << "rsi: " << msg.rsi() << endl;
    }
}

int main(int argc, char *argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    boost::asio::io_service io_service;
    boost::asio::streambuf buf;
    followifier::Batch batch;

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " SAVED_BATCH_FILE" << endl;
        return -1;
    }

    tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234));
    tcp::socket socket_(io_service);
    acceptor_.accept(socket_);

    boost::asio::read_until(socket_, buf, delimiter);
    string data{
            boost::asio::buffers_begin(buf.data()),
            boost::asio::buffers_begin(buf.data()) + buf.size() - delimiter.size()
    };
    if (!batch.ParseFromString(data)) {
        cerr << "Failed to parse batch" << endl;
        return -1;
    }

    ListMessages(batch);
    return 0;
}
