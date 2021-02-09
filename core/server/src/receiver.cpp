#include <src/connection.h>
#include "receiver.h"
#include "statistics.h"

using std::cout;
using std::cerr;
using std::endl;

std::mutex receiver::m;
messages_map receiver::messagesBuffer;
std::unordered_map<std::string, unsigned short> receiver::messagesAge;
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

    /* True when all boards have sent the same frame, hence it's time for a new round */
    bool aFrameHasBeenSentByAllBoards = false;
#endif

    /* Average RSSI value for each bursts: Probe Request frames belonging to the same burst are considered
     * to be received subsequently.
     * Every average RSSI value is paired to the number of contributions needed to compute the average.
     */
    typedef std::pair<double, unsigned int> avg_t; // RSSI average value and number of contributions
    std::unordered_map<burst, avg_t, burst::burst_hasher> burstAverages;

    /* For printing purposes */
    unsigned short messageCounter = 1;

    /* For each message in the batch */
    for (const followifier::ESP32Message &newMessage: newBatch.messages()) {

        /* Burst to which this message belongs to */
        burst currentBurst = burst(newMessage);

        /* Retrieving corresponding burst */
        if (burstAverages.find(currentBurst) == burstAverages.end()) {

            /* New burst */
            burstAverages.insert(std::pair<burst, avg_t>(currentBurst,
                                                         std::pair<double, unsigned int>(newMessage.metadata().rssi(),
                                                                                         1)));
        } else {

            /* Updating average of the existing burst */
            burstAverages[currentBurst].first += (newMessage.metadata().rssi() - burstAverages[currentBurst].first) /
                                                 (burstAverages[currentBurst].second++);
        }

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
            messagesAge.insert(std::make_pair(newMessage.frame_hash(), 0));
        }

        /* Metadata alteration: inserting the burst average RSSI value */
        messagesBuffer.find(newMessage.frame_hash())->second[newBatch.boardmac()].set_rssi(
                burstAverages[currentBurst].first);

        /* Print message to screen */
        cout << messageCounter++ << ")\t" << logMessage(newMessage, burstAverages[currentBurst].first) << endl;

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
            messagesAge.erase(newMessage.frame_hash());
        } else {

            // TODO Roundless mode: delete messages not being sent by all boards after a while
        }
    }

    // Printing bursts average values
//    for (auto &burstAverage: burstAverages) {
//        cout << "Burst " << std::string(burstAverage.first) << " has an average RSSI value of "
//             << burstAverage.second.first << "." << endl;
//    }

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

    /* Delete old and unused messages */
    cleanMessagesBuffer();

    cout << endl << endl;
}

void receiver::cleanMessagesBuffer() {

    auto ageEntryIterator = messagesAge.begin();
    while (ageEntryIterator != messagesAge.end()) {
        if (++ageEntryIterator->second >= MESSAGES_CLEANING_AGE_THRESHOLD) {
            cout << "Deleting frame " << prettyHash(ageEntryIterator->first)
                 << " since it has not been sent by all boards after " << ageEntryIterator->second
                 << " batch receptions." << endl;
            messagesBuffer.erase(ageEntryIterator->first);
            ageEntryIterator = messagesAge.erase(ageEntryIterator);
        } else {
            ++ageEntryIterator;
        }
    }
}