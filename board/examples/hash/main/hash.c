//
// Created by marcomicera on 11/6/19.
//

#include "mbedtls/md.h"
#include "stddef.h"
#include <string.h>
#include <stdio.h>

/**
 * All-purpose hash function.
 * https://techtutorialsx.com/2018/05/10/esp32-arduino-mbed-tls-using-the-sha-256-algorithm/
 *
 * @param payload   payload to be hashed.
 * @return          the digest.
 */
size_t* hash(const char *payload) {
    size_t shaResult[32];
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    const size_t payloadLength = strlen(payload);
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, (const unsigned char *) payload, payloadLength);
    mbedtls_md_finish(&ctx, shaResult);
    mbedtls_md_free(&ctx);
    return shaResult;
}

int main() {

    char *payload = "Hello SHA 256!";
    size_t* result = hash(payload);
    for(unsigned long i= 0; i < sizeof(result); i++){
        char str[3];
        sprintf(str, "%02x", (int)result[i]);
        printf("%s", str);
    }

    return 0;
}

