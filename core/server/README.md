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
1. Set the number of boards in [`receiver.h`](src/receiver.h)
1. Optionally, free the `core` server port (default is `12345`):
    ```bash
   lsof -i tcp:12345 | grep LISTEN | awk '{print $2}' | xargs kill
   ```
1. Run the `core` server:
    ```bash
    $ chmod u+x start.sh
    $ ./start.sh
    ```

##### Using a dummy packet producer to generate input data
1. Create a batch file (e.g., `touch batch.txt`) 
1. Move to the [dummy packet producer directory](../examples/dummy_packets)
    ```bash
    $ cd ../examples/dummy_packets
    ```
1. Launch the dummy packet producer
    ```bash
    $ python3 dummy_packet_producer.py  --batch_size 10 --batch_rate 10 --protobuf --out_file batch.txt 127.0.0.1 12345
    ```

## References 
1. [How to use MongoDB with C](http://mongoc.org/libmongoc/current/tutorial.html#starting-mongodb)
