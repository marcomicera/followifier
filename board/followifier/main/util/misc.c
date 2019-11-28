//
// Created by marcomicera on 9/12/19.
//

#include "misc.h"
#include "mbedtls/md.h"
#include <string.h>
#include <esp_log.h>
#include <esp_wifi_types.h>

char BOARD_TAG[9] = "Board";

void hash(const wifi_promiscuous_pkt_t *packet, unsigned char* result) {
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    const size_t payloadLength = packet->rx_ctrl.sig_len;
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&ctx);
    // Hashing without including CRC TODO Check whether this is right or not
    mbedtls_md_update(&ctx, (const unsigned char *) packet->payload, payloadLength - 4);
    mbedtls_md_finish(&ctx, result);
    mbedtls_md_free(&ctx);
}
