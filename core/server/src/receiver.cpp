#include <src/connection.h>
#include "receiver.h"

using std::cout;
using std::endl;

std::mutex receiver::m;
std::unordered_set<followifier::Batch, receiver::BatchHasher, receiver::BatchEqualFn> receiver::batchesBuffer;

bool receiver::batchContainsMessage(const followifier::Batch &batch, const followifier::ESP32Message &message) {

    for (const auto &batchMessage: batch.messages()) {

        /* Two messages are equal if the Access Point's MAC address are the same, and they have been
         * sent at the same time by the mobile device
         */
        if (batchMessage.apmac() == message.apmac() && batchMessage.timestamp() == message.timestamp()) {
            return true;
        }
    }

    return false;
}

void receiver::addBatch(const followifier::Batch &newBatch) {

    /* Critical section */
    std::lock_guard<std::mutex> lockGuard(m);

    cout << "Received batch from " + newBatch.boardmac() + " of size " + std::to_string(newBatch.messages_size())
         << "." << endl;

    /* Source MAC address appears for the first time */
    if (batchesBuffer.find(newBatch) == batchesBuffer.end()) {
        /* Insert batch into local buffer */
        batchesBuffer.insert(newBatch);
    } else {
        /* New timeslot, need to clear the batches buffer */
        batchesBuffer.clear();
        batchesBuffer.insert(newBatch);
    }

    /* Server has received batches from all boards.
     * This means this 'round' (timeslot) is about to finish and it is time
     * to discard messages that have not been sent by all boards.
     */
    if (batchesBuffer.size() == NUMBER_BOARDS) {

        /* Batches comparator */
        receiver::BatchEqualFn batchEqualFn;

        /* Checking whether all messages have been sent by all the other boards */
        for (const followifier::ESP32Message &newMessage : newBatch.messages()) {

            bool messageHasBeenSentByAllBoards = true;

            for (auto &otherBatch : batchesBuffer) {

                /* Skipping the current batch and testing whether the other batch contains the current newMessage */
                if (!batchEqualFn(otherBatch, newBatch) && !batchContainsMessage(otherBatch, newMessage)) {
                    messageHasBeenSentByAllBoards = false;
                    break;
                }
            }

            if (messageHasBeenSentByAllBoards) {

                /* TODO Add newMessage to the database */
                cout << "Message <" << newMessage.apmac() << ", " << newMessage.timestamp()
                     << "> has been sent by all boards." << endl;

                /* TODO Remove it from the batches buffer */
                batchesBuffer.clear(); // FIXME
            }
        }
        cout << "Checked all messages of this timeslot" << endl;
    }
}