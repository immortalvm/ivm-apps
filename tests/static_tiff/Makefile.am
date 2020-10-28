
noinst_PROGRAMS = static_tiff

static_tiff_CFLAGS = \
     $(AM_CFLAGS) \
     -Xlinker -mdce \
     -I../../file-format-decoders/include \
     -L../../file-format-decoders/lib \
     -I${top_srcdir}/ivm_io \
     -I$(LIBTESTDATA_DIR)/tiff
static_tiff_LDADD = \
     -lm -ltiff -lz -ljpeg
static_tiff_SOURCES = \
     main.c \
     ../../ivm_io/ivm_io.c

check_PROGRAMS = static_tiff

