#!/usr/bin/fish

set PROTO_FILE_DIR ../../common
set PROTO_FILE_NAME message.proto
set PROTO_EXEC /usr/local/bin/protoc
set GEN_DIR gen
set SRC_DIR src

# Compile the protobuf message
$PROTO_EXEC --cpp_out=$GEN_DIR --proto_path=$PROTO_FILE_DIR $PROTO_FILE_NAME
$PROTO_EXEC --python_out=$GEN_DIR --proto_path=$PROTO_FILE_DIR $PROTO_FILE_NAME

# Compiling the C++ server
g++ $SRC_DIR/protobuf_server.cpp $GEN_DIR/message.pb.cc -pthread -lprotoc -lprotobuf -o test_server
