# `core` section

### Launch the `core` server
```bash
$ chmod u+x start.sh
$ ./start.sh <BATCH_FILE>
```

#### Using a dummy packet producer to generate input data
1. Create a batch file (e.g., `touch batch.txt`) 
1. Move to the [dummy packet producer directory](../examples/dummy_packets)
    ```bash
    $ cd ../examples/dummy_packets
    ```
1. Launch the dummy packet producer
    ```bash
    $ python3 dummy_packet_producer.py  --batch_size 10 --batch_rate 10 --protobuf --out_file batch.txt 127.0.0.1 12345
    ```
