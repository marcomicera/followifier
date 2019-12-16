import argparse
import os
import pprint
import socket
import threading
import time
from datetime import datetime


def replay(filename, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(("127.0.0.1", port))
        with open(filename, 'rb') as f:
            data = f.read()
        s.send(data)
    print("Sent {} bytes from {}".format(len(data), threading.currentThread().getName()))


def service_replay(port, prefix, batch_number, trace_dir):
    threads = []
    for batch_index in range(batch_number):
        for board_num in range(3):
            filename = "{}/{}b{}_t{}".format(trace_dir, prefix, board_num, batch_index)
            if os.path.exists(filename):
                print("Sending {} to server.".format(filename))
                t = threading.Thread(target=replay, args=(filename, port))
                t.start()
                threads.append(t)
            else:
                print("File {} does not exist: please make sure you specify the trace directory "
                      "with the --trace-dir flag.".format(filename))
        time.sleep(60)
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


def service_capture(port, prefix, batch_number, trace_dir):
    board_counter = 0
    counters = {'dummy': 0}  # just to init, for the loop
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        s.bind(("0.0.0.0", port))
    except socket.error as msg:
        print("Error while binding socket: {}".format(msg))
        exit(1)

    s.listen(3)
    print("Socket listening.")
    threads = []
    if not os.path.exists(trace_dir):
        os.makedirs(trace_dir)
    while not all(counter >= batch_number for counter in counters.values()):
        conn, addr = s.accept()
        addr = addr[0]
        # Using addr as a unique fingerprint here.
        if addr not in counters:
            counters[addr] = [board_counter, 0]
            board_counter += 1
        else:
            counters[addr][1] += 1
        pprint.pprint(counters)
        filename = "{}/{}b{}_t{}".format(trace_dir, prefix, *counters[addr])
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
parser.add_argument('mode', choices=['capture', 'replay'], help='')
parser.add_argument('port', type=int, help="Port to listen on in capture mode, or to send packets in replay mode.")
parser.add_argument('--batch-number', type=int, default=10, help="Number of batches to capture or replay.")
parser.add_argument('--file-prefix', type=str, default="batch_", help="Prefix for saved files.")
parser.add_argument('--trace-dir', type=str, default=datetime.now().strftime("%Y-%m-%d-%H-%M-%S"),
                    help="Where to save/retrieve batches.")
args = parser.parse_args()

print("Activating {} mode...".format(args.mode))
modes[args.mode](args.port, args.file_prefix, args.batch_number, args.trace_dir)
