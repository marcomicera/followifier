# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/system/console/components)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(board)

# Protobuf
set(PROTO_FILE_DIR ../../common)
set(PROTO_EXEC protoc-c)
set(PROTO_FILE_NAME message.proto)
set(GEN_DIR ./main/gen)
file(MAKE_DIRECTORY ${GEN_DIR})
message(STATUS "Generating C proto stubs...")
list(APPEND PROTO_C_ARGS "--c_out=${GEN_DIR}" "--proto_path=${PROTO_FILE_DIR}" "${PROTO_FILE_NAME}")
execute_process(COMMAND ${Protobuf_PROTOC_EXECUTABLE} ${PROTO_C_ARGS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error)
message(STATUS "...done. ${output}")
