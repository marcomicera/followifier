# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_FLAGS "-g -Wall -Werror")
project(core)

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

# Protobuf (https://github.com/shaochuan/cmake-protobuf-example)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../common
        ${CMAKE_CURRENT_BINARY_DIR}/../../../common)
#include(../../../common/CMakeLists.txt)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Building executable
add_executable(${PROJECT_NAME} protobuf_server.cpp)
target_link_libraries(${PROJECT_NAME} proto ${PROTOBUF_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
if (Boost_FOUND)
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif ()
