//
// Created by marcomicera on 11/6/19.
//

#include "mbedtls/md.h"
#include "stddef.h"
#include <string.h>
#include <stdio.h>
#include <esp_log.h>

static const char* TAG = "hash-example";

/**
 * All-purpose hash function.
 * https://techtutorialsx.com/2018/05/10/esp32-arduino-mbed-tls-using-the-sha-256-algorithm/
 *
 * @param payload   payload to be hashed.
 * @param result    the digest.
 */
void hash(const char *payload, unsigned char* result) {
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    const size_t payloadLength = strlen(payload);
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, (const unsigned char *) payload, payloadLength);
    mbedtls_md_finish(&ctx, result);
    mbedtls_md_free(&ctx);
}

void app_main(void) {
    char *payload = "Hello SHA 256!";
    unsigned char result[32];
    ESP_LOGI(TAG, "Hashing the following sentence: %s", payload);
    hash(payload, result);
    ESP_LOGI(TAG, "Printing digest...");
    for(unsigned long i= 0; i < sizeof(result); i++){
        char str[3];
        sprintf(str, "%02x", (int)result[i]);
        ESP_LOGI(TAG, "%s", str);
    }
}

