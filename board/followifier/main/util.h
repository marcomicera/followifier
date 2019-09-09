//
// Created by marcomicera on 8/9/19.
//

#ifndef FOLLOWIFIER_UTIL_H
#define FOLLOWIFIER_UTIL_H

static const char *TAG = "followifier"; // TODO use ESP_LOGE(TAG, ...) in this file

#define MUST_NOT_HAPPEN(condition, string_error, ...) {\
    if ((condition)) {\
        fprintf(stderr, string_error, ##__VA_ARGS__);\
        fprintf(stderr, ": %s.\n", strerror(errno));\
        exit(EXIT_FAILURE);\
    }\
}

#endif //FOLLOWIFIER_UTIL_H
