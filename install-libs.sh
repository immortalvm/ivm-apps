#!/bin/bash

if [ "$1" = "ssh" ]; then
    GIT_METHOD="git@github.com:"
else
    GIT_METHOD="https://github.com/"
fi

abort_with_error() {
    REASON="$1"
    echo
    echo "ERROR: The installation could not be completed, because $REASON."
    echo
    exit 1
}

# Get code
echo "This will check-out and install all needed libraries one directory up."
# Allow skipping the prompt
if [[ $* != *-y* ]]; then
    echo "Press <enter> to continue"
    read
fi
pushd ..
wget http://www.zlib.net/fossils/zlib-1.2.12.tar.gz || abort_with_error "downloading zlib failed"
tar zxvf zlib-1.2.12.tar.gz  || abort_with_error "unpacking zlib failed"
wget -qO- http://ijg.org/files/jpegsrc.v9d.tar.gz | tar xzf - || abort_with_error "downloading and unpacking the jpeg source code failed"
git clone ${GIT_METHOD}immortalvm/testdata.git || abort_with_error "cloning the testdata repository failed"
git clone ${GIT_METHOD}immortalvm/boxing.git || abort_with_error "cloning the boxing repository failed"
git clone ${GIT_METHOD}immortalvm/afs.git || abort_with_error "cloning the afs repository failed"
git clone ${GIT_METHOD}immortalvm/tiff-4.1.0 || abort_with_error "cloning the tiff repository failed"
git clone ${GIT_METHOD}immortalvm/ivm-ghostscript || abort_with_error "cloning the ivm-ghostscript repository failed"
git clone ${GIT_METHOD}immortalvm/ivm-doc || abort_with_error "cloning the ivm-doc repository failed"
git clone ${GIT_METHOD}immortalvm/ivm-hex-dump || abort_with_error "cloning the ivm-hex-dump repository failed"

# Get compilers - get different versions for regression testing
wget https://github.com/immortalvm/ivm-compiler/releases/download/2.0/gcc-10.2.0-ivm64-2.0-linux.zip
unzip gcc-10.2.0-ivm64-2.0-linux.zip
wget https://github.com/immortalvm/ivm-compiler/releases/download/1.2rc1/gcc-8.3.0-ivm64-1.2rc1-linux.zip
unzip gcc-8.3.0-ivm64-1.2rc1-linux.zip

# Get assembler
declare -a asmver=(v0.36 v0.37)
for a in ${asmver[@]}; do
    asm="${a}_linux-x64"
    wget https://github.com/immortalvm/ivm-implementations/releases/download/$a/$asm.zip
    mkdir ivm-$asm
    pushd ivm-$asm
    unzip ../$asm.zip
    chmod a+rx *
    popd
done

# Get vm implementations
git clone ${GIT_METHOD}immortalvm/ivm-implementations.git || abort_with_error "cloning the ivm-implementations repository failed"
git clone ${GIT_METHOD}immortalvm/yet-another-ivm-emulator.git || abort_with_error "cloning the yet-another-ivm-emulator repository failed"

pushd jpeg-9d
cat << EOF > ivm64.patch
diff --git a/config.sub b/config.sub
index 9ccf09a..dae85e6 100755
--- a/config.sub
+++ b/config.sub
@@ -314,6 +314,7 @@ case $basic_machine in
 	| visium \\
 	| wasm32 \\
 	| x86 | xc16x | xstormy16 | xtensa \\
+    | ivm64 \\
 	| z8k | z80)
 		basic_machine=$basic_machine-unknown
 		;;
EOF

patch -p1 < ivm64.patch || abort_with_error "patching the jpeg source code failed"
popd

pushd testdata
./create-testdata.sh || abort_with_error "creating the test data failed"
popd

pushd ivm-doc
make || abort_with_error "building ivm-doc failed"
popd

pushd ivm-hex-dump
./test.sh || abort_with_error "testing ivm-hex-dump failed"
popd

popd

echo "Installation complete"
