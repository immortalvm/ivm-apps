
noinst_PROGRAMS = static_jpeg

#     -Xlinker -mdce 

static_jpeg_CFLAGS = \
     $(AM_CFLAGS) \
     -I${top_srcdir} \
     -I${abs_top_builddir}/boxing/include \
     -I${top_srcdir}/file-format-decoders/include \
     -L../../file-format-decoders/lib \
     -L../../app \
     -L$(LIBBOXING_DIR) \
     -I${top_srcdir}/ivm_io \
     -I$(LIBTESTDATA_DIR)/jpeg
static_jpeg_LDADD = \
     -livmapp \
     -ltiff -ljpeg -lz -lm \
     -lunboxing


if ! IVM_COMPILE
static_jpeg_LDADD += \
     -lpng
endif


static_jpeg_SOURCES = \
     main.c \
     ../../ivm_io/ivm_io.c

check_PROGRAMS = static_jpeg

