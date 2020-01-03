#!/bin/bash

# Creates a screen daemon if it does not exist already and does not attach to it.
_screen_daemon_no_reattach() { 
  if screen -ls | awk '{print $1}' | grep -q "followifier-$1$"; then
    echo "The $1 module is already running."
  else
    echo "Launching the $1 module..."
    screen -dmS followifier-$1 bash -c "$2; exec bash"
    echo "...$1 launched as a screen daemon."
  fi
}

# Launches the UI
_launch_ui() {
    sudo service mongod start
    _screen_daemon_no_reattach "reader" "node frontend/reader/index.js"
    _screen_daemon_no_reattach "gui" "cd frontend && npm start --prefix interface"
    xdg-open http://localhost:4200/ &
}

# Launches the core server without compiling first
_launch_core() {
    cd core/server && ./core
}

# Compiles and launches the core server
_compile_and_launch_core() {
    cd core/server && ./start.sh
}

_help_message() {
    echo "Usage: $(basename "$0") [options]" >&2
    echo
    echo "  -h, --help              Shows this help message"
    echo "  --compile               Imports home directory's dotfiles"
    echo
    exit 1
}

if [ $# -eq 0 ]; then
    _launch_ui
    _launch_core
else
    for i in "$@"
    do
        case $i in
            --help|-h)
                _help_message
                shift
                ;;
            --compile)
                _launch_ui
                _compile_and_launch_core
                shift
                ;;
            *)
                _help_message
                shift
                ;;
        esac
    done
fi
