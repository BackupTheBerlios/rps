# generated 2003/9/21 13:35:54 CEST by thoma@Tiger.(none)
# using glademm V2.0.0b

AC_INIT(src/rps.cc)
AM_INIT_AUTOMAKE(rps, 0.0)
AM_CONFIG_HEADER(config.h)

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AM_PROG_LIBTOOL

PKG_CHECK_MODULES(SIGC,[sigc++-1.2 >= 1.2.0])
AC_SUBST(SIGC_CFLAGS)
AC_SUBST(SIGC_LIBS) 


dnl *************************************************
dnl gettext support
dnl *************************************************

GETTEXT_PACKAGE=rpgs
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE")

dnl Add the languages which your application supports here.
ALL_LINGUAS=""
AM_GNU_GETTEXT

dnl Set PACKAGE_LOCALE_DIR in config.h.
if test "x${prefix}" = "xNONE"; then
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${ac_default_prefix}/${DATADIRNAME}/locale")
else
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${prefix}/${DATADIRNAME}/locale")
fi

AC_LANG_CPLUSPLUS

PKG_CHECK_MODULES(GTKMM,[gtkmm-2.0 >= 2.2.7])
AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)

AC_DEFINE(ASDSUPPORT,1,"[ with asd support ]")
AC_INCLUDES="-I/usr/src/asd4-0.2/"

AC_OUTPUT(Makefile lib/Makefile src/Makefile tools/Makefile)
