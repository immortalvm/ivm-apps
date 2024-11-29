. ./setup.sh "$IVM10_BIN" "$IVM_ASM_DIR"
./build.sh -t ivm -b lib
PATH="$IVM12_BIN:$PATH" CXX=ivm64-g++ ./build.sh -t ivm -b app
