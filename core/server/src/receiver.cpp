#include <src/connection.h>
#include "receiver.h"
#include "statistics.h"

using std::cout;
using std::cerr;
using std::endl;

std::mutex receiver::m;
messages_map receiver::messagesBuffer;
#ifndef ROUNDLESS_MODE
std::unordered_set<std::string> receiver::lastRoundBoardMacs;
#endif

void receiver::addBatch(const followifier::Batch &newBatch, database &database) {

    /* Critical section */
    std::lock_guard<std::mutex> lockGuard(m);

    /* Printing headline info */
    cout.flush();
    cout << "New batch received from " + newBatch.boardmac() + " of size " + std::to_string(newBatch.messages_size())
         << "." << endl;

#ifndef ROUNDLESS_MODE
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
#endif

    /* Printing received messages */
    if (newBatch.messages_size() > 0) { // if there is at least one message in it
        int messageCounter = 1;
        for (const auto &newMessage : newBatch.messages()) { // print all messages
            cout << messageCounter++ << ")\t" << logMessage(newMessage) << endl;
        }
    }
    cout << endl << endl;

#ifndef ROUNDLESS_MODE
    /* True when all boards have sent the same frame, hence it's time for a new round */
    bool aFrameHasBeenSentByAllBoards = false;
#endif

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
#ifndef ROUNDLESS_MODE
                             cerr << "Terminating since roundless mode is disabled." << endl;
                             exit(1); // FIXME maybe there's a better way to handle this
#endif
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

#ifndef ROUNDLESS_MODE
            aFrameHasBeenSentByAllBoards = true;
#endif
            cout << "Message " << prettyHash(newMessage.frame_hash()) << " has been sent by all boards." << endl;

            /* Computing device position */
            Point devicePosition = statistics::getDevicePosition(messagesBuffer.find(newMessage.frame_hash())->second);
            statistics::logDeviceLocation(newMessage.metadata().devicemac(), devicePosition);
            if (!devicePosition.isValid()) {
                statistics::logInvalidDeviceLocation(prettyHash(newMessage.frame_hash()),
                                                     newMessage.metadata().devicemac(),
                                                     devicePosition);
                cout << endl;
                continue;
            }
            cout << endl;

            /* Storing it into the database */
            // FIXME The following should store all these statistics
            // TODO check object internal representation in MongoDB
            database.insert_message(newMessage, devicePosition);

            /* Clearing the entry relative to this frame */
            messagesBuffer.erase(newMessage.frame_hash());
        } else {

            // TODO Roundless mode: delete messages not being sent by all boards after a while
        }
    }

#ifndef ROUNDLESS_MODE
    /* Number of boards seen during this round assertion */
    if (aFrameHasBeenSentByAllBoards && (lastRoundBoardMacs.size() != NUMBER_BOARDS)) {
        cerr << "A frame has been sent by all boards, but during different rounds.\n"
         << "Server should not consider frames belonging to previous round since roundless mode is disabled."
         << endl;
        exit(1);
    }

    /* If all boards have sent their batch */
    if (lastRoundBoardMacs.size() == NUMBER_BOARDS) {

        /* Start a new round */
        newRound(std::to_string(lastRoundBoardMacs.size()) + " out of " + std::to_string(NUMBER_BOARDS) +
                 " boards have sent their batch.");
    }
#endif

    /* Delete messages older than 5 minutes */
    deleteOldMessagesFromBuffer();
}

void receiver::deleteOldMessagesFromBuffer() {

    struct timeval tp{};
    std::vector<std::string> messagesToDelete;

    //get 5 minutes ago milliseconds
    gettimeofday(&tp, nullptr);
    long int ms = (tp.tv_sec - 5 * 60);
    cout << "Current timestamp: " << tp.tv_sec << ", 5 minutes ago: " << ms << endl;

    for (auto i : messagesBuffer) {
        if (i.second.begin()->second.timestamp() < ms) {
            //adds them to a vector to delete them later on
            messagesToDelete.push_back(i.first);
        }
    }

    //delete found messages
    if (!messagesToDelete.empty()) {
        cout << "Deleting " << messagesToDelete.size() << " messages" << endl;
        for (const auto &i : messagesToDelete) {
            messagesBuffer.erase(i);
        }
        // cout << "Deleted old messages" << endl;
    }
    messagesToDelete.clear();
}