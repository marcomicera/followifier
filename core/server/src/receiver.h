//
// Created by simona on 21/09/19.
//

#ifndef CORE_RECEIVER_H
#define CORE_RECEIVER_H

#include <string>
#include <gen/message.pb.h>

#define NUMBER_BOARDS 1

/**
 * Receives and filters batches from all boards.
 */
class receiver {

public:

    /**
     * Adds a new batch to the buffer.
     *
     * @param newBatch the just-received batch to be added to the buffer.
     */
    static void addBatch(const followifier::Batch &newBatch);

    class BatchHasher {
    public:
        inline size_t operator()(const followifier::Batch &batch) const {
            return ((std::hash<std::string>()(batch.boardmac())));
        }
    };

    class BatchEqualFn {
    public:
        inline bool operator()(const followifier::Batch &b1, const followifier::Batch &b2) const {
            return b1.boardmac() == b2.boardmac();
        }
    };

private:

    /**
     * Checks whether a batch contains the specified message or not.
     *
     * @param batch     the batch against which the check must be performed.
     * @param message   the message against which the check must be performed.
     * @return          true in case the specified message is contained in the
     *                  specified batch, false otherwise.
     */
    static bool batchContainsMessage(const followifier::Batch &batch, const followifier::ESP32Message &message);

    /**
     * Batches must be added in an interruptable fashion.
     */
    static std::mutex m;

    /**
     * Batches buffer mapping board source MAC addresses to the
     * batch of messages they have sent during the last timeslot.
     */
    static std::unordered_set<followifier::Batch, BatchHasher, BatchEqualFn> batchesBuffer;
};

#endif //CORE_RECEIVER_H
