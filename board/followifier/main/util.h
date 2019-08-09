//
// Created by marcomicera on 8/9/19.
//

#ifndef FOLLOWIFIER_UTIL_H
#define FOLLOWIFIER_UTIL_H

#define MUST_NOT_HAPPEN(condition, string_error, ...) {\
    if ((condition)) {\
        fprintf(stderr, string_error, ##__VA_ARGS__);\
        exit(EXIT_FAILURE);\
    }\
}

#endif //FOLLOWIFIER_UTIL_H
