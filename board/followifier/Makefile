#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := simple_sniffer

EXTRA_COMPONENT_DIRS := $(IDF_PATH)/examples/system/console/components

include $(IDF_PATH)/make/project.mk

CFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib -lprotobuf-c
