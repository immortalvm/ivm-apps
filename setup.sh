#!/bin/bash
if [ -z "$1" ]; then
    declare -a compilers=($(readlink -f $(dirname $(find .. -iname "ivm64-gcc"))))
    i=0
    for c in ${compilers[@]}; do
        echo "$i: $c"
        i=$((i+1))
    done
    read -p "Select compiler: " c
    IVMCC=${compilers[$c]}
else
    IVMCC="$1"
fi
echo "You selected $IVMCC"
echo

if [ -z "$2" ]; then
    declare -a assemblers=($(readlink -f $(dirname $(find .. -iname "ivm"))))
    i=0
    for c in ${assemblers[@]}; do
        echo "$i: $c"
        i=$((i+1))
    done
    read -p "Select assembler: " c
    IVM=${assemblers[$c]}
else
    IVM="$2"
fi
echo "You selected $IVM"

export PATH=$PATH:$IVMCC:$IVM:$PWD/../ivm-hex-dump
export TESTDATA_DIR=$PWD/../testdata/reel/png
export IVM_EMULATOR_INPUT=$PWD/../testdata/reel/png

echo "PATH=$PATH"

