#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# required version of gstreamer and gst-plugins-base
AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday memset])

LT_PREREQ([2.2.6])
LT_INIT

AC_ARG_ENABLE([install-libarib25],
  [AS_HELP_STRING([--enable-install-libarib25],
    [Install libarib25 library. [default=yes]])],
  [install_libarib25="${enableval}"],
  [install_libarib25=yes]
)
AM_CONDITIONAL([INSTALL_LIBARIB25],[test x$install_libarib25 != xno])

AC_ARG_ENABLE([install-b25],
  [AS_HELP_STRING([--enable-install-b25],
    [Install b25 command. [default=yes]])],
  [install_b25="${enableval}"],
  [install_b25=yes]
)
AM_CONDITIONAL([INSTALL_B25],[test x$install_b25 != xno])


AC_CHECK_PROG(HAVE_PKGCONFIG, pkg-config, [ ], [
  AC_MSG_ERROR([You need to have pkg-config installed!])
])

PKG_CHECK_MODULES(PCSCLITE, [
  libpcsclite >= PCSCLITE_REQUIRED
], [
  AC_SUBST(PSCSLITE_CFLAGS)
  AC_SUBST(PSCSLITE_LIBS)
], [
  AC_MSG_ERROR([
    Missing libpcsclite.
  ])
])

# =======================================================================
# Check for the required version of GStreamer core (and gst-plugins-base)
#
# This will export GST_CFLAGS and GST_LIBS variables for use in Makefile.am
#
# If you need libraries from gst-plugins-base here, also add:
# for libgstaudio-1.0: gstreamer-audio-1.0 >= $GST_REQUIRED
# for libgstvideo-1.0: gstreamer-video-1.0 >= $GST_REQUIRED
# for libgsttag-1.0: gstreamer-tag-1.0 >= $GST_REQUIRED
# for libgstpbutils-1.0: gstreamer-pbutils-1.0 >= $GST_REQUIRED
# for libgstfft-1.0: gstreamer-fft-1.0 >= $GST_REQUIRED
# for libgstinterfaces-1.0: gstreamer-interfaces-1.0 >= $GST_REQUIRED
# for libgstrtp-1.0: gstreamer-rtp-1.0 >= $GST_REQUIRED
# for libgstrtsp-1.0: gstreamer-rtsp-1.0 >= $GST_REQUIRED
# etc.
# =======================================================================

GST_REQUIRED=1.0.0

GST_PLUGIN_LDFLAGS="-module -avoid-version -export-symbols-regex '^[_]*gst_plugin_.*' $GST_ALL_LDFLAGS"
AC_SUBST(GST_PLUGIN_LDFLAGS)

GST_PLUGIN_LIBTOOLFLAGS="--tag=disable-static"
AC_SUBST(GST_PLUGIN_LIBTOOLFLAGS)


PKG_CHECK_MODULES(GST, [
    gstreamer-1.0              >= $GST_REQUIRED
    gstreamer-base-1.0         >= $GST_REQUIRED
    gstreamer-controller-1.0   >= $GST_REQUIRED
  ], [
    AC_SUBST(GST_CFLAGS)
    AC_SUBST(GST_LIBS)
  ], [
  AC_MSG_ERROR([
    Can't find the following GStreamer development packages:

      gstreamer-1.0              >= $GST_REQUIRED
      gstreamer-base-1.0         >= $GST_REQUIRED
      gstreamer-controller-1.0   >= $GST_REQUIRED

    Please make sure you have the necessary GStreamer-1.0
    development headers installed.

    On debian/Ubuntu systems you will probably need to install the
    'libgstreamer1.0-dev' and 'libgstreamer-plugins-base1.0-dev' packages.

    On RPM-based systems you will probably need to install the
    'gstreamer-devel-1.0' package.
  ])
])

# check if compiler understands -Wall (if yes, add -Wall to GST_CFLAGS)
AC_MSG_CHECKING([to see if compiler understands -Wall])
save_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS -Wall"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([ ], [ ])], [
  GST_CFLAGS="$GST_CFLAGS -Wall"
  AC_MSG_RESULT([yes])
], [
  AC_MSG_RESULT([no])
])
 
AC_CONFIG_FILES([Makefile
                 gst/Makefile
                 src/Makefile])
AC_OUTPUT
