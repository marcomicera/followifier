## Overview

This is the firmware for `followifier` boards.

#### How it works
1. Board starts sniffing and storing packets
2. Upon reaching a certain amount of stored packets:
    1. Board stops sniffing
    2. Board connects to the [Wi-Fi network](main/components/wifi.h) 
    3. Board flushes all the data to the [core server](../../core)
    4. Board disconnects from the [Wi-Fi network](main/components/wifi.h)
3. Board starts sniffing again

#### Configuration
1. Set the Wi-Fi SSID and password in [`main/components/wifi.h`](main/components/wifi.h)
    - You can see known Wi-Fi networks' details on Linux by using:
        ```bash
        $ sudo cat /etc/NetworkManager/system-connections/<SSID> | grep 'ssid=\|psk='
        ```
1. Run the [`core`](../../core/server) server and take note of its IP address and port
    - More instructions on how to do it [here](../../core/server/README.md)
1. Configure the [`core`](../../core/server) server's IP address and port in [`main/components/flusher.h`](main/components/flusher.h)
1. Clean, compile, flash and run the software on all boards:
    ```bash
   $ idf.py fullclean && idf.py build && idf.py -p <port> flash monitor 
   ```
   Where `<port>` indicates the serial port to which the ESP32 board is connected to (e.g., "`COM3`" on Windows or "`/dev/ttyUSB0`" on Linux)
    - You can list all connected boards on Linux with:
        ```bash
        $ ls /dev/ttyUSB*
        ```
1. To exit IDF monitor use the shortcut `Ctrl+]`
