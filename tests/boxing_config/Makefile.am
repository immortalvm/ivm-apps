
noinst_PROGRAMS = boxing_config

boxing_config_CFLAGS = \
     $(AM_CFLAGS) \
     -I${top_builddir}/boxing/include \
     -I${top_srcdir}/afs

boxing_config_LDADD = \
     ${top_builddir}/boxing/lib/libunboxing.a \
     -lm

boxing_config_SOURCES = \
     main.c 

check_PROGRAMS = boxing_config

