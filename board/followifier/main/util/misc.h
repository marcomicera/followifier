//
// Created by marcomicera on 8/9/19.
//

#ifndef FOLLOWIFIER_MISC_H
#define FOLLOWIFIER_MISC_H

#include <esp_wifi_types.h>
#include "stddef.h"

/**
 * Tag by which this board is identified in the output.
 * Format: "Board XY", where "XY" represents the last two digits of this board's MAC address
 */
extern char BOARD_TAG[9];

/**
 * Aborts in case the condition is not satisfied.
 * Prints the `errno` number and its meaning.
 */
#define MUST_NOT_HAPPEN(condition, string_error, ...) {\
    if ((condition)) {\
        fprintf(stderr, string_error, ##__VA_ARGS__);\
        fprintf(stderr, " (error %d): %s.\n", errno, strerror(errno));\
        exit(EXIT_FAILURE);\
    }\
}

/**
 * Prints an error message in case the condition is not satisfied
 * and jump to the specified label.
 */
#define ESP_ERROR_CHECK_JUMP_LABEL(a, str, goto_tag, ...)                                              \
    do                                                                                    \
    {                                                                                     \
        if (!(a))                                                                         \
        {                                                                                 \
            ESP_LOGE(BOARD_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__);   \
            ESP_LOGE(BOARD_TAG, "errno %d: %s ", errno, strerror(errno));                 \
            goto goto_tag;                                                                \
        }                                                                                 \
    } while (0)

/**
 * Hashes a Wi-Fi 802.11 packet.
 * https://techtutorialsx.com/2018/05/10/esp32-arduino-mbed-tls-using-the-sha-256-algorithm/
 *
 * @param payload   packet whose payload must be hashed.
 * @param result    the digest.
 */
void hash(const wifi_promiscuous_pkt_t *packet, unsigned char* result);

#endif //FOLLOWIFIER_MISC_H
