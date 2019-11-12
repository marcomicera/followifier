# `dummy_packet_producer`
Dummy packets generator in Python.\
It simulates boards behaviour.
Useful when debugging the [`core` server](../../server).

#### Launching the example
 ```bash
$ python3 dummy_packet_producer.py --batch_size 5 --batch_rate 5 --boards_number 2 192.168.1.15 12345
```
