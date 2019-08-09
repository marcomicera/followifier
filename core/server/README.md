## Dummy packet producer example
This simple example shows how to exchange protobuf message between a Python dummy producer and a C++ server. 

#### Steps
1. Compile this server by executing the `./compile.sh` script
2. Launch a TCP listener on a port (e.g., `nc -l -p 12345`)
3. Create a batch file (e.g., `touch batch.txt`) 
4. Launch the dummy packet producer:
    ```bash
    python3 dummy_packet_producer.py  --batch_size 10 --batch_rate 10 --protobuf --out_file batch.txt 127.0.0.1 12345
    ```
5. To deserialize the protobuf message, run `./test_server batch.txt` 