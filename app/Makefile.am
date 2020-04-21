
noinst_PROGRAMS = ivmapp

ivmapp_CFLAGS = \
     $(AM_CFLAGS) \
     -I${top_srcdir} \
     -I${top_srcdir}/afs \
     -I${top_srcdir}/boxing/inc \
     -I${top_srcdir}/boxing/thirdparty/glib \
     -I${top_srcdir}/boxing/thirdparty/minixml/inc \
     -I${top_srcdir}/file-format-decoders/include \
     -L${top_srcdir}/file-format-decoders/lib
ivmapp_LDADD = \
     ${top_builddir}/boxing/lib/libafs.a \
     ${top_builddir}/boxing/lib/libunboxing.a \
     ${top_builddir}/boxing/lib/libafs.a \
     -lm -ltiff -lz
ivmapp_SOURCES = \
     main.c \
     ../afs/afsutil.c \
     ../ivm_formats/ivm_format_tiff.c \
     ../ivm_formats/ivm_format_zlib.c \
     ../ivm_formats/ivm_formats.c

check_PROGRAMS = ivmapp

