#!/bin/bash
#
# Build apps, either for LNX or iVM

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

./autogen.sh

for ((i=0; i<${#TARGETS[*]}; i++)) do 

    BUILDROOT=$PWD/build/${TARGETS[$i]}
    COMPILER=${COMPILERS[$i]}

    pushd $BUILDROOT
    CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" $ROOT/configure --host ivm64 LIBBOXING_DIR=$BUILDROOT/boxing/lib
    make && make install
    popd
    
done
