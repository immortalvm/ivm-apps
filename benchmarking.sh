#!/bin/bash
#
#

ROOT=$PWD
#IVM_EMU=$ROOT/../ivm-implementations/OtherMachines/vm
IVM_EMU=$ROOT/../yet_another_ivm_emulator/ivm_emu_fast_io_parallel
LOGFILE=$ROOT/benchmark.log
TIME="/usr/bin/time --output=$LOGFILE --append --format \"$(date -I);%C;%E;%P\""

# Start log
if [ ! -f "$LOGFILE" ] ; then
    echo "date;command;real time;CPU usage" > $LOGFILE
fi

pushd build/ivm/app
$TIME -- $IVM_EMU ivmapp.b -o out -i ../../../../testdata/reel/png/ -m $((1024*1024*1024))
popd 

# static_tiff decode test
pushd tests/static_tiff
#make clean && make
#ivm as  --noopt --bin "static_tiff.b" --sym "static_tiff.sym" static_tiff
$TIME -- $IVM_EMU ./static_tiff.b
popd

