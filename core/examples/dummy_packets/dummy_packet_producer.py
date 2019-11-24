import argparse
import random
import socket
import string
import sys
import time
from datetime import datetime

sys.path.insert(1, '../../server/gen')
import message_pb2

from functools import wraps

hexdigits = '0123456789abcdef'


def gen_random_mac():
    return ':'.join([''.join(random.choices(population=hexdigits, k=2)) for _ in range(6)])


def gen_random_ssid():
    return ''.join(random.choices(population=string.ascii_uppercase, k=8))


def gen_random_hash():
    return ''.join(random.choices(population=hexdigits, k=64)).encode('utf-8')


def gen_random_rssi():
    return random.randint(-90, 0)

def gen_random_timestamp(rate):
    now = datetime.now()
    timestamp = datetime.timestamp(now) - random.randint(0, rate)
    return (round(timestamp))



def timer(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        res = func(*args, **kwargs)
        print(f"Function call took {time.perf_counter() - start}")
        return res

    return wrapper


def gen_dummy_batch_base(batch_size, rate):
    batch = message_pb2.Batch()
    for _ in range(batch_size):
        fill_message(batch.messages.add(), rate)
    return batch


def gen_dummy_batch(batch, mac, common_hashes, a):
    used_common_hashes = 0
    batch.boardMac = mac
    for message in batch.messages:

        # Setting a random RSSI
        if a:
            message.metadata.rssi = -62;
        else:
            message.metadata.rssi = -51;
        

        # Introduce a common frame hash as long as there are some available
        if len(common_hashes) > used_common_hashes:
            message.frame_hash = common_hashes[used_common_hashes]
            used_common_hashes += 1
        else:
            message.frame_hash = gen_random_hash()

    return batch


def fill_message(message, rate):
    message.metadata.deviceMac = gen_random_mac()
    message.metadata.ssid = gen_random_ssid()
    message.metadata.timestamp = gen_random_timestamp(rate)
    # message.frame_hash = gen_random_hash()
      


def main():
    # Parsing arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('ip', action='store', help="server IP")
    parser.add_argument('port', action='store', type=int, help='server port')
    parser.add_argument('--batch_size', action='store', type=int, help='number of packets produced per batch',
                        default=20)
    parser.add_argument('--batch_rate', action='store', type=int, help='period in seconds between subsequent batches',
                        default=30)
    parser.add_argument('--boards_mac', nargs='+', help='mac of boards', required=True)
    args = parser.parse_args()

    # Packet generation loop
    while True:

        # Generate a batch base message on which batches will be generated
        batch_base = gen_dummy_batch_base(args.batch_size, args.batch_rate)

        # Generating a random number of common frame hashes for this round
        num_common_hashes = random.randrange(args.batch_size)  # from 0 to `args.batch_size`
        common_hashes = []
        for _ in range(num_common_hashes):
            common_hashes.append(gen_random_hash())
        print("\nNew round, " + str(num_common_hashes) + " messages will be stored in the database.")

        # Simulating multiple boards
        check = True
        for board_mac_address in args.boards_mac:
            try:
                batch = gen_dummy_batch(batch_base, board_mac_address, common_hashes, check).SerializeToString()
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.connect((args.ip, args.port))
                s.send(batch)
                s.send(b'\n\r\n\r')
                print("Board " + board_mac_address + " has sent a batch.")
                s.close()
                check = not check
            except ConnectionRefusedError:
                print("Server not available.")

        # Simulating sniffing period
        time.sleep(args.batch_rate)


if __name__ == '__main__':
    main()
