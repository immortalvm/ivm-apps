
# Compile
ivm64-gcc main.c \
          -I ../../file-format-decoders/include/ \
          -I ../../../testdata/tiff/ \
          -L ../../file-format-decoders/lib/ \
          -ltiff \
          -lm \
          -I ../../ivm_io/ \
          ../../ivm_io/ivm_io.o

# Output device
mkdir out

# Run with ivm C implementation
sed -i -e 's/as-run  --noopt -m $MAXMEM//' a.out
../../../ivm-implementations/OtherMachines/vm a.b -o out

# Run with C# implementation
sed -i -e 's/1500000000/15000000/' a.out ; ./a.out

# View result
eog out/00000001.png 
