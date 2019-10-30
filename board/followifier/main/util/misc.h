//
// Created by marcomicera on 8/9/19.
//

#ifndef FOLLOWIFIER_MISC_H
#define FOLLOWIFIER_MISC_H

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
 * djb2 hash function by Dan Bernstein.
 * https://stackoverflow.com/questions/7666509/hash-function-for-string
 *
 * @param str   data to be hashed.
 * @return      data digest.
 */
unsigned long hash(unsigned char *);

#endif //FOLLOWIFIER_MISC_H
