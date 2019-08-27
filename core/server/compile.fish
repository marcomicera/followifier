#!/usr/bin/fish

set PROTO_FILE_DIR ../../common
set PROTO_FILE_NAME message.proto
set PROTO_EXEC /usr/local/bin/protoc

# Compile the protobuf message
$PROTO_EXEC --cpp_out=. --proto_path=$PROTO_FILE_DIR $PROTO_FILE_NAME
$PROTO_EXEC --python_out=. --proto_path=$PROTO_FILE_DIR $PROTO_FILE_NAME

# Compiling the C++ server
g++ protobuf_server.cpp message.pb.cc -pthread -lprotoc -lprotobuf -o test_server
