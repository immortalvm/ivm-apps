#!/bin/bash
#
# Build libs, either for LNX or iVM

ROOT=$PWD

declare -a TARGETS
declare -a COMPILERS
CLEAN=0
TARGET="ivm"
BUILD="app"

help() {
    echo "Build iVM libraries"
    echo "$0 [-t|--target <lnx|ivm|all>] [-b|--build <lib|app|all>] [-c|--clean] [-h|--help]" 
}


while [[ "$#" -gt 0 ]]; do
    case $1 in
        -b|--build) BUILD="$2"; shift ;;
        -t|--target) TARGET="$2"; shift ;;
        -c|--clean) CLEAN=1 ;;
        -h|--help) help; exit 0 ;;
        *) echo "Unknown parameter passed: $1"; help; exit 1 ;;
    esac
    shift
done

if [ "$TARGET" == "lnx" ] ; then
    TARGETS=(lnx)
    COMPILERS=(gcc)
    HOSTS=("")
elif [ "$TARGET" == "ivm" ] ; then
    TARGETS=(ivm)
    COMPILERS=(ivm64-gcc)
    HOSTS=("--host ivm64")
elif [ "$TARGET" == "all" ]; then
    TARGETS=(ivm lnx)
    COMPILERS=(ivm64-gcc gcc)
    HOSTS=("--host ivm64" "")
else
    echo "Illegal target: $TARGET"
    exit 1
fi


# Build LIBS?
if [ "$BUILD" == "lib" ] || [ "$BUILD" == "all" ]; then
    for ((i=0; i<${#TARGETS[*]}; i++)) do 

        BUILDROOT=$PWD/build/${TARGETS[$i]}
        COMPILER=${COMPILERS[$i]}
        HOST=${HOSTS[$i]}
        
        [ "$CLEAN" == "1" ] && rm -rf $BUILDROOT
        
        mkdir -p $BUILDROOT
        
        pushd $BUILDROOT
        mkdir boxing
        pushd boxing
        CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" $ROOT/../boxing/configure $HOST --prefix=$BUILDROOT/boxing
        make && make install 
        popd
        
        mkdir afs
        pushd afs
        CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" $ROOT/../afs/configure $HOST --prefix=$BUILDROOT/boxing LIBBOXING_DIR=$BUILDROOT/boxing
        make && make install 
        popd
        
        mkdir zlib-1.2.11
        pushd zlib-1.2.11 
        CC=$COMPILER $ROOT/../zlib-1.2.11/configure --static --prefix=$BUILDROOT/file-format-decoders
        make && make install || exit
        popd

        mkdir jpeg-9d
        pushd jpeg-9d 
        CC=$COMPILER $ROOT/../jpeg-9d/configure --enable-shared=no --enable-static=yes $HOST --prefix=$BUILDROOT/file-format-decoders
        make && make install || exit
        popd
        
        mkdir tiff-4.1.0
        pushd tiff-4.1.0
        CC=$COMPILER $ROOT/../tiff-4.1.0/configure \
          $HOST --prefix=$BUILDROOT/file-format-decoders \
          --with-zlib-include-dir=$BUILDROOT/file-format-decoders/include \
          --with-zlib-lib-dir=$BUILDROOT/file-format-decoders/lib \
          --with-jpeg-include-dir=$BUILDROOT/file-format-decoders/include \
          --with-jpeg-lib-dir=$BUILDROOT/file-format-decoders/lib
        
        make && make install || exit
        popd
        
        popd
        
    done
fi

if [ "$BUILD" == "app" ] || [ "$BUILD" == "all" ]; then
    
    for ((i=0; i<${#TARGETS[*]}; i++)) do 

        BUILDROOT=$PWD/build/${TARGETS[$i]}
        COMPILER=${COMPILERS[$i]}

        pushd $BUILDROOT
        CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" $ROOT/configure --host ivm64 LIBBOXING_DIR=$BUILDROOT/boxing/lib
        make 
        popd
        
    done
fi

