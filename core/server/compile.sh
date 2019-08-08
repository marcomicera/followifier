#!/usr/bin/fish

g++ protobuf_server.cpp message.pb.cc  -lprotoc -lprotobuf -o test_server
