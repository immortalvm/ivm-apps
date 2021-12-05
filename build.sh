#!/bin/bash
#
# Build libs, either for LNX or iVM

ROOT=$PWD

declare -a TARGETS
declare -a COMPILERS
CLEAN=0
TARGET="ivm"
BUILD="app"
FIXMATH=""

help() {
    echo "Build iVM libraries"
    echo "$0 [-t|--target <lnx|ivm|all>] [-b|--build <lib|app|vm|all>] [-c|--clean] [-f|--fixmath] [-h|--help]" 
}


while [[ "$#" -gt 0 ]]; do
    case $1 in
        -b|--build) BUILD="$2"; shift ;;
        -t|--target) TARGET="$2"; shift ;;
        -c|--clean) CLEAN=1 ;;
        -f|--fixmath) FIXMATH="--with-fixmath32" ;;
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

if [ "$BUILD" != "app" ] && [ "$BUILD" != "lib" ] && [ "$BUILD" != "vm" ] && [ "$BUILD" != "all" ]; then
    echo "Illegal build target: $BUILD";
fi


echo "TARGETS: ${TARGETS[*]}"
echo "BUILDING: ${BUILD[*]}"

# Build VMs?
if [ "$BUILD" == "vm" ] || [ "$BUILD" == "all" ]; then
    pushd $ROOT/../yet-another-ivm-emulator
    make || { popd ; exit 1; }
    popd
    pushd $ROOT/../yet-another-ivm-emulator
    make || { popd ; exit 1; }
    popd
fi

# Build LIBS?
if [ "$BUILD" == "lib" ] || [ "$BUILD" == "all" ]; then
    for ((i=0; i<${#TARGETS[*]}; i++)) do 

        CURRENT_TARGET=${TARGETS[$i]}
        BUILDROOT=$ROOT/build/$CURRENT_TARGET
        COMPILER=${COMPILERS[$i]}
        HOST=${HOSTS[$i]}
        
        [ "$CLEAN" == "1" ] && rm -rf $BUILDROOT
        
        mkdir -p $BUILDROOT
        
        pushd $BUILDROOT

        if [ "$FIXMATH" != "" ] ; then
            echo "===== LIBFIXMATH64 ====="
            [ ! -d "./libfixmath64" ] && mkdir libfixmath64
            pushd libfixmath64
            [ ! -f "$ROOT/../libfixmath64/configure" ] && { (echo "Running LIBFIXMATH configure"; cd $ROOT/../libfixmath64 ; ./autogen.sh) || { popd; exit 1; } }
            [ ! -f "./Makefile" ] && {
                CC=$COMPILER $ROOT/../libfixmath64/configure \
                  $HOST \
                  --prefix=$BUILDROOT/libfixmath64; }
            { make && make install; } || { popd ; exit 1; }
            popd
        fi
        
        echo "===== BOXING ====="
        [ ! -d "./boxing" ] && mkdir boxing
        pushd boxing
        [ ! -f "$ROOT/../boxing/configure" ] && { (echo "Running BOXING configure"; cd $ROOT/../boxing ; ./autogen.sh) || { popd; exit 1; } }
        [ ! -f "./Makefile" ] && {
            CC=$COMPILER \
              CFLAGS="-DBOXING_USE_C99_LIBRARIES" \
              LIBMATHFIX_DIR=$BUILDROOT/libfixmath64 \
              $ROOT/../boxing/configure \
              $HOST \
              $FIXMATH \
              --prefix=$BUILDROOT/boxing; }
        { make && make install; } || { popd ; exit 1; }
        popd
        
        echo "===== AFS ====="
        [ ! -d "./afs" ] && mkdir afs
        pushd afs
        [ ! -f "$ROOT/../afs/configure" ] && { (echo "Running AFS configure"; cd $ROOT/../afs ; ./autogen.sh) || { popd; exit 1; } }
        [ ! -f "./Makefile" ] && {
            CC=$COMPILER \
              CFLAGS="-DBOXING_USE_C99_LIBRARIES" \
              $ROOT/../afs/configure \
              $HOST --prefix=$BUILDROOT/afs \
              LIBBOXING_DIR=$BUILDROOT/boxing; }
        { make && make install; } #|| { popd ; exit 1; }
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

        echo "===== GHOSTSCRIPT ====="
        [ ! -d "./ivm-ghostscript" ] && mkdir ivm-ghostscript
        pushd ivm-ghostscript
        [ ! -f "./Makefile" ] && {
            if [ "$CURRENT_TARGET" == "ivm" ] ; then 
                GS_FLAGS="--with-drivers=ivm64"
                PDFARCH="TARGET_ARCH_FILE=$PWD/arch-config/arch_autoconf.h"
                CC=$COMPILER CFLAGS="-DGS_NO_FILESYSTEM -DCMS_NO_PTHREADS" LDFLAGS="" \
                  $ROOT/../ivm-ghostscript/ghostscript-9.52/configure \
                  --with-memory-alignment=8 \
                  --prefix=$BUILDROOT/file-format-decoders \
                  $HOST --disable-threading --disable-contrib \
                  --disable-fontconfig --disable-dbus --disable-cups $GS_FLAGS
                { make $PDFARCH && make so $PDFARCH && make install; } || { popd ; exit 1; }
            else
                #$ROOT/../ivm-ghostscript/ghostscript-9.52/autogen.sh
                CC=$COMPILER CFLAGS="-DGS_NO_FILESYSTEM -DCMS_NO_PTHREADS" LDFLAGS="" \
                  $ROOT/../ivm-ghostscript/ghostscript-9.52/configure \
                  --with-memory-alignment=8 \
                  --prefix=$BUILDROOT/file-format-decoders \
                  $HOST --disable-threading --disable-contrib \
                  --disable-fontconfig --disable-dbus --disable-cups
                { make && make so && make install; } || { popd ; exit 1; }
            fi
        }
        popd
        
        popd
        
    done
fi

if [ "$BUILD" == "app" ] || [ "$BUILD" == "all" ]; then
    
    [ "$CLEAN" == "1" ] && rm -rf $BUILDROOT/Makefile
    
    for ((i=0; i<${#TARGETS[*]}; i++)) do 

        BUILDROOT=$ROOT/build/${TARGETS[$i]}
        COMPILER=${COMPILERS[$i]}
        HOST=${HOSTS[$i]}

        [[ ! -d $BUILDROOT ]] && mkdir -p $BUILDROOT
        [ ! -f "$ROOT/configure" ] && { pushd $ROOT; ./autogen.sh ; popd; }

        echo "Building in $BUILDROOT"
        pushd $BUILDROOT || { echo "ERROR: $BUILDROOT does not exist"; exit 1; } 
        [ ! -f "./Makefile" ] && {
            CC=$COMPILER CFLAGS="-DBOXING_USE_C99_LIBRARIES" \
              LIBBOXING_DIR=$BUILDROOT/boxing/lib \
              LIBAFS_DIR=$BUILDROOT/afs/lib \
              LIBTESTDATA_DIR=$ROOT/../testdata \
              $ROOT/configure $HOST
        }
        make TESTDATA_DIR=$TESTDATA_DIR
        popd
        
    done
fi

