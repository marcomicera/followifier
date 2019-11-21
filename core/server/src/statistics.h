#ifndef CORE_STATISTICS_H
#define CORE_STATISTICS_H

#include "point.h"
#include <string>
#include <unordered_map>
#include "../gen/message.pb.h"
#include "receiver.h"

#define ONE_METER_RSSI -65

/**
 * When true, only one device will generate printouts.
 * Used for debug purposes.
 */
#define DEBUG_ONE_DEVICE_TRACKING 0

/**
 * MAC address of the device to be tracked in the "one device tracking" debug mode.
 * Used only when `DEBUG_ONE_DEVICE_TRACKING` is true.
 */
#define DEBUG_TRACKED_DEVICE_MAC "01:23:45:67:89:ab"

class statistics {

protected:

    static bool checkPoint(double, double, std::unordered_map<std::string, followifier::ESP32Metadata> &);

    static double estimatedDistance(double);

public:

    static Point getDevicePosition(std::unordered_map<std::string, followifier::ESP32Metadata> &boardMetadatas);


    /**
     * Prints out the announcement of a device location from a board.
     * It filters out other devices location in case the "one device tracking"
     * debug mode is active (`DEBUG_ONE_DEVICE_TRACKING` set to true).
     *
     * @param boardMac          the board MAC address which is announcing the distance.
     * @param deviceMac         the device distance with respect to the announcing board.
     * @param deviceDistance    the distance announced by the board.
     */
    static void
    logDeviceDistanceAnnouncement(const std::string &boardMac, const std::string &deviceMac, double deviceDistance) {
        if (!DEBUG_ONE_DEVICE_TRACKING || (DEBUG_ONE_DEVICE_TRACKING && deviceMac == DEBUG_TRACKED_DEVICE_MAC)) {
            std::cout << "Board " << boardMac << " announced device " << deviceMac << " at a distance of "
                      << deviceDistance << " cm." << std::endl;
        }
    }

    /**
     * Prints out a device location.
     * It filters out other devices location in case the "one device tracking"
     * debug mode is active (`DEBUG_ONE_DEVICE_TRACKING` set to true).
     *
     * @param deviceMac         the device MAC address whose location is about to be printed.
     * @param deviceLocation    the device location.
     */
    static void logDeviceLocation(const std::string deviceMac, const Point &deviceLocation) {
        if (!DEBUG_ONE_DEVICE_TRACKING || (DEBUG_ONE_DEVICE_TRACKING && deviceMac == DEBUG_TRACKED_DEVICE_MAC)) {
            std::cout << "Device " << deviceMac << " located at: " << deviceLocation << "." << std::endl;
        }
    }

    /**
     * Prints out an error message in case a device has an invalid location.
     * It filters out other devices location in case the "one device tracking"
     * debug mode is active (`DEBUG_ONE_DEVICE_TRACKING` set to true).
     *
     * @param frameHash         the frame hash that will be discarded as a consequence of the device's invalid location.
     * @param deviceMac         the MAC address of the device in an invalid location.
     * @param deviceLocation    the invalid device's location.
     */
    static void
    logInvalidDeviceLocation(const std::string &frameHash, const std::string deviceMac, const Point &deviceLocation) {
        if (!DEBUG_ONE_DEVICE_TRACKING || (DEBUG_ONE_DEVICE_TRACKING && deviceMac == DEBUG_TRACKED_DEVICE_MAC)) {
            std::cerr << "Frame" << frameHash << " discarded since announcing device " << deviceMac
                      << " is located in an invalid position (" << deviceLocation << ")." << std::endl;
        }
    }
};


#endif //CORE_STATISTICS_H
