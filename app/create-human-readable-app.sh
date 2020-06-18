#!/bin/bash
#
# Create human readable version of app
#

bin=$1

if [ "$bin" == "" ] ; then
    bin="ivmapp.b"
fi

name=$(basename $bin .b)

charwidth=650

base64 -w $charwidth $bin  > $name.base64

echo "Dimensions: $((4096-32*2))x$((2160-32*2))"

head -100 $name.base64 | ./text2pdf -v5 -c$charwidth -x$((4096-32*2)) -y$((2160-32*2)) > $name.pdf

