//
// Created by marcomicera on 8/9/19.
//

#ifndef FOLLOWIFIER_MISC_H
#define FOLLOWIFIER_MISC_H

#include "stddef.h"

extern const char *TAG;

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
            ESP_LOGE(SNIFFER_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            ESP_LOGE(SNIFFER_TAG, "errno %d: %s ", errno, strerror(errno));               \
            goto goto_tag;                                                                \
        }                                                                                 \
    } while (0)

/**
 * All-purpose hash function.
 * https://techtutorialsx.com/2018/05/10/esp32-arduino-mbed-tls-using-the-sha-256-algorithm/
 *
 * @param payload   payload to be hashed.
 * @param result    the digest.
 */
void hash(const char *payload, unsigned char* result);

#endif //FOLLOWIFIER_MISC_H
