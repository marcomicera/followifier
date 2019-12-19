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
#include "settings.h"

using boost::uuids::detail::md5;

#define NUMBER_BOARDS Settings::configuration.boards.size()

/**
 * When defined, the server never deletes frames of previous rounds.
 *
 * Comment the following line to disable it.
 */
#define ROUNDLESS_MODE

/*
 * Messages which have not been sent by all boards after this many
 * batch receptions will be deleted from the local messages buffer.
 * This must be greater than the number of boards by at least an order of magnitude.
 */
#define MESSAGES_CLEANING_AGE_THRESHOLD 20

typedef std::unordered_map<
        std::string, // frame hash
        std::unordered_map< // sender
                std::string, // board's MAC address
                followifier::ESP32Metadata
        >> messages_map;

/**
 * Receives and filters batches from all boards.
 */
class receiver {

protected:

    /**
     * Batches must be added in an interruptable fashion.
     */
    static std::mutex m;

    /**
     * Messages buffer mapping frame hashes to boards' metadata.
     */
    static messages_map messagesBuffer;

    /**
     * Stores the age of every frame hash.
     * Useful for cleaning purposes.
     */
    static std::unordered_map<std::string, unsigned short> messagesAge;

#ifndef ROUNDLESS_MODE
    /**
     * MAC addresses of boards that have sent a message during the last round.
     */
    static std::unordered_set<std::string> lastRoundBoardMacs;
#endif

public:

    /**
     * Adds a new batch to the buffer.
     *
     * @param newBatch the just-received batch to be added to the buffer.
     */
    static void addBatch(const followifier::Batch &newBatch, database &database);

     /**
      * Deletes old and unused messages from the messages buffer.
      */
    static void cleanMessagesBuffer();

    /**
     * Logs a Proto message following its own format.
     *
     * @param message   message to be printed.
     * @return          a string representation of the message.
     */
    static std::string logMessage(const followifier::ESP32Message &message) {
        return "< Hash: " + prettyHash(message.frame_hash()) +
               ", RSSI: " + std::to_string(message.metadata().rssi()) +
               ",  Src MAC: " + message.metadata().devicemac() +
               ",  Timestamp: " + std::to_string(message.metadata().timestamp()) + ">";
    }

#ifndef ROUNDLESS_MODE
    /**
     * To be called every time a new round begins.
     *
     * @param cause     the cause that led the start of a new round.
     */
    static void newRound(const std::string &cause) {
        std::cout << cause << " " << std::flush; // TODO glog
        std::cout << "Starting a new round..." << std::endl;
        if (lastRoundBoardMacs.size() == NUMBER_BOARDS) {
            std::cout << std::endl << std::endl;
        }
        lastRoundBoardMacs.clear();
        messagesBuffer.clear();
    }
#endif

    /**
     * Pretty-prints a hash digest.
     * Frame hashes are stored as a sequence of bytes, hence this function is needed to print a human-readable version.
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
