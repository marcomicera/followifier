#include <src/connection.h>
#include "receiver.h"

using std::cout;
using std::endl;

std::mutex receiver::m;
std::map<std::string,followifier::Batch> receiver::batches;

bool receiver::isContained(const followifier::Batch &batch, std::string hash) {
    for (int i = 0; i < batch.messages_size(); i++) {
        const followifier::ESP32Message &msg = batch.messages(i);
        if(msg.frame_hash().compare(hash)==0){
            return true;
        }
    }
    return false;
}

void receiver::fillDatabase(std::vector<followifier::Batch> batch){
    for(int i=0; i<batch[0].messages_size(); i++){
        const followifier::ESP32Message &msg = batch[0].messages(i);
        bool check = true;
        for(auto j = batch.begin() + 1; j!=batch.end(); ++j){
            check = check and isContained(*j, msg.frame_hash());
        }
        if(check){
            std::cout << "added " + msg.frame_hash()<< std::endl;
            //addToDatabase
        }else{
            std::cout << "not added " + msg.frame_hash() << std::endl;
        }
    }
}

void receiver::addBatch(followifier::Batch batch, std::string address) {
    m.lock();
    cout << "received batch from " + address + " with size" + std::to_string(batch.messages_size()) << endl;
    if (batches.find(address) != batches.end()) {
        //some board data got lost, resetting everything
        cout << "some packets were lost, resetting" << endl;
        batches.clear();
    }
    batches.insert(std::pair<std::string, followifier::Batch>(address, batch));
    cout << "batches size " + std::to_string(batches.size()) << endl;
    if (batches.size() == number_boards) {
        cout << "data received from all boards" << endl;
        //data received from all boards
        std::vector<followifier::Batch> bs;
        for (auto i = batches.begin(); i != batches.end(); i++) {
            bs.push_back(i->second);
        }
        receiver::fillDatabase(bs);
        //clean batches
        batches.clear();
    }
    m.unlock();
}
