#include <iostream>
#include <fstream>
#include "message.pb.h"

using namespace std;

void ListMessages(const followifier::Batch& batch) {
    for (int i = 0; i < batch.messages_size(); i++) {
        const followifier::ESP32Message& msg = batch.messages(i);

        cout << "MAC address: " << msg.mac() << endl;
        cout << "SSID: " << msg.ssid() << endl;
        cout << "HASH: " << msg.frame_hash() << endl;
        cout << "timestamp: " << msg.timestamp() << endl;
        cout << "rsi: " << msg.rsi() << endl;
    }
}

int main(int argc, char* argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " SAVED_BATCH_FILE" << endl;
        return -1;
    }
    followifier::Batch batch;

    {
        fstream input(argv[1], ios::in | ios::binary);
        if (!batch.ParseFromIstream(&input)) {
            cerr << "Failed to parse batch" << endl;
            return -1;
        }
    }

    ListMessages(batch);
    return 0;
}
