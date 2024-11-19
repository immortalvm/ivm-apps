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

clone() {
    if [ ! -d $2 ]; then
        echo clone $2...
        git clone -q ${GIT_METHOD}$1/$2 || abort_with_error "Cloning the $2 repository failed"
    fi
}

dl() {
    if [ ! -f "${1##*/}" ]; then
        echo dl ${1##*/}...
        wget -q "$1" || abort_with_error "dl ${1##*/} failed"
    fi
}

dl_unzip() {
    if [ ! -f "${1##*/}" ]; then
        dl "$1"
        echo unzip ${1##*/}...
        if [ -z "$2" ]; then
            unzip -q "${1##*/}"
        else
            unzip -q "${1##*/}" -d "$2"
        fi
    fi
}

dl_untar() {
    if [ ! -f "${1##*/}" ]; then
        dl "$1"
        echo "(un)tar ${1##*/}..."
        tar xzf ${1##*/} || abort_with_error "unpacking ${1##*/} failed"
    fi
}

# Get code
echo "This will check-out and install all needed libraries one directory up."
# Allow skipping the prompt
if [[ $* != *-y* ]]; then
    echo "Press <enter> to continue"
    read
fi
pushd ..
dl_untar http://www.zlib.net/fossils/zlib-1.2.12.tar.gz
dl_untar http://ijg.org/files/jpegsrc.v9d.tar.gz
clone immortalvm testdata
clone immortalvm boxing
clone immortalvm afs
clone immortalvm tiff-4.1.0
clone immortalvm ivm-ghostscript
clone immortalvm ivm-doc
clone immortalvm ivm-hex-dump
clone immortalvm ROAEshell

# Get compilers - get different versions for regression testing
dl_unzip "https://github.com/immortalvm/ivm-compiler/releases/download/3.4/gcc-12.2.0-ivm64-3.4-linux.zip" "3.4"
dl_unzip "https://github.com/immortalvm/ivm-compiler/releases/download/3.3/gcc-12.2.0-ivm64-3.3-linux.zip" "3.3"
dl_unzip "https://github.com/immortalvm/ivm-compiler/releases/download/3.2/gcc-12.2.0-ivm64-3.2-linux.zip" "3.2"
dl_unzip "https://github.com/immortalvm/ivm-compiler/releases/download/3.1/gcc-12.2.0-ivm64-3.1-linux.zip" "3.1"
dl_unzip "https://github.com/immortalvm/ivm-compiler/releases/download/3.0/gcc-12.2.0-ivm64-3.0-linux.zip" "3.0"
dl_unzip "https://github.com/immortalvm/ivm-compiler/releases/download/2.1/gcc-10.2.0-ivm64-2.1-linux.zip" "2.1"
dl_unzip "https://github.com/immortalvm/ivm-compiler/releases/download/2.0/gcc-10.2.0-ivm64-2.0-linux.zip" "2.0"
dl_unzip "https://github.com/immortalvm/ivm-compiler/releases/download/1.2rc1/gcc-8.3.0-ivm64-1.2rc1-linux.zip" "1.2rc1"

# Get assembler
declare -a asmver=(v0.36 v0.37)
for a in ${asmver[@]}; do
    asm="${a}_linux-x64"
    if [ ! -d ivm-$asm ]; then
        wget -q https://github.com/immortalvm/ivm-implementations/releases/download/$a/$asm.zip
        mkdir ivm-$asm
        pushd ivm-$asm
        unzip -q ../$asm.zip
        chmod a+rx *
        popd
    fi
done

# Get vm implementations
clone immortalvm ivm-implementations
clone immortalvm yet-another-ivm-emulator

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
