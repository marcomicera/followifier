import argparse
import os
import pprint
import select
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


def service_replay(port, prefix, num_timeslots, trace_dir):
    threads = []
    for batch_index in range(num_timeslots + 1):
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
    socket.setblocking(0)
    while True:
        ready = select.select([socket], [], [], 40)
        if ready[0]:
            data = socket.recv(2048)
            print("{} has just received a {} bytes packet.".format(threading.currentThread().getName(), len(data)))
            if not data:
                break
            message += data
        else:
            print("{} did not receive the whole batch. Terminating...".format(threading.currentThread().getName()))
            return
    print("{} received a total of {} bytes.".format(threading.currentThread().getName(), len(message)))
    with open(filename, 'wb') as f:
        f.write(message)
        print("{} saved a batch in {}.".format(threading.currentThread().getName(), filename))


def service_capture(port, prefix, num_timeslots, trace_dir):
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
    while not all(counter >= num_timeslots for counter in counters.values()):
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
        print("Capturing {}...".format(filename))
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
parser.add_argument('--num-timeslots', type=int, default=10, help="Number of batches to capture or replay.")
parser.add_argument('--file-prefix', type=str, default="batch_", help="Prefix for saved files.")
parser.add_argument('--trace-dir', type=str, default=datetime.now().strftime("%Y-%m-%d-%H-%M-%S"),
                    help="Where to save/retrieve batches.")
args = parser.parse_args()

print("Activating {} mode...".format(args.mode))
modes[args.mode](args.port, args.file_prefix, args.num_timeslots, args.trace_dir)
