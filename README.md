<p align="center">
  <img src="frontend/paper/src/assets/img/followifier-logo.png" width="60%" alt="followifier logo">
</p>
<p align="center">
  <img alt="GitHub tag (latest by date)" src="https://img.shields.io/github/v/tag/marcomicera/followifier">
</p>

`followifier` is a tracking system that exploits Wi-Fi Probe Request frames sent by ESP32 boards placed in the corners of a room to detect and monitor devices.

### How to run it
1. Create a [server configuration file](core/server/README.md#configuration-file)
1. Start the server:
    ```bash
    $ chmod u+x server.sh
    $ ./server.sh
    ```
1. [Flash the firmware on all boards](board/followifier/README.md)

### Team

- [Marco Micera](https://github.com/marcomicera)
- [Matteo D'Ospina](https://github.com/suprmat95)
- [Pietro Francesco Tirenna](https://github.com/madt1m)
- [Simona Saitta](https://github.com/SelyBarguest)
