//
// Created by marcomicera on 9/12/19.
//

#include "misc.h"
#include "mbedtls/md.h"
#include <string.h>
#include <esp_log.h>

const char *TAG = "followifier";

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
