set -e
WD=$(dirname $(realpath $BASH_SOURCE))
echo $WD
mkdir -p $WD/siard
pushd $WD/siard
# rm -r *
# ivm64-gcc -DBOXING_USE_C99_LIBRARIES -I../../../boxing/inc ../../../boxing/src/log.c -c -o boxing_log.o
# ivm64-gcc -DBOXING_USE_C99_LIBRARIES -I../../../boxing/inc ../../../boxing/thirdparty/glib/gvector.c -c -o gvector.o
# ivm64-g++ ../../../ROAEshell/thirdparty/tinyxml2/tinyxml2.cpp -c -o tinyxml2.o
# ivm64-gcc -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/crc32.c -c -o crc32.o
# ivm64-gcc -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/adler32.c -c -o adler32.o
# ivm64-gcc -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/zutil.c -c -o zutil.o
# ivm64-gcc -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/inftrees.c -c -o inftrees.o
# ivm64-gcc -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/inffast.c -c -o inffast.o
# ivm64-gcc -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/inflate.c -c -o inflate.o
# ivm64-gcc -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/contrib/minizip/ioapi.c -c -o ioapi.o
# ivm64-gcc -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/contrib/minizip/unzip.c -c -o unzip.o
# ivm64-g++ -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/contrib/minizip/ida_miniunz_utils.cpp -c -o ida_miniunz_utils.o
# ivm64-gcc -I../../../ROAEshell/thirdparty/zlib ../../../ROAEshell/thirdparty/zlib/contrib/minizip/ida_miniunz.c -c -o ida_miniunz.o
# ivm64-gcc ../../../ROAEshell/ida/siard2sql/libsiardunzip.c -c -o libsiardunzip.o
# ivm64-g++ -I../../../ROAEshell/thirdparty/tinyxml2 ../../../ROAEshell/ida/siard2sql/libsiardxml.cpp -c -o libsiardxml.o
xxd -i ../../../ROAEshell/db/simpledb.siard > simpledb.h
ivm64-gcc\
 -lm\
 -lstdc++\
 -I../../build/ivm/afs/include\
 -DBOXING_USE_C99_LIBRARIES\
 -I../../../boxing/inc\
 -I../../../boxing/thirdparty/glib\
 -I../..\
 boxing_log.o\
 gvector.o\
 tinyxml2.o\
 crc32.o\
 adler32.o\
 zutil.o\
 inftrees.o\
 inffast.o\
 inflate.o\
 ioapi.o\
 unzip.o\
 ida_miniunz_utils.o\
 ida_miniunz.o\
 libsiardunzip.o\
 libsiardxml.o\
 ../ivm_format_siard_test.c\
 -o ivm_format_siard_test
ivm64-as ivm_format_siard_test
ivm64-emu ivm_format_siard_test.b
popd
