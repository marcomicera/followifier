# `dummy_packet_producer`
Dummy packets generator in Python.\
It simulates boards behaviour.
Useful when debugging the [`core` server](../../server).

#### Launching the example
 ```bash
$ python3 dummy_packet_producer.py 192.168.1.15 12345 --batch_size 5 --batch_rate 5 --boards_mac 2e:c5:44:b4:7a:08 1a:dd:db:a7:b7:f5
```
