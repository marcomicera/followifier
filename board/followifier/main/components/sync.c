//
// Created by marcomicera on 11/13/19.
//

#include <esp_log.h>
#include <esp_sntp.h>
#include <util/conf.h>
#include "util/misc.h"

void time_sync_notification_cb(struct timeval *tv) {

    // Retrieving time
    char strftime_buf[64];
    time_t now = 0; // wait for time to be set
    struct tm timeinfo = {0};
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time is: %s", strftime_buf);
}

void init_sntp() {

    /* Choices:
     *
     * - SNTP_OPMODE_POLL: polling mode using unicast.
     * - SNTP_OPMODE_LISTENONLY
     */
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

    sntp_setservername(0, SERVER_ADDRESS);

    /* Sync notification callback */ // TODO Sync time periodically
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);

    /* SNTP time update mode:
     *
     * - SNTP_SYNC_MODE_IMMED:  update system time immediately when receiving a response from the SNTP server.
     * - SNTP_SYNC_MODE_SMOOTH: smooth time updating. Time error is gradually reduced using adjtime function.
     *                          If the difference between SNTP response time and system time is large
     *                          (more than 35 minutes) then update immediately.
     */
    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);

    ESP_LOGI(TAG, "SNTP module initialized.");
}

void send_sntp_request() {

    /* Initializing module.
     * Sends out request instantly or after SNTP_STARTUP_DELAY(_FUNC).
     */
    sntp_init();
}

bool time_has_been_set() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    if (timeinfo.tm_year < (2018 - 1900)) { // Is time set? If not, tm_year will be (1970 - 1900).
        ESP_LOGI(TAG, "Time is not set yet.");
        return false;
    }

    return true;
}
