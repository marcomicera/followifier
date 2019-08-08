import argparse
import datetime
import json
#import protobuf
import random
import socket
import string
import time
import sys

from functools import wraps

hexdigits = '0123456789abcdef'

def gen_random_mac():
    return ':'.join([''.join(random.choices(population=hexdigits, k=2)) for _ in range(6)])

def gen_random_ssid():
    return ''.join(random.choices(population=string.ascii_uppercase, k=8))

def gen_random_timestamp():
    return random.randrange(sys.maxsize)

def gen_random_hash():
    return ''.join(random.choices(population=hexdigits, k=16))

def gen_random_rsi():
    return random.randrange(-90, 0)

class DummyPacket(object):
    def __init__(self, mac_address=None, ssid=None, timestamp=None, pkt_hash=None, rsi=None):
        self.mac_address = mac_address or gen_random_mac()
        self.ssid = ssid or gen_random_ssid()
        self.timestamp = timestamp or gen_random_timestamp()
        self.pkt_hash = pkt_hash or gen_random_hash()
        self.rsi = rsi or gen_random_rsi()

def timer(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        res = func(*args, **kwargs)
        print(f"Function call took {time.perf_counter() - start}")
        return res
    return wrapper
        
@timer
def produce_json_batch(batch_size):
    return json.dumps([DummyPacket().__dict__ for _ in range(batch_size)])
    
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('ip', action='store', help="server IP")
    parser.add_argument('port', action='store', type=int, help='server port')
    parser.add_argument('--batch_size', action='store', type=int, help='number of packets produced per batch',
            default=100)
    parser.add_argument('--batch_rate', action='store', type=int, help='period in seconds between subsequent batches',
            default=1)
    args = parser.parse_args()

    # UDP would probably be more suited
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((args.ip, args.port))
    
    while True:
        packets_json = produce_json_batch(args.batch_size)
        s.send(packets_json.encode())
        time.sleep(args.batch_rate) 
        
if __name__ == '__main__':
    main()
