# Get code
echo "This will check-out and install all needed libraries one directory up."
echo "Press <enter> to continue"
read
pushd ..
wget http://www.zlib.net/zlib-1.2.12.tar.gz || (popd;exit 1)
tar zxvf zlib-1.2.12.tar.gz  || (popd; exit 1)
wget -qO- http://ijg.org/files/jpegsrc.v9d.tar.gz | tar xzf - || (popd;exit 1)
git clone https://github.com/immortalvm/testdata.git || (popd;exit 1)
git clone https://github.com/immortalvm/boxing.git || (popd;exit 1)
git clone https://github.com/immortalvm/afs.git || (popd;exit 1)
git clone https://github.com/immortalvm/tiff-4.1.0 || (popd;exit 1)
git clone https://github.com/immortalvm/ivm-ghostscript || (popd;exit 1)
git clone https://github.com/immortalvm/ivm-doc || (popd;exit 1)
git clone https://github.com/immortalvm/ivm-hex-dump || (popd;exit 1)

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
git clone https://github.com/immortalvm/ivm-implementations.git
git clone https://github.com/immortalvm/yet-another-ivm-emulator.git

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

patch -p1 < ivm64.patch || (popd;popd;exit 1)
popd

pushd testdata
./create-testdata.sh || { popd; exit 1; }
popd

pushd ivm-doc
make || { popd; exit 1; }
popd

pushd ivm-hex-dump
./test.sh || { popd; exit 1; }
popd

popd

echo "Install complete"
