# Packet replayer

### How to capture a trace
1. [Launch boards](../../../board/followifier/README.md)
1. Start the NTP server:
    ```bash
    $ /etc/init.d/ntp start
    ```
1. Optionally, free the `core` server port (example with port 12345):
    ```bash
   lsof -i tcp:12345 | grep LISTEN | awk '{print $2}' | xargs kill
   ```
1. Capture batches with:
    ```bash
    $ python3 replayer.py capture <SERVER_PORT> --trace-dir <TRACE_DIRECTORY>
    ```

### How to replay a trace
1. Read the trace `README.me` file and copy its `config.json` file into the [`core server` directory](../../server)
1. [Launch the `core` server](../../server/README.md#launch-the-core-server)
1. Run the trace emulator:
    ```bash
    $ python3 replayer.py replay <SERVER_PORT> --trace-dir <TRACE_DIRECTORY> --batch-number <NUMBER_OF_TIMESLOTS_IN_TRACE_DIR>
    ```
