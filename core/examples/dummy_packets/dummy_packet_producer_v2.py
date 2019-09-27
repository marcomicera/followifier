import argparse
import datetime
import json
import random
import socket
import string
import time
import sys
from scapy.all import IP, TCP, send, sr1

sys.path.insert(1, '../../server/gen')
import message_pb2

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
    def __init__(self, mac=None, ssid=None, timestamp=None, frame_hash=None, rsi=None):
        self.mac = mac or gen_random_mac()
        self.ssid = ssid or gen_random_ssid()
        self.timestamp = timestamp or gen_random_timestamp()
        self.frame_hash = frame_hash or gen_random_hash()
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

@timer
def produce_protobuf_batch(batch_size):
    batch = message_pb2.Batch()
    for _ in range(batch_size):
        fill_message(batch.messages.add())
    return batch

def fill_message(message):
    p = DummyPacket()
    for k, v in p.__dict__.items():
        setattr(message, k, v)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--ip', action='append', help="server IP")
    parser.add_argument('port', action='store', type=int, help='server port')
    parser.add_argument('--batch_size', action='store', type=int, help='number of packets produced per batch',
                        default=100)
    parser.add_argument('--batch_rate', action='store', type=int, help='period in seconds between subsequent batches',
                        default=1)
    parser.add_argument('--protobuf', action='store_true', help='Enable this switch for protocol buffer serialization (default JSON)',
                        default=False)
    args = parser.parse_args()

    target = args.ip
    port = int(args.port)
    # UDP would probably be more suited
    while True:
        batch = produce_protobuf_batch(args.batch_size).SerializeToString() if args.protobuf else produce_json_batch(args.batch_size).encode()
        for ip in args.ip:
            print(ip)
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((ip, args.port))
            s.send(batch)
            print('ok')
            s.send(b"\n\r\n\r")
            s.close()
        print(len(batch))
        time.sleep(args.batch_rate)

if __name__ == '__main__':
    main()
