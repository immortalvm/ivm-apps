

#sed -i -e 's/as-run  --noopt -m //' ivmapp


ivm as  --noopt --bin "ivmapp.b" --sym "ivmapp.sym" ivmapp

mkdir out
../../ivm-implementations/OtherMachines/vm ivmapp.b -o out -i ../../testdata/reel/png/


# ../../../../ivm-implementations/OtherMachines/vm ivmapp.b -o out -i ../../../../testdata/reel/png/ -m $((1024*1024*1024))
