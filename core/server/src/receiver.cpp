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

    // Printing received messages
    cout << "new batch received from " + newBatch.boardmac() + " of size " + // intentionally lowercase
            std::to_string(newBatch.messages_size());
    if (newBatch.messages_size() > 0) { // if there is at least one message in it
        cout << ":" << endl;
        int messageCounter = 1;
        for (const auto &newMessage : newBatch.messages()) { // print all messages
            cout << messageCounter++ << ")\t<" << newMessage.apmac() << ", " << newMessage.timestamp() << ">" << endl;
        }
    } else {
        cout << ".";
    }
    cout << endl << endl;

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

            /* Store message only if it has been sent by all boards */
            if (messageHasBeenSentByAllBoards) {

                cout << "Message <" << newMessage.apmac() << ", " << newMessage.timestamp()
                     << "> has been sent by all boards." << endl;

                // TODO Add newMessage to the database
            }
            else { // message has not been sent by all boards: dropping it
                cout << "Message <" << newMessage.apmac() << ", " << newMessage.timestamp()
                     << "> has not been sent by all boards. Dropping it..." << endl;
            }
        }

        // Clearing the batches buffer
        cout << "Checked all messages of this timeslot. Clearing internal buffer...\n\n\n";
        batchesBuffer.clear();
    }
}