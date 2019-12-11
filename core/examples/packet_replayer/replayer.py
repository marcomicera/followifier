import argparse
import pprint
import threading
import socket
import os
import sys


def replay(filename, port):
    socket = socket.socket(socket.AF_INET, socket.AF_STREAM)
    with open(filename, 'rb') as f:
        data = f.read()
    socket.send(data)

def service_replay(port, prefix, batch_number):
    threads = []
    for board_num in range(3):
        for batch_index in range(batch_number):
            filename = "{}_{}_{}".format(prefix, board_num, batch_number)
            print("Sending {} to server.".format(filename))
            t = threading.Thread(target=replay, args=(filename, port))
            t.start()
            threads.append(t)
    for t in threads:
        t.join()

def capture(socket, filename):
    message = b""
    while True:
        data = socket.recv(2048)
        print("{} received {}".format(threading.currentThread().getName(), len(data)))
        if not data:
            break
        message += data
    print("{} got {} bytes.".format(threading.currentThread().getName(), len(data)))
    with open(filename, 'wb') as f:
        f.write(message)


def service_capture(port, prefix, batch_number):
    board_counter = 0
    counters = {'dummy': 0} # just to init, for the loop
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.bind(("0.0.0.0", port))
    except socket.error as msg:
        print("Error while binding socket: {}".format(msg))
        exit(1)

    s.listen(3)
    print("Socket listening.") 
    threads = []
    while not all(counter >= batch_number for counter in counters.values()): 
        conn, addr = s.accept()
        # Using addr as a unique fingerprint here.
        if addr not in counters:
            counters[addr] = (board_counter, 0)
            board_counter += 1
        else:
            counters[addr][1] += 1
        filename = "{}_{}_{}".format(prefix, *counters[addr])
        print("Capturing {} batch".format(filename))
        t = threading.Thread(target=capture, args=(conn, filename))
        t.start()
        threads.append(t)
    print("Joining threads.")
    for t in threads:
        t.join()
    print("Received following number of batches from these ips:")
    pprint.pprint(counters)

modes = {
        'replay': service_replay,
        'capture': service_capture
        }

parser = argparse.ArgumentParser()
parser.add_argument('mode', choices=['capture', 'replay'])
parser.add_argument('port', type=int, help="port to listen on in capture mode, or to send packets in replay mode.")
parser.add_argument('--batch-number', type=int, default=10, help="Number of batches to capture or replay.")
# setting it as default
# parser.add_argument('--boards-number', type=int, default=3, help="Number of expected sockets")
parser.add_argument('--file-prefix', type=str, default="batch_", help="prefix for saved files.")
args = parser.parse_args()

print("Activating {} mode...".format(args.mode))
modes[args.mode](args.port, args.file_prefix, args.batch_number)
