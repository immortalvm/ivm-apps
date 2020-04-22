#!/bin/bash
#
#

ROOT=$PWD
IVM_EMU=$ROOT/../ivm-implementations/OtherMachines/vm
LOGFILE=$ROOT/benchmark.log

# Start log
echo "command;real time;CPU usage" > $LOGFILE

# static_tiff decode test
pushd tests/static_tiff
#make clean && make
#ivm as  --noopt --bin "static_tiff.b" --sym "static_tiff.sym" static_tiff
/usr/bin/time --output=$LOGFILE --append --format "%C;%E;%P" -- $IVM_EMU ./static_tiff.b
popd
