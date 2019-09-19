# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

project(core)

message(STATUS "Running core's cmake...")
message(STATUS "Source directory is ${CMAKE_SOURCE_DIR}")

# Boost
find_package(Boost 1.67.0 COMPONENTS system filesystem REQUIRED)
if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

# pthread
find_package(Threads REQUIRED)
if (CMAKE_USE_PTHREADS_INIT)
    add_definitions(-DHAVE_PTHREAD)
endif (CMAKE_USE_PTHREADS_INIT)

# Protobuf
set(PROTO_FILE_DIR "../../common")
set(PROTO_EXEC "protoc")
set(PROTO_FILE_NAME "message.proto")
set(GEN_DIR "gen")
#set(SRC_DIR "src")
#SET_SOURCE_FILES_PROPERTIES(${PROTO_SRC} ${PROTO_INCL} PROPERTIES GENERATED TRUE)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "Generating C++ proto stubs...")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE_NAME})
#set(PROTOBUF_C_ARGUMENTS "--cpp_out=${GEN_DIR} --proto_path=${PROTO_FILE_DIR} ${PROTO_FILE_NAME}")
#execute_process(COMMAND ${PROTO_EXEC} ${PROTOBUF_C_ARGUMENTS})
message(STATUS "Proto source file: ${PROTO_SRCS}")
message(STATUS "Proto header file: ${PROTO_HDRS}")

message(STATUS "Generating Python proto stubs...")
protobuf_generate_python(PROTO_PY ${PROTO_FILE_NAME})
#set(PROTOBUF_PYTHON_ARGUMENTS "--python_out=${GEN_DIR} --proto_path=${PROTO_FILE_DIR}")
#execute_process(COMMAND ${PROTO_EXEC} ${PROTOBUF_PYTHON_ARGUMENTS} ${PROTO_FILE_NAME})
message(STATUS "Python generated file: ${PROTO_PY}")

file(GLOB PROTOBUF_MODELS_INCLUDES "gen/message.pb.cc")

# Building executable
add_executable(${PROJECT_NAME} protobuf_server.cpp ${PROTOBUF_MODELS_INCLUDES})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} ${Protobuf_LIBRARIES})
if (Boost_FOUND)
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif ()
