#!/bin/bash

reset
cmake --build cmake-build-debug --target app-flash -- -j 4 && make monitor -j 4
