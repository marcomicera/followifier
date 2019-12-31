# `core` section
Our `core` server:
- Collects packets from [`board`s](../../board)
- Performs real-time analytics
- Sends data to the [`frontend` module](../../frontend)

## Dependencies
[MongoDB](https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/#install-mongodb-community-edition-using-deb-packages)
and [`mongocxx` drivers](http://mongocxx.org/mongocxx-v3/installation/).

## Configuration file
Create a configuration file `config.json` having the following format:
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
    ]
}
```

## Launch the `core` server
1. Start MongoDB:
    ```bash
   $ sudo service mongod start 
   ```
1. Optionally, free the `core` server port (example with port 12345):
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
