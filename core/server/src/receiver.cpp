#include <src/connection.h>
#include "receiver.h"

using std::cout;
using std::endl;

std::mutex receiver::m;
std::map<std::string, followifier::Batch> receiver::batches;

bool receiver::isContained(const std::string &mac, const std::string &hash) {
    for (int i = 0; i < batches.at(mac).messages_size(); i++) {
        const followifier::ESP32Message &msg = batches.at(mac).messages(i);
        if (msg.frame_hash() == hash) {
            return true;
        }
    }
    return false;
}

void receiver::addBatch(const followifier::Batch &newBatch) {

    m.lock();
    std::string newBatchSrcAddress = newBatch.boardmac();
    cout << "Received batch from " + newBatchSrcAddress + " of size " + std::to_string(newBatch.messages_size()) << "." << endl;

    /* Source MAC address appears for the first time */
    if (batches.find(newBatchSrcAddress) == batches.end()) {
        batches.insert(std::pair<std::string, followifier::Batch>(newBatchSrcAddress, newBatch));
    } else {
        /* New timeslot, need to clear all previous batches */
        batches.clear();
        batches.insert(std::pair<std::string, followifier::Batch>(newBatchSrcAddress, newBatch));
    }

    /* Server has received batches from all boards */
    if (batches.size() == NUMBER_BOARDS) {
        for (const followifier::ESP32Message &message : newBatch.messages()) {
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