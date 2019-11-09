//
// Created by simona on 21/09/19.
//

#ifndef CORE_RECEIVER_H
#define CORE_RECEIVER_H

#include <string>
#include <gen/message.pb.h>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include "database.h"

using boost::uuids::detail::md5;

#define NUMBER_BOARDS 2

typedef std::unordered_multimap<
        std::string, // frame hash
        std::pair< // sender
                std::string, // board's MAC address
                followifier::ESP32Metadata
        >> messages_map;

/**
 * Receives and filters batches from all boards.
 */
class receiver {

    /**
     * When true, it never deletes frames of previous rounds.
     *
     * Warning: this is an experimental flag that should be set only in special cases
     *          (e.g., debugging, no synchronization between boards, etc.).
     *          This flag will cause a StackOverflow after a while.
     */
    static const bool ROUNDLESS_MODE = false;

public:

    /**
     * Adds a new batch to the buffer.
     *
     * @param newBatch the just-received batch to be added to the buffer.
     */
    static void addBatch(const followifier::Batch &newBatch, database &database);

    /**
     * Logs a Proto message following its own format.
     *
     * @param message   message to be printed.
     * @return          a string representation of the message.
     */
    static std::string logMessage(const followifier::ESP32Message &message) {
        return "< Hash: " + prettyHash(message.frame_hash()) +
               ",  Src MAC: " + message.metadata().apmac() +
               ",  Timestamp: " + std::to_string(message.metadata().timestamp()) + ">";
    }

    /**
     * To be called every time a new round begins.
     */
    static void newRound() {
        if (!ROUNDLESS_MODE) {
            lastRoundBoardMacs.clear();
            messagesBuffer.clear();
        }
        std::cout << "All boards have sent their batch. Starting a new round..." << std::endl;
    }

protected:

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
     * Messages buffer mapping frame hashes to boards' metadata.
     */
    static messages_map messagesBuffer;

    /**
     * MAC addresses of boards that have sent a message during the last round.
     */
    static std::unordered_set<std::string> lastRoundBoardMacs;

    /**
     * Pretty-prints a hash digest.
     *
     * @param digest    digest to be printed.
     * @return          a pretty-printed digest.
     */
    static std::string prettyHash(const std::string &digest) {
        const auto charDigest = digest.c_str();
        std::string result;
        boost::algorithm::hex(charDigest, charDigest + sizeof(md5::digest_type) * 2, std::back_inserter(result));
        return result;
    }
};

#endif //CORE_RECEIVER_H
