# echo-upper

Communication test between ESP32 boards (client) and a central PC (server).

## Running the example
1. Build and run the server `cd server && ant build run [-Dport=<port_number>]` (default port is 9991)
2. Specify the Wi-Fi SSID and password by modifying the `WIFI_SSID` and `WIFI_PASSWORD` #define constants in [echo-upper.c](client/main/echo-upper.c)
3. Configure the server destination and port in the same [echo-upper.c](client/main/echo-upper.c) file (`SERVER_ADDRESS` and `SERVER_PORT` #define constants)
4. Allow the server to establish a TCP connection with the board
  * Incoming TCP connections: `sudo iptables -A INPUT -p tcp --dport <port_number> -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT`
  * Outgoing TCP connections: `sudo iptables -A OUTPUT -p tcp --sport <port_number> -m conntrack --ctstate ESTABLISHED -j ACCEPT`
5. Set the proper serial port to which the ESP32 board is connected to via the `cd ../client && make menuconfig` command, as shown in the [Compile and run an hello world example](../../README.md) section
6. Flash and launch the board application: `make app-flash monitor -j 4`
7. You can stop the board application with the `Ctrl + ]` key combo

## Result
1. The server starts and waits for a connection to be established on its port
2. The board will connect to the Wi-Fi network and send a test string to the server
3. The server will reply with **the same string after swapping its case**
