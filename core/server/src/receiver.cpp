#include <src/connection.h>
#include "receiver.h"

using std::cout;
using std::endl;

std::mutex receiver::m;
std::map<std::string, std::vector<followifier::Batch>> receiver::batches;

bool receiver::isContained(const std::string &mac, std::string hash) {
    for (const followifier::Batch &batch : batches.at(mac)) {
        for (int i = 0; i < batch.messages_size(); i++) {
            const followifier::ESP32Message &msg = batch.messages(i);
            if (msg.frame_hash() == hash) {
                return true;
            }
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
        std::vector<followifier::Batch> list;
        followifier::Batch empty;
        for (int i = 0; i <= 3; i++) {
            //store batch from 4 timeslots
            list.push_back(empty);
        }
        list.push_back(batch);
        batches.insert(std::pair<std::string, std::vector<followifier::Batch>>(mac, list));
    } else {
        //new timeslot
        batches.at(mac).erase(batches.at(mac).begin(), batches.at(mac).begin());
        batches.at(mac).push_back(batch);
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
                removeMessage(message.frame_hash());
            }
        }
    }
    m.unlock();
}

void receiver::removeMessage(const std::string &hash) {
    for (auto &pairs : batches) {
        removeMessageFromList(hash, pairs.second);
    }
}

void receiver::removeMessageFromList(const std::string &hash, std::vector<followifier::Batch> values) {
    for (auto &list : values) {
        for (int i = 0; i < list.messages_size(); i++) {
            if (list.messages(i).frame_hash() == hash) {
                list.mutable_messages()->DeleteSubrange(i, 1);
                return;
            }
        }
    }
}


