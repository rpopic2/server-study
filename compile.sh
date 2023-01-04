#!/bin/bash
# Author : rpopic2
# Compiles server and client with no fuss

SCRIPT_NAME=$0
function usage {
    >&2 cat << EOF
Usage : $SCRIPT_NAME <path>
Provide <path> where server.c, client.c is contained.
EOF
    exit $1
}

if test -z $1; then
    usage 1
fi

SERVER_PATH=$(realpath $1)/server.c
CLIENT_PATH=$(realpath $1)/client.c
if test ! -f $SERVER_PATH || test ! -f $CLIENT_PATH; then
    echo $SERVER_PATH or $CLIENT_PATH does not exist.
    echo $(pwd)
    usage 2
fi

function compile {
    clang $1 -o $2
    if test 0 -ne $?; then
        echo Error occured while compiling $1
        exit 3
    fi
}
compile $SERVER_PATH server
compile $CLIENT_PATH client

