
VM=vm
TESTDATA_DIR=reel/png
OUT_DIR=out
ivmipdir = testdata
bindir=.

if !IVM_COMPILE
# For use with valgrind checker
CLFAGS = -fstack-protector-all -g -O0
else
IVM_TARGETS=ivmipxz ivmipscans # xz is self extracting version 
endif
bin_PROGRAMS = ivmip $(IVM_TARGETS)

ivmip_CFLAGS = \
     $(AM_CFLAGS) $(CLFAGS) -DIVM_RAW_TESTDATA=1\
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

if !IVM_COMPILE
LNX_LIBS=-lpng
endif

ivmip_LDADD = \
     -livmapp \
     -lafs \
     -lunboxing \
     -ltiff \
     -ljpeg \
     -lz \
     -lm \
     $(LNX_LIBS)

ivmip_SOURCES = \
     main.c \
     inputdevice.c \
     inputdevice.h \
     settings.h \
     ../afs/afsutil.c \
     ../ivm_io/ivm_io.c

if IVM_COMPILE

# Build for reading scans instead of raw files
ivmipscans_CFLAGS = \
     $(AM_CFLAGS) $(CLFAGS) -DIVM_RAW_TESTDATA=1\
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
ivmipscans_LDADD = $(ivmip_LDADD)
ivmipscans_SOURCES = $(ivmip_SOURCES)

# Experimental: Dynamic app loading - only for ivm target
ivmipxz_CFLAGS = \
     $(AM_CFLAGS) $(CFLAGS) -DIVM_INCLUDE_APP=0 -DIVM_RAW_TESTDATA=0\
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

ivmipxz_SOURCES = \
     main.c \
     inputdevice.c \
     inputdevice.h \
     settings.h \
     ../afs/afsutil.c \
     ../ivm_io/ivm_io.c
endif

install-data-hook:
	echo "mkdir -p out" > $(DESTDIR)/test.sh
	echo "ivm ivmip.b -o out -i ${ivmipdir}  -m 2073741824" >> $(DESTDIR)/test.sh

# Optimized and non optimized asm / test
ivmip.b: ivmip
	ivm as  --noopt --bin "ivmip.b" --sym "ivmip.sym" ivmip
asm: ivmip.b

ivmipscans.b: ivmipscans
	ivm as  --noopt --bin "ivmipscans.b" --sym "ivmipscans.sym" ivmipscans
asmscans: ivmipscans.b

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

all-local: asm asmscans
clean-local:
	-rm *.sym *.b

.PHONY: test asm asmxz asmopt

check_PROGRAMS = ivmip $(IVM_TARGETS)
