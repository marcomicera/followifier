//
// Created by marcomicera on 11/13/19.
//

#include <esp_log.h>
#include <esp_sntp.h>
#include "util/misc.h"
#include "sync.h"

void time_sync_notification_cb(struct timeval *tv) {
    // TODO Sync notification callback
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

void init_sntp() {

    ESP_LOGI(TAG, "Initializing SNTP module...");

    /* Choices:
     *
     * - SNTP_OPMODE_POLL: polling mode using unicast.
     * - SNTP_OPMODE_LISTENONLY
     */
    sntp_setoperatingmode(SNTP_OPMODE_POLL);

    // FIXME Use core name?
    sntp_setservername(0, "pool.ntp.org");

    /* Sync notification callback */
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);

    /* SNTP time update mode:
     *
     * - SNTP_SYNC_MODE_IMMED:  update system time immediately when receiving a response from the SNTP server.
     * - SNTP_SYNC_MODE_SMOOTH: smooth time updating. Time error is gradually reduced using adjtime function.
     *                          If the difference between SNTP response time and system time is large
     *                          (more than 35 minutes) then update immediately.
     */
    sntp_set_sync_mode(SNTP_SYNC_MODE_IMMED);

    /* Initializing module. Sends out request instantly or after SNTP_STARTUP_DELAY(_FUNC). */
    sntp_init();

    ESP_LOGI(TAG, "...STP module initialized.");
}

void obtain_time() {
    time_t now = 0; // wait for time to be set
    struct tm timeinfo = { 0 };
    short retry = 0;
    const short retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}
