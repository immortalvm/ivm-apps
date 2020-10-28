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

if [ "$BUILD" != "app" ] && [ "$BUILD" != "lib" ] && [ "$BUILD" != "all" ]; then
    echo "Illegal build target: $BUILD";
fi


echo "TARGETS: ${TARGETS[*]}"
echo "BUILDING: ${BUILD[*]}"

# Build LIBS?
if [ "$BUILD" == "lib" ] || [ "$BUILD" == "all" ]; then
    for ((i=0; i<${#TARGETS[*]}; i++)) do 

        BUILDROOT=$ROOT/build/${TARGETS[$i]}
        COMPILER=${COMPILERS[$i]}
        HOST=${HOSTS[$i]}
        
        [ "$CLEAN" == "1" ] && rm -rf $BUILDROOT
        
        mkdir -p $BUILDROOT
        
        pushd $BUILDROOT
        
        echo "===== BOXING ====="
        [ ! -d "./boxing" ] && mkdir boxing
        pushd boxing
        [ ! -f "$ROOT/../boxing/configure" ] && { (echo "Running BOXING configure"; cd $ROOT/../boxing ; ./autogen.sh) || { popd; exit 1; } }
        [ ! -f "./Makefile" ] && CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" $ROOT/../boxing/configure $HOST --prefix=$BUILDROOT/boxing
        { make && make install; } || { popd ; exit 1; }
        popd
        
        echo "===== AFS ====="
        [ ! -d "./afs" ] && mkdir afs
        pushd afs
        [ ! -f "$ROOT/../afs/configure" ] && { (echo "Running AFS configure"; cd $ROOT/../afs ; ./autogen.sh) || { popd; exit 1; } }
        [ ! -f "./Makefile" ] && { CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" $ROOT/../afs/configure $HOST --prefix=$BUILDROOT/afs LIBBOXING_DIR=$BUILDROOT/boxing; }
        { make && make install; }  || { popd ; exit 1; }
        popd
        
        echo "===== ZLIB ====="
        [ ! -d zlib-1.2.11 ] && mkdir zlib-1.2.11
        pushd zlib-1.2.11 
        [ ! -f "./Makefile" ] && { CC=$COMPILER $ROOT/../zlib-1.2.11/configure --static --prefix=$BUILDROOT/file-format-decoders; }
        { make && make install; } || { popd ; exit 1; }
        popd

        echo "===== JPEG ====="
        [ ! -d jpeg-9d ] && mkdir jpeg-9d
        pushd jpeg-9d 
        [ ! -f "./Makefile" ] && { CC=$COMPILER $ROOT/../jpeg-9d/configure --enable-shared=no --enable-static=yes $HOST --prefix=$BUILDROOT/file-format-decoders; }
        { make && make install; } || { popd ; exit 1; }
        popd
        
        echo "===== TIFF ====="
        [ ! -d "./tiff-4.1.0" ] && mkdir tiff-4.1.0
        pushd tiff-4.1.0
        [ ! -f "./Makefile" ] && {
            CC=$COMPILER $ROOT/../tiff-4.1.0/configure \
              $HOST --prefix=$BUILDROOT/file-format-decoders \
              --enable-shared=no \
              --with-zlib-include-dir=$BUILDROOT/file-format-decoders/include \
              --with-zlib-lib-dir=$BUILDROOT/file-format-decoders/lib \
              --with-jpeg-include-dir=$BUILDROOT/file-format-decoders/include \
              --with-jpeg-lib-dir=$BUILDROOT/file-format-decoders/lib;
            { make && make install; } || { popd ; exit 1; }
        }
        
        popd
        
        popd
        
    done
fi

if [ "$BUILD" == "app" ] || [ "$BUILD" == "all" ]; then
    
    for ((i=0; i<${#TARGETS[*]}; i++)) do 

        BUILDROOT=$ROOT/build/${TARGETS[$i]}
        COMPILER=${COMPILERS[$i]}
        HOST=${HOSTS[$i]}

        [[ ! -d $BUILDROOT ]] && mkdir -p $BUILDROOT
        [ ! -f "$ROOT/configure" ] && { pushd $ROOT; ./autogen.sh ; popd; }

        echo "Building in $BUILDROOT"
        pushd $BUILDROOT || { echo "ERROR: $BUILDROOT does not exist"; exit 1; } 
        [ ! -f "./Makefile" ] && { CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" $ROOT/configure $HOST LIBBOXING_DIR=$BUILDROOT/boxing/lib LIBAFS_DIR=$BUILDROOT/afs/lib LIBTESTDATA_DIR=$ROOT/../testdata   ; }
        make 
        popd
        
    done
fi

