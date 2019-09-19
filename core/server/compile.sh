#!/usr/bin/bash

PROTO_FILE_DIR=../../common
PROTO_FILE_NAME=message.proto
PROTO_EXEC=protoc
SRC_DIR=src
GEN_DIR=gen

# Compile the protobuf message
$PROTO_EXEC --cpp_out=$GEN_DIR --proto_path=$PROTO_FILE_DIR $PROTO_FILE_NAME
$PROTO_EXEC --python_out=$GEN_DIR --proto_path=$PROTO_FILE_DIR $PROTO_FILE_NAME

# Compiling the C++ server
g++ $SRC_DIR/protobuf_server.cpp $GEN_DIR/message.pb.cc -pthread -lprotoc -lprotobuf -o test_server
