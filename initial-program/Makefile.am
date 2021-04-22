
VM=vm
TESTDATA_DIR=reel/png
OUT_DIR=out
ivmipdir = testdata
bindir=.
bin_PROGRAMS = ivmip ivmipxz

if ! IVM_COMPILE
# For use with valgrind checker
CLFAGS = -fstack-protector-all -g -O0
endif

ivmip_CFLAGS = \
     $(AM_CFLAGS) $(CLFAGS) \
     -I${top_srcdir} \
     -I${abs_top_builddir}/afs/include \
     -I${abs_top_builddir}/boxing/include \
     -I${abs_top_builddir}/boxing/thirdparty/glib \
     -I${abs_top_builddir}/boxing/thirdparty/minixml/inc \
     -I../file-format-decoders/include \
     -I${top_srcdir}/app \
     -I${top_srcdir}/initial-program \
     -L$(LIBBOXING_DIR)/../../file-format-decoders/lib \
     -L$(LIBBOXING_DIR) \
     -L$(LIBAFS_DIR) \
     -L../app

ivmip_LDADD = \
     -livmapp \
     -lafs \
     -lunboxing \
     -ltiff \
     -ljpeg \
     -lz \
     -lm 

if ! IVM_COMPILE
ivmip_LDADD += \
     -lpng
endif

ivmip_SOURCES = \
     main.c \
     inputdevice.c \
     inputdevice.h \
     settings.h \
     ../afs/afsutil.c \
     ../ivm_io/ivm_io.c


# Self extracting version
ivmipxz_CFLAGS = \
     $(AM_CFLAGS) $(CFLAGS) -DIVM_INCLUDE_APP=0\
     -I${top_srcdir} \
     -I${abs_top_builddir}/afs/include \
     -I${abs_top_builddir}/boxing/include \
     -I${abs_top_builddir}/boxing/thirdparty/glib \
     -I${abs_top_builddir}/boxing/thirdparty/minixml/inc \
     -I${top_srcdir}/app \
     -I${top_srcdir}/initial-program \
     -L$(LIBBOXING_DIR)/../../file-format-decoders/lib \
     -L$(LIBBOXING_DIR) \
     -L$(LIBAFS_DIR)


ivmipxz_LDADD = \
     -lafs \
     -lunboxing \
     -lm 

if ! IVM_COMPILE
ivmipxz_LDADD += \
     -lpng
endif

ivmipxz_SOURCES = \
     main.c \
     inputdevice.c \
     inputdevice.h \
     settings.h \
     ../afs/afsutil.c \
     ../ivm_io/ivm_io.c


check_PROGRAMS = ivmip ivmipxz

#ivmip_DATA = $(TESTDATA_DIR)/*png

install-data-hook:
	echo "mkdir -p out" > $(DESTDIR)/test.sh
	echo "ivm ivmip.b -o out -i ${ivmipdir}  -m 2073741824" >> $(DESTDIR)/test.sh

# Optimized and non optimized asm / test
ivmip.b: ivmip
	ivm as  --noopt --bin "ivmip.b" --sym "ivmip.sym" ivmip
asm: ivmip.b

asmxz: ivmipxz
	ivm as  --bin "ivmipxz.b" --sym "ivmipxz.sym" ivmipxz

asmopt: ivmip
	ivm as --bin "ivmip-opt.b" --sym "ivmip-opt.sym" ivmip

testxz: ivmipxz
	$([ "$TESTDATA_DIR" == "" ] && echo "Warning: No TESTDATA_DIR defined" )
	mkdir -p $(OUT_DIR)
	$(VM) ivmipxz.b -o $(OUT_DIR) -i $(TESTDATA_DIR) -m 2073741824

test: ivmip
	$([ "$TESTDATA_DIR" == "" ] && echo "Warning: No TESTDATA_DIR defined" )
	mkdir -p $(OUT_DIR)
	$(VM) ivmip.b -o $(OUT_DIR) -i $(TESTDATA_DIR) -m 2073741824

testopt: ivmip
	$([ "$TESTDATA_DIR" == "" ] && echo "Warning: No TESTDATA_DIR defined" )
	mkdir -p $(OUT_DIR)
	$(VM) ivmip-opt.b -o $(OUT_DIR) -i $(TESTDATA_DIR) -m 2073741824

.PHONY: test asm asmxz asmopt

all-local: asm


