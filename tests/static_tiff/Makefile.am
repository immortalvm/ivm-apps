
noinst_PROGRAMS = static_tiff

static_tiff_CFLAGS = \
     $(AM_CFLAGS) \
     -Xlinker -mdce \
     -I${top_srcdir}/file-format-decoders/include \
     -L${top_srcdir}/file-format-decoders/lib \
     -I${top_srcdir}/ivm_io \
     -I${top_srcdir}/../testdata/tiff
static_tiff_LDADD = \
     -lm -ltiff -lz
static_tiff_SOURCES = \
     main.c \
     ../../ivm_io/ivm_io.c

check_PROGRAMS = static_tiff

