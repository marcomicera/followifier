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
