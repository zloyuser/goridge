dnl config.m4 for extension goridge

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([goridge],
dnl   [for goridge support],
dnl   [AS_HELP_STRING([--with-goridge],
dnl     [Include goridge support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([goridge],
  [whether to enable goridge support],
  [AS_HELP_STRING([--enable-goridge],
    [Enable goridge support])],
  [no])

if test "$PHP_GORIDGE" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, GORIDGE_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-goridge -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/goridge.h"  # you most likely want to change this
  dnl if test -r $PHP_GORIDGE/$SEARCH_FOR; then # path given as parameter
  dnl   GORIDGE_DIR=$PHP_GORIDGE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for goridge files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       GORIDGE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$GORIDGE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the goridge distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-goridge -> add include path
  dnl PHP_ADD_INCLUDE($GORIDGE_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-goridge -> check for lib and symbol presence
  dnl LIBNAME=GORIDGE # you may want to change this
  dnl LIBSYMBOL=GORIDGE # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_GORIDGE_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your goridge library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $GORIDGE_DIR/$PHP_LIBDIR, GORIDGE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_GORIDGE_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your goridge library.])
  dnl ],[
  dnl   -L$GORIDGE_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(GORIDGE_SHARED_LIBADD)

  dnl In case of no dependencies
  PHP_REQUIRE_CXX()
  PHP_SUBST(BUFFER_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, BUFFER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(goridge, src/Frame.cpp src/Connection.cpp src/StreamConnection.cpp src/Relay.cpp src/RPC.cpp goridge.cpp, $ext_shared)
fi
