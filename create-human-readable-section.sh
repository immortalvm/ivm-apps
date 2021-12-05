#!/bin/bash
#
# This script creates the human readable section to be stored together with the digital storage medium.
# The section consist of:
#  - Guide: How to implement the virtual machine and decode the initial program.
#  - IVM instruction set architecture
#  - The initial program: hex encoded

out=$1
if [ "$out" == "" ] ; then
    out="./human-readable-section"
fi

rm -rf $out
mkdir $out

# Get docs
cp ../ivm-doc/guide.pdf $out || { echo "ERROR: guide not found" ; exit 1; }
cp ../ivm-doc/ivmisa.pdf $out || { echo "ERROR: ivm isa not found" ; exit 1; }

# Hex encode application
# Here we target piql film as output device 4096x2160 with 34 pixels border, change this
# to target other devices
w=$((4096 - 34*2))
h=$((2160 - 34*2))
fontsize=20
app="build/ivm/ivm_xz/self_extract.b"

../ivm-hex-dump/create-human-readable-app.sh $w $h $fontsize $out/ip $app

# Create package for distribution
tar czf human-readable-section.tgz $out/*pdf $out/ip/*.png

# Validate
../ivm-hex-dump/ivm-hex-dump -d -i $out/ip | diff $app -
if [ "$?" == "0" ] ; then
    echo "Validation OK"
fi
