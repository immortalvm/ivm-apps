
noinst_PROGRAMS = ivmapp

#     -Xlinker -mdce 

ivmapp_CFLAGS = \
     $(AM_CFLAGS) \
     -I${top_srcdir} \
     -I${top_srcdir}/afs \
     -I${abs_top_builddir}/afs/include \
     -I${abs_top_builddir}/boxing/include \
     -I${abs_top_builddir}/boxing/thirdparty/glib \
     -I${abs_top_builddir}/boxing/thirdparty/minixml/inc \
     -I../file-format-decoders/include \
     -L$(LIBBOXING_DIR)/../../file-format-decoders/lib \
     -L$(LIBBOXING_DIR) \
     -L$(LIBAFS_DIR)
ivmapp_LDADD = \
     -lunboxing \
     -lafs \
     -lm -ltiff -ljpeg -lz

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
     ../ivm_formats/ivm_format_jpeg.c \
     ../ivm_formats/ivm_formats.c

check_PROGRAMS = ivmapp

