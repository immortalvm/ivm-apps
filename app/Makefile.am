
noinst_PROGRAMS = ivmapp

ivmapp_CFLAGS = \
     $(AM_CFLAGS) \
     -I${top_srcdir} \
     -I${top_srcdir}/afs \
     -I${top_srcdir}/boxing/inc \
     -I${top_srcdir}/boxing/thirdparty/glib \
     -I${top_srcdir}/boxing/thirdparty/minixml/inc 
ivmapp_LDADD = ${top_builddir}/boxing/lib/libunboxing.a -lm 
ivmapp_SOURCES = \
     main.c \
     ../afs/afsutil.c

check_PROGRAMS = ivmapp

