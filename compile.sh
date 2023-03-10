#!/bin/bash
# Author : rpopic2
# Compiles server and client with no fuss

SCRIPT_NAME=$0
function usage {
    >&2 cat << EOF
Usage : $SCRIPT_NAME <path> <additional_file>
Provide <path> where server.c*, client.c* is contained.
EOF
    exit $1
}

if test -z $1; then
    usage 1
fi
ADD_FILE=$(realpath $1)/$2
SERVER_PATH=$(realpath $1)/server.c*
CLIENT_PATH=$(realpath $1)/client.c*
LIB_PATH=$(realpath $1)/client.c*
if test ! -f $SERVER_PATH || test ! -f $CLIENT_PATH; then
    echo $SERVER_PATH or $CLIENT_PATH does not exist.
    echo $(pwd)
    usage 2
fi

function compile {
    EXTENSION=$(basename $1 | cut -d'.' -f2)
    if test $EXTENSION == "cpp"; then
        clang++ $ADD_FILE $1 -Wall -std=c++20 -o $2
    else
        clang $1 -o $2
    fi
    if test 0 -ne $?; then
        echo Error occured while compiling $1
        exit 3
    fi
}
compile $SERVER_PATH server
compile $CLIENT_PATH client

