/**
 * Copyright (c) 2017, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * ESP32/016
 * WiFi Sniffer.
 */

/* EDITED by Pietro Tirenna, 2018 */

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"


#define	WIFI_CHANNEL_MAX		(13)
#define	WIFI_CHANNEL_SWITCH_INTERVAL	(500)

#define PROBE_REQUEST 	0x0040
#define PROBE_RESPONSE	0x0050

#define SUBTYPE_MASK 0x00F0

#define DEBUG 0

static wifi_country_t wifi_country = {.cc="CN", .schan=1, .nchan=13, .policy=WIFI_COUNTRY_POLICY_AUTO};

typedef struct {
	unsigned frame_ctrl:16;
	unsigned duration_id:16;
	uint8_t addr1[6]; /* receiver address */
	uint8_t addr2[6]; /* sender address */
	uint8_t addr3[6]; /* filtering address */
	unsigned sequence_ctrl:16;
	uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
	wifi_ieee80211_mac_hdr_t hdr;
	uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

static esp_err_t event_handler(void *ctx, system_event_t *event);
static void wifi_sniffer_init(void);
static void wifi_sniffer_set_channel(uint8_t channel);
//static const char *wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type);
static const char *wifi_sniffer_packet_subtype2str(wifi_ieee80211_mac_hdr_t *header);
static void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type);

static int wifi_sniffer_is_probe(unsigned short fctl);
void

app_main(void)
{
	int channel = 1;

	/* setup */
	wifi_sniffer_init();

	/* loop */
	while (true) {
		vTaskDelay(WIFI_CHANNEL_SWITCH_INTERVAL / portTICK_PERIOD_MS);
		wifi_sniffer_set_channel(channel);
		channel = (channel % WIFI_CHANNEL_MAX) + 1;
    	}
}

esp_err_t
event_handler(void *ctx, system_event_t *event)
{

	return ESP_OK;
}

void
wifi_sniffer_init(void)
{

	nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_country(&wifi_country) ); /* set country for channel range [1, 13] */
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_NULL) );
    ESP_ERROR_CHECK( esp_wifi_start() );
	esp_wifi_set_promiscuous(true);
	esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

void
wifi_sniffer_set_channel(uint8_t channel)
{
	esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
}

/*
const char *
wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
{
	switch(type) {
	case WIFI_PKT_MGMT: return "MGMT";
	case WIFI_PKT_DATA: return "DATA";
	default:
	case WIFI_PKT_MISC: return "MISC";
	}
}
*/

const char *
wifi_sniffer_packet_subtype2str(wifi_ieee80211_mac_hdr_t *header)
{
	// Masking the frame control to only examine Subtype fields, bits [12:9]
	// i.e 0000 1111 0000 0000
	// PROBE_REQUEST -> 1000
	// PROBE_RESPONSE -> 1001
	switch(header->frame_ctrl & SUBTYPE_MASK) {
		case PROBE_REQUEST:
			return "PROBE_REQUEST";
		case PROBE_RESPONSE:
			return "PROBE_RESPONSE";
		default:
			return "NOTIMPL";
	}
}

int wifi_sniffer_is_probe(unsigned short fctl) {
	return ((fctl & SUBTYPE_MASK) == PROBE_REQUEST || (fctl & SUBTYPE_MASK) == PROBE_RESPONSE);
}

/* The packet handler here has to print information from PROBE REQUESTS and RESPONSE.
/* For each packet, we print:
/* Sender MAC
/* SSID - if not broadcast
/* timestamp
/* packet hash
/* Signal power
*/
void
wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
{

	/* PIETRO - Previosly, this function only printed address header fields.
	Extending this to dump PROBE REQUEST frames, with metadata
	*/


	if (type != WIFI_PKT_MGMT)
		return;

	// Converts buffer to the correct struct definition
	const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
	
	// Extracts Payload and Header with our user-defined representation 
	const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
	const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;


	if (DEBUG) {
		printf("Masked:unmasked frame control of packet has value %04x:%04x\n",
			(hdr->frame_ctrl & SUBTYPE_MASK), hdr->frame_ctrl);
	}

	if (wifi_sniffer_is_probe((unsigned short)hdr->frame_ctrl)) {
		printf("PACKET TYPE=%s | CHAN=%02d | RSSI=%02d \n"
			"TransmADDR=%02x:%02x:%02x:%02x:%02x:%02x |"
			" BSSID=%02x:%02x:%02x:%02x:%02x:%02x \n"
			"TIMESTAMP=%10d\n",
			wifi_sniffer_packet_subtype2str((wifi_ieee80211_mac_hdr_t *)hdr),
			ppkt->rx_ctrl.channel,
			ppkt->rx_ctrl.rssi,
			// ADDR2
			hdr->addr2[0],hdr->addr2[1],hdr->addr2[2],
			hdr->addr2[3],hdr->addr2[4],hdr->addr2[5],
			// ADDR3
			hdr->addr3[0],hdr->addr3[1],hdr->addr3[2],
			hdr->addr3[3],hdr->addr3[4],hdr->addr3[5],
			ppkt->rx_ctrl.timestamp
		);
	}
}
