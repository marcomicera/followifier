# echo-upper

Communication test between ESP32 boards (client) and a central PC (server).

## Running the example
1. Build and run the server `cd server && ant build run [-Dport=<port_number>]` (default port is 9991)
2. Specify the Wi-Fi SSID and password by modifying the `WIFI_SSID` and `WIFI_PASSWORD` #define constants in [echo-upper.c](client/main/echo-upper.c)
3. Configure the server destination and port in the same [echo-upper.c](client/main/echo-upper.c) file (`SERVER_ADDRESS` and `SERVER_PORT` #define constants)
6. Build and run the application with `idf.py -p PORT flash monitor`, where `PORT` indicates the serial port to which the ESP32 board is connected to (e.g., "`COM3`" on Windows or "`/dev/ttyUSB0`" on Linux)
7. You can stop the board application with the `Ctrl + ]` key combo

## Result
1. The server starts and waits for a connection to be established on its port
2. The board will connect to the Wi-Fi network and send a test string to the server
3. The server will reply with **the same string after swapping its case**
