#!/bin/bash

# Script usage help message
USAGE="Usage: ./core SAVED_BATCH_FILE"

# Remote VM address
SAVED_BATCH_FILE=${1?$USAGE}

cmake .
make
./core $SAVED_BATCH_FILE
