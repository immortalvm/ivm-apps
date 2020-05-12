
noinst_PROGRAMS = ivmapp

#     -Xlinker -mdce 

ivmapp_CFLAGS = \
     $(AM_CFLAGS) \
     -I${top_srcdir} \
     -I${top_srcdir}/afs \
     -I${top_srcdir}/boxing/inc \
     -I${top_srcdir}/boxing/thirdparty/glib \
     -I${top_srcdir}/boxing/thirdparty/minixml/inc \
     -I${top_srcdir}/file-format-decoders/include \
     -L$(LIBBOXING_DIR)/../../file-format-decoders/lib \
     -L$(LIBBOXING_DIR) 
ivmapp_LDADD = \
     -lunboxing \
     -lafs \
     -lm -ltiff -lz

if ! IVM_COMPILE
ivmapp_LDADD += \
     -lpng
endif

ivmapp_SOURCES = \
     main.c \
     ../afs/afsutil.c \
     ../ivm_io/ivm_io.c \
     ../ivm_formats/ivm_format_tiff.c \
     ../ivm_formats/ivm_format_zlib.c \
     ../ivm_formats/ivm_formats.c

check_PROGRAMS = ivmapp

