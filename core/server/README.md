# `core` section
Our `core` server:
- Collects packets from [`board`s](../../board)
- Performs real-time analytics
- Sends data to the [`frontend` module](../../frontend)

## Launch the `core` server
1. Install [MongoDB](https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/#install-mongodb-community-edition-using-deb-packages)
and [`mongocxx` drivers](http://mongocxx.org/mongocxx-v3/installation/)
1. Start MongoDB:
    ```bash
   $ sudo service mongod start 
   ```
1. Create a configuration file `config.json` having the following format: <!-- TODO add missing fields -->
    ```json
    {
        "port": 12345,
        "boards": [
            {
                "mac": "2e:c5:44:b4:7a:08",
                "x": 0,
                "y": 0 
            },
            {
                "mac": "1a:dd:db:a7:b7:f5",
                "x": 20,
                "y": 0 
            }

        ],
        "room_coordinates":[
            {"x": 0, "y": 0},
            {"x": 0, "y": 90},
            {"x": 90, "y": 0},
            {"x": 90, "y": 90}
        ],
        "calibration": {
            "calibration_device": "cc:61:e5:13:1a:d1",
            "min_num_calibration_messages": 2,
            "duration_in_seconds": 15
        }
    }
    ```
    - The `calibration` object is completely optional, and refers to the initial boards calibration phase during
    the user is asked to place a mobile device at 1 meter from all boards, one at the time.\
    If missing, calibration will not be performed.
        -  The mobile calibration device that has to be carried around is `calibration_device`.\
        If missing, calibration will not be performed.
        - `min_num_calibration_messages` represents the minimum number of messages that the calibration device needs
        to send in a batch in order for the calibration to make sense.\
        If missing, a default value will be used.
        - `duration_in_seconds` represents the duration in seconds of each board calibration.\
        If missing, a default value will be used.
1. Optionally, free the `core` server port:
    ```bash
   lsof -i tcp:12345 | grep LISTEN | awk '{print $2}' | xargs kill
   ```
1. Run the `core` server:
    ```bash
    $ chmod u+x start.sh
    $ ./start.sh
    ```

##### Using a dummy packet producer to generate input data
1. Move to the [dummy packet producer directory](../examples/dummy_packets)
    ```bash
    $ cd ../examples/dummy_packets
    ```
1. Launch the dummy packet producer
    ```bash
    $ python3 dummy_packet_producer.py --batch_size 10 --batch_rate 10 --protobuf --out_file batch.txt 127.0.0.1 12345
    ```

## References 
1. [How to use MongoDB with C](http://mongoc.org/libmongoc/current/tutorial.html#starting-mongodb)
