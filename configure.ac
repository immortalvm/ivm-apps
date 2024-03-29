#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([ivmapp], [0.1.0], [ivmapps@piql.com])
AM_INIT_AUTOMAKE([
	foreign
	1.11
	-Wall
	-Wno-portability
	silent-rules
	tar-pax
	subdir-objects
])
#	no-dist-gzip
#	dist-xz
#	check-news

AC_COPYRIGHT([Copyright (C) 2020 Piql AS])

# Check that we are in the correct dir
AC_CONFIG_SRCDIR([initial-program/main.c])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_RANLIB
AC_PROG_LN_S


# Checks for header files.
AC_CHECK_HEADERS([float.h inttypes.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([floor memmove memset pow sqrt strcasecmp strchr strtol strtoull])

# Checks for command line options
AC_ARG_VAR(LIBBOXING_DIR, path to boxing lib)
AC_ARG_VAR(LIBAFS_DIR, path to afs lib)
AC_ARG_VAR(LIBTESTDATA_DIR, path to testdata lib)
AM_CONDITIONAL([IVM_COMPILE], [test "x$CC" = "xivm64-gcc"])
AC_CONFIG_FILES([
    Makefile
    app/Makefile
    initial-program/Makefile
    ivm_xz/Makefile
    tests/static_tiff/Makefile
    tests/static_pdf/Makefile
    tests/static_jpeg/Makefile
    tests/boxing_config/Makefile])
AC_OUTPUT
AC_MSG_RESULT([
        $PACKAGE $VERSION
        ================

        prefix:                 ${prefix}
        sysconfdir:             ${sysconfdir}
        libdir:                 ${libdir}
        includedir:             ${includedir}

        compiler:               ${CC}
        cflags:                 ${CFLAGS}
        ldflags:                ${LDFLAGS}

        callbacks:              ${enable_callbacks}
        tests:                  ${with_unit_tests}

        libboxing:              ${LIBBOXING_DIR}
        libafs:                 ${LIBAFS_DIR}
        libtestdata:            ${LIBTESTDATA_DIR}
])
AM_COND_IF([IVM_COMPILE],
  [AC_MSG_RESULT([
        ivm_compile:            yes])],
  [AC_MSG_RESULT([
        ivm_compile:            no])])
