//
// Created by simona on 21/09/19.
//

#ifndef CORE_RECEIVER_H
#define CORE_RECEIVER_H

#include <string>
#include <gen/message.pb.h>

class receiver {
public:
    static void fillDatabase(std::vector<followifier::Batch> batches);
    static std::map<std::string, followifier::Batch> batches;
    static void addBatch(followifier::Batch batch, std::string);
private:
    static bool isContained(const followifier::Batch &batch, std::string hash);
    static std::mutex m;
};

#endif //CORE_RECEIVER_H
