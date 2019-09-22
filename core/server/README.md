# `core` section

### Launch the `core` server
```bash
$ chmod u+x start.sh
$ ./start.sh <BATCH_FILE>
```

#### Using a dummy packet producer to generate input data
1. Create a batch file (e.g., `touch batch.txt`) 
1. Launch the dummy packet producer:
    ```bash
    $ python3 dummy_packet_producer.py  --batch_size 10 --batch_rate 10 --protobuf --out_file batch.txt 127.0.0.1 12345
    ```

# `server` section

### Launch the `server` server
```bash
$ chmod u+x start_server.sh
$ ./start_server.sh
```

#### Using a dummy packet producer to generate input data
Launch the dummy packet producer:
    ```bash
    $ python3 dummy_packet_producer.py  --batch_size 10 --batch_rate 10 --protobuf --out_file batch.txt --ip 127.0.0.1 --ip 10.0.2.15 12345
    ```
IMPORTANT: indicate two IPs to test the server

   
   
