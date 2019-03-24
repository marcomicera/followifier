# Introduction

The purpose of this ESP32 module is to implement a sniffer which:

- Collects WiFi Probes from near station;
- Extracts some metadata from collected probes -- timestamp, hash, RSSI, sender_mac, SSID;
- Group former data from multiple packets and send them over a socket, to a master server.

The boards has to mantain synchronization with respect to the server clock.

# System Design

The core submodules:

- Sniffer
- Server Interaction
- Synchronization
- Configuration

## The Sniffing Module

Quoting from the API guide, the ESP32 WiFi component works as follows:

```

*               Notes about WiFi Programming
 *
 *  The esp32 WiFi programming model can be depicted as following picture:
 *
 *
 *                            default handler              user handler
 *  -------------             ---------------             ---------------
 *  |           |   event     |             | callback or |             |
 *  |   tcpip   | --------->  |    event    | ----------> | application |
 *  |   stack   |             |     task    |  event      |    task     |
 *  |-----------|             |-------------|             |-------------|
 *                                  /|\                          |
 *                                   |                           |
 *                            event  |                           |
 *                                   |                           |
 *                                   |                           |
 *                             ---------------                   |
 *                             |             |                   |
 *                             | WiFi Driver |/__________________|
 *                             |             |\     API call
 *                             |             |
 *                             |-------------|
 *
 * The WiFi driver can be consider as black box, it knows nothing about the high layer code, such as
 * TCPIP stack, application task, event task etc, all it can do is to receive API call from high layer
 * or post event queue to a specified Queue, which is initialized by API esp_wifi_init().
 *
 * The event task is a daemon task, which receives events from WiFi driver or from other subsystem, such
 * as TCPIP stack, event task will call the default callback function on receiving the event. For example,
 * on receiving event SYSTEM_EVENT_STA_CONNECTED, it will call tcpip_adapter_start() to start the DHCP
 * client in it's default handler.
 *
 * Application can register it's own event callback function by API esp_event_init, then the application callback
 * function will be called after the default callback. Also, if application doesn't want to execute the callback
 * in the event task, what it needs to do is to post the related event to application task in the application callback function.
 *
 * The application task (code) generally mixes all these thing together, it calls APIs to init the system/WiFi and
 * handle the events when necessary.
 *
 */
```

In our implementation, the event task will collect the critical metadata from the packets, and will send it to the producer thread.

## Server Interaction

The idea is to gather a certain number of data from the wifi `event_task`, and send them in bufferized bursts.
A thread will therefore receive packets from the callback, serialize it into a given data structure, and send it whenever
a given timeout is reached, or the maximum length threshold has been crossed.

The serialization of the metadata can be handled by the `json` component of our ESP-IDF operating system.

Each JSON message could be so defined:

{
	"timestamp": <timestamp>,
	"packet_list": {
		"packet_1": {
			"transm_addr": <hex_string>
			"bssid": <hex_string>
			"timestamp": <hex_string>
			...
			...
		},
		"packet_2": {...},
		...
		...
	}
}

## Synchronization

Synchronization of slave-server clocks will be handled by means of an SNTP interaction with a server.

## Configuration

Configuration of the board concerns:

- WiFi options -- in promiscuous mode, and in connect-mode to send data to server
- Metadata options -- choose which data should be sent to server
- And other stuff.