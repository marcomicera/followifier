#include <src/connection.h>
#include "receiver.h"

using std::cout;
using std::cerr;
using std::endl;

std::mutex receiver::m;
messages_map receiver::messagesBuffer;
std::unordered_set<std::string> receiver::lastRoundBoardMacs;

void receiver::addBatch(const followifier::Batch &newBatch, database &database) {

    /* Critical section */
    std::lock_guard<std::mutex> lockGuard(m);

    // Printing received messages
    cout.flush();
    cout << "New batch received from " + newBatch.boardmac() + " of size " + std::to_string(newBatch.messages_size());
    if (newBatch.messages_size() > 0) { // if there is at least one message in it
        cout << ":" << endl;
        int messageCounter = 1;
        for (const auto &newMessage : newBatch.messages()) { // print all messages
            cout << messageCounter++ << ")\t" << logMessage(newMessage) << endl;
        }
    } else {
        cout << ".";
    }
    cout << endl << endl;

    /* If the board's MAC address appears for the first time */
    if (lastRoundBoardMacs.find(newBatch.boardmac()) == lastRoundBoardMacs.end()) {

        /* Insert it into the set of boards that have sent a message during the last round */
        lastRoundBoardMacs.insert(newBatch.boardmac());
    } else {

        /* A new round begins.
         * The same board could otherwise announce the same frame at least twice during the same round,
         * and this is considered to be an error.
         */
        newRound();
    }

    /* True when all boards have sent the same frame, hence it's time for a new round */
    bool aFrameHasBeenSentByAllBoards = false;

    /* For each message in the batch */
    for (const followifier::ESP32Message &newMessage: newBatch.messages()) {

        /* If this frame has been sent already */
        if (messagesBuffer.find(newMessage.frame_hash()) !=
            messagesBuffer.end()) { // lower complexity than `equal_range()`

            /* Check whether the same board has sent the same frame already */
            auto sameFrameSendersRange = messagesBuffer.equal_range(newMessage.frame_hash());
            for_each(sameFrameSendersRange.first, sameFrameSendersRange.second,
                     [&newBatch, &newMessage](messages_map::value_type &senderData) {
                         if (senderData.first == newBatch.boardmac()) {

                             /* Error: same board has announced the same frame twice */
                             cerr << "Board " << newBatch.boardmac() << " has announced frame "
                                  << newMessage.frame_hash() << " at least twice." << endl;
                             if (!ROUNDLESS_MODE) {
                                 cerr << "Terminating since roundless mode is disabled." << endl;
                                 exit(1); // FIXME maybe there's a better way to handle this
                             }
                         }
                     });
        }

        /* Insert it in the messages buffer */
        messagesBuffer.insert(std::make_pair(newMessage.frame_hash(),
                                             std::make_pair(newBatch.boardmac(), newMessage.metadata())));

        /* If this message has been sent by all other boards */
        if (messagesBuffer.count(newMessage.frame_hash()) == NUMBER_BOARDS) {

            aFrameHasBeenSentByAllBoards = true;

            /* Storing it into the database */
            cout << "Message " << prettyHash(newMessage.frame_hash()) << " has been sent by all boards." << endl;
            // TODO Computing statistics (#33)
            // FIXME The following should store all these statistics
            database.insert_message(newMessage); // TODO check object internal representation in MongoDB

            /* Clearing the entry relative to this frame */
            messagesBuffer.erase(newMessage.frame_hash());
        } else {

            // TODO Roundless mode: delete messages not being sent by all boards after a while
        }
    }

    /* Number of boards seen during this round assertion */
    if (aFrameHasBeenSentByAllBoards && (lastRoundBoardMacs.size() != NUMBER_BOARDS)) {
        cout << "A frame has been sent by all boards, but during different rounds." << endl;
        if (!ROUNDLESS_MODE) {
            cerr << "Server should not consider frames belonging to previous round since roundless mode is disabled."
                 << endl;
            exit(1);
        }
    }

    /* If all boards have sent their batch */
    if (lastRoundBoardMacs.size() == NUMBER_BOARDS) {

        /* Start a new round */
        newRound();
    }
}