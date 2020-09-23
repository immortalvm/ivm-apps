#!/bin/bash

if [ $# != 2 ]; then
    echo "usage: <binary file> <label name>"
    exit 1
fi

if [ ! -r $1 ]; then
    echo "error: Cannot read file: " $1
    exit 1
fi

printf $2
printf ":\n"
printf "data1 [ "
hexdump -v -e '"0x" 1/1 "%02X" " "' $1
printf "]\n"

