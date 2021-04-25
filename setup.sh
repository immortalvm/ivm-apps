#!/bin/bash

declare -a compilers=($(readlink -f $(dirname $(find .. -iname "ivm64-gcc"))))
declare -a assemblers=($(readlink -f $(dirname $(find .. -iname "ivm"))))

i=0
for c in ${compilers[@]}; do
    echo "$i: $c"
    i=$((i+1))
done
read -p "Select compiler: " c
IVMCC=${compilers[$c]}
echo "You selected $IVMCC"
echo

i=0
for c in ${assemblers[@]}; do
    echo "$i: $c"
    i=$((i+1))
done

read -p "Select assembler: " c
IVM=${assemblers[$c]}
echo "You selected $IVM"

export PATH=$PATH:$IVMCC:$IVM
export TESTDATA_DIR=$PWD/../testdata/reel/png

echo "PATH=$PATH"

