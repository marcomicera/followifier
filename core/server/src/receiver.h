//
// Created by simona on 21/09/19.
//

#ifndef CORE_RECEIVER_H
#define CORE_RECEIVER_H

#include <string>
#include <gen/message.pb.h>

#define NUMBER_BOARDS 1
class receiver {
public:
    static void addBatch(const followifier::Batch& batch);
private:
    static bool isContained(const std::string& mac, const std::string& hash);
    static std::mutex m;
    static std::map<std::string, followifier::Batch> batches;
 };

#endif //CORE_RECEIVER_H
