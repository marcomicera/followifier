#include <src/connection.h>
#include "receiver.h"
#include "statistics.h"

using std::cout;
using std::cerr;
using std::endl;

std::mutex receiver::m;
messages_map receiver::messagesBuffer;
std::unordered_set<std::string> receiver::lastRoundBoardMacs;

void receiver::addBatch(const followifier::Batch &newBatch, database &database) {

    /* Critical section */
    std::lock_guard<std::mutex> lockGuard(m);

    /* Printing headline info */
    cout.flush();
    cout << "New batch received from " + newBatch.boardmac() + " of size " + std::to_string(newBatch.messages_size())
         << "." << endl;

    /* If the board's MAC address does not appear for the first time */
    if (lastRoundBoardMacs.find(newBatch.boardmac()) != lastRoundBoardMacs.end()) {

        /* A new round begins.
         * The same board could otherwise announce the same frame at least twice during the same round,
         * and this is considered to be an error.
         */
        newRound("Board " + newBatch.boardmac() + " appears for the second time during this round.");
    } else {

        cout << "Board " << newBatch.boardmac() << " appears for the first time during this round." << endl;
    }

    /* Insert it into the set of boards that have sent a message during the last round */
    lastRoundBoardMacs.insert(newBatch.boardmac());

    /* Printing received messages */
    if (newBatch.messages_size() > 0) { // if there is at least one message in it
        int messageCounter = 1;
        for (const auto &newMessage : newBatch.messages()) { // print all messages
            cout << messageCounter++ << ")\t" << logMessage(newMessage) << endl;
        }
    }
    cout << endl << endl;

    /* True when all boards have sent the same frame, hence it's time for a new round */
    bool aFrameHasBeenSentByAllBoards = false;

    /* For each message in the batch */
    for (const followifier::ESP32Message &newMessage: newBatch.messages()) {

        /* If this frame has been sent already */
        if (messagesBuffer.find(newMessage.frame_hash()) !=
            messagesBuffer.end()) { // lower complexity than `equal_range()`

            /* Check whether the same board has sent the same frame already */
            auto messageInBuffer = messagesBuffer.find(newMessage.frame_hash());
            for_each(messageInBuffer->second.begin(), messageInBuffer->second.end(),
                     [&newBatch, &newMessage](
                             std::unordered_map<std::string, followifier::ESP32Metadata>::value_type &senderData) {
                         if (senderData.first == newBatch.boardmac()) {
                             /* Error: same board has announced the same frame twice */
                             cerr << "Board " << newBatch.boardmac() << " has announced frame "
                                  << prettyHash(newMessage.frame_hash()) << " at least twice." << endl;
                             if (!ROUNDLESS_MODE) {
                                 cerr << "Terminating since roundless mode is disabled." << endl;
                                 exit(1); // FIXME maybe there's a better way to handle this
                             }
                         }
                     });

            /* Insert metadata*/
            messageInBuffer->second.insert(
                    std::pair<std::string, followifier::ESP32Metadata>(newBatch.boardmac(), newMessage.metadata()));
        } else {
            /* First time receiving the message */
            std::unordered_map<std::string, followifier::ESP32Metadata> tempBoardMap;
            tempBoardMap.insert(std::make_pair(newBatch.boardmac(), newMessage.metadata()));
            messagesBuffer.insert(std::make_pair(newMessage.frame_hash(), tempBoardMap));
        }


        /* If this message has been sent by all other boards */
        if (messagesBuffer.find(newMessage.frame_hash())->second.size() == NUMBER_BOARDS) {

            aFrameHasBeenSentByAllBoards = true;

            Point position = statistics::getPosition(newMessage.frame_hash(),
                                                     messagesBuffer.find(newMessage.frame_hash())->second);
            /* Storing it into the database */
            cout << "Message " << newMessage.frame_hash() << " has been sent by all boards from position "
                 << position.getX() << "," << position.getY() << endl;
            /* Compute position */
            // FIXME The following should store all these statistics
            database.insert_message(newMessage, position); // TODO check object internal representation in MongoDB

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
        newRound(std::to_string(lastRoundBoardMacs.size()) + " out of " + std::to_string(NUMBER_BOARDS) +
                 " boards have sent their batch.");
    }
}