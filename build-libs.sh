#!/bin/bash
#
# Build libs, either for LNX or iVM

ROOT=$PWD

declare -a TARGETS
declare -a COMPILERS

if [ "$1" == "lnx" ] ; then
    TARGETS=(lnx)
    COMPILERS=(gcc)
elif [ "$1" == "ivm" ] ; then
    TARGETS=(ivm)
    COMPILERS=(ivm64-gcc)
else
    TARGETS=(ivm lnx)
    COMPILERS=(ivm64-gcc gcc)
fi

for ((i=0; i<${#TARGETS[*]}; i++)) do 

    BUILDROOT=$PWD/build/${TARGETS[$i]}
    COMPILER=${COMPILERS[$i]}

    rm -rf $BUILDROOT
    mkdir -p $BUILDROOT
    
    pushd $BUILDROOT
    mkdir boxing
    pushd boxing
    CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" $ROOT/../boxing/configure --host ivm64 --prefix=$BUILDROOT/boxing
    make && make install
    popd
    
    mkdir afs
    pushd afs
    CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" $ROOT/../afs/configure --host ivm64 --prefix=$BUILDROOT/boxing LIBBOXING_DIR=$BUILDROOT/boxing
    make && make install
    popd
    
    mkdir zlib-1.2.11
    pushd zlib-1.2.11 
    CC=$COMPILER $ROOT/../zlib-1.2.11/configure --static --prefix=$BUILDROOT/file-format-decoders
    make && make install
    popd
    
    mkdir tiff-4.1.0
    pushd tiff-4.1.0
    CC=$COMPILER $ROOT/../tiff-4.1.0/configure --host ivm64 --prefix=$BUILDROOT/file-format-decoders
    make && make install
    popd
    
    popd
    
done
