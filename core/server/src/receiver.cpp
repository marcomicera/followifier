#include <src/connection.h>
#include "receiver.h"

using std::cout;
using std::endl;

std::mutex receiver::m;
std::map<std::string, followifier::Batch> receiver::batches;

bool receiver::isContained(const std::string &mac, const std::string& hash) {
    for (int i = 0; i < batches.at(mac).messages_size(); i++) {
        const followifier::ESP32Message &msg = batches.at(mac).messages(i);
        if (msg.frame_hash() == hash) {
            return true;
        }
    }
    return false;
}

void receiver::addBatch(const followifier::Batch &batch) {
    m.lock();
    std::string mac = batch.mac();
    cout << "received batch from " + mac + " with size " + std::to_string(batch.messages_size()) << endl;
    if (batches.find(mac) == batches.end()) {
        //first time inserting a batch for this mac
        batches.insert(std::pair<std::string, followifier::Batch>(mac, batch));
    } else {
        //new timeslot
        batches.clear();
        batches.insert(std::pair<std::string, followifier::Batch>(mac, batch));
    }

    //check batches
    if (batches.size() == NUMBER_BOARDS) {
        for (const followifier::ESP32Message &message : batch.messages()) {
            bool check = true;
            for (auto &batche : batches) {
                if (!isContained(batche.first, message.frame_hash())) {
                    check = false;
                    break;
                }
            }
            if (check) {
                //add to db
                cout << "added to db message with hash " << message.frame_hash() << endl;
                //remove from stored batches
            }
        }
    }
    m.unlock();
}