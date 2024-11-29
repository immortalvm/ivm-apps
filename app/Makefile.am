
lib_LIBRARIES = libivmapp.a

libivmapp_a_CFLAGS = \
     $(AM_CFLAGS) \
     -I${top_srcdir} \
     -I${top_srcdir}/afs \
     -I${abs_top_builddir}/afs/include \
     -I${abs_top_builddir}/boxing/include \
     -I${abs_top_builddir}/boxing/thirdparty/glib \
     -I${abs_top_builddir}/boxing/thirdparty/minixml/inc \
     -I../file-format-decoders/include \
     -I${top_srcdir}/initial-program \
     -L$(LIBBOXING_DIR)/../../file-format-decoders/lib \
     -L$(LIBBOXING_DIR) \
     -L$(LIBAFS_DIR)

libivmapp_a_CPPFLAGS = \
     $(AM_CPPFLAGS) \
     $(libivmapp_a_CFLAGS) \
     -I../../ROAEshell/thirdparty/zlib \
     -I../../ROAEshell/thirdparty/tinyxml2

libivmapp_a_SOURCES = \
     app.c \
     app.h \
     ../ivm_formats/ivm_format_tiff.c \
     ../ivm_formats/ivm_format_zlib.c \
     ../ivm_formats/ivm_format_jpeg.c \
     ../ivm_formats/ivm_format_siard.c \
     ../../ROAEshell/thirdparty/tinyxml2/tinyxml2.cpp \
     ../../ROAEshell/thirdparty/zlib/contrib/minizip/ioapi.c \
     ../../ROAEshell/thirdparty/zlib/contrib/minizip/unzip.c \
     ../../ROAEshell/thirdparty/zlib/contrib/minizip/ida_miniunz_utils.cpp \
     ../../ROAEshell/thirdparty/zlib/contrib/minizip/ida_miniunz.c \
     ../../ROAEshell/ida/siard2sql/libsiardunzip.c \
     ../../ROAEshell/ida/siard2sql/libsiardxml.cpp \
     ../ivm_formats/ivm_formats.c


