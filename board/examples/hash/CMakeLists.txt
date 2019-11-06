cmake_minimum_required(VERSION 3.5)
project(hash)

set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/system/console/components)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
add_executable(${PROJECT_NAME} hash.c)
