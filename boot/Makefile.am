
noinst_PROGRAMS = ivmboot

ivmboot_CFLAGS = \
     $(AM_CFLAGS) \
     -I${top_srcdir} \
     -I${abs_top_builddir}/afs/include \
     -I${abs_top_builddir}/boxing/include \
     -I${abs_top_builddir}/boxing/thirdparty/glib \
     -I${abs_top_builddir}/boxing/thirdparty/minixml/inc \
     -I../file-format-decoders/include \
     -I${top_srcdir}/app \
     -L$(LIBBOXING_DIR)/../../file-format-decoders/lib \
     -L$(LIBBOXING_DIR) \
     -L$(LIBAFS_DIR) \
     -L../app

ivmboot_LDADD = \
     -lunboxing \
     -lafs \
     -livmapp \
     -ltiff \
     -ljpeg \
     -lz \
     -lm 

if ! IVM_COMPILE
ivmboot_LDADD += \
     -lpng
endif

ivmboot_SOURCES = \
     main.c \
     inputdevice.c \
     inputdevice.h \
     settings.h \
     ../afs/afsutil.c \
     ../ivm_io/ivm_io.c

check_PROGRAMS = ivmboot
