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
        self.apMac = mac or gen_random_mac()
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

def produce_protobuf_dummy_batch(batch_size):
    batch = message_pb2.Batch()
    for _ in range(batch_size):
        fill_message(batch.messages.add())
    return batch

def produce_protobuf_batch(batch, mac):
    batch.boardMac = mac
    for message in batch.messages:
        message.rsi = gen_random_rsi()
    return batch

def fill_message(message):
    p = DummyPacket()
    for k, v in p.__dict__.items():
        setattr(message, k, v)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('ip', action='store', help="server IP")
    parser.add_argument('port', action='store', type=int, help='server port')
    parser.add_argument('--batch_size', action='store', type=int, help='number of packets produced per batch',
                        default=100)
    parser.add_argument('--batch_rate', action='store', type=int, help='period in seconds between subsequent batches',
                        default=1)
    parser.add_argument('--boards_number', action='store', type=int, help='number of dummy boards',
                        default=1)
    args = parser.parse_args()
    port = int(args.port)
    macs = []
    # UDP would probably be more suited
    for i in range(0, args.boards_number):
        macs.append(gen_random_mac())
    while True:
        dummy_batch = produce_protobuf_dummy_batch(args.batch_size)
        for mac in macs:
            batch = produce_protobuf_batch(dummy_batch, mac).SerializeToString()
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((args.ip, args.port))
            #add '\0' because last byte is removed from server since board adds it
            s.send(batch+b'\0')
            s.send(b'\n\r\n\r')
            print("Sent batch from board with mac " + mac)
            s.close()
        time.sleep(args.batch_rate)

if __name__ == '__main__':
    main()
