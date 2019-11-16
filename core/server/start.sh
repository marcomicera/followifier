#!/bin/bash

# Building the project
cmake .
make

# Allowing inbound and outbound TCP connections on the specified port
PORT=$(python3 -c "import json; print(json.load(open('./config.json'))['port'])")
echo Asking password to allow TCP inbound and outbound connections on port "$PORT" on this machine...
sudo iptables -A INPUT -p tcp --dport "$PORT" -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
sudo iptables -A OUTPUT -p tcp --sport "$PORT" -m conntrack --ctstate ESTABLISHED -j ACCEPT

# Launching the server
./core
