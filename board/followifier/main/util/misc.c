//
// Created by marcomicera on 9/12/19.
//

#include "misc.h"

const char *TAG = "followifier";

unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c

    return hash;
}
