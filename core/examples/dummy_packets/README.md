# `dummy_packet_producer`
Dummy packets generator in Python.\
Useful while debugging the [`core` server](../../server).

#### Launching the example
1. Create a batch file (e.g., `touch batch.txt`)
1. Launch the dummy packet producer
    ```bash
    $ python3 dummy_packet_producer.py  --batch_size 10 --batch_rate 10 --protobuf --out_file batch.txt 127.0.0.1 12345
    ```
