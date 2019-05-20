dnl $Id$
dnl config.m4 for extension roaring_bitmap

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(roaring_bitmap, for roaring_bitmap support,
dnl Make sure that the comment is aligned:
dnl [  --with-roaring_bitmap             Include roaring_bitmap support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(roaring_bitmap, whether to enable roaring_bitmap support,
dnl Make sure that the comment is aligned:
[  --enable-roaring_bitmap           Enable roaring_bitmap support])

if test "$PHP_ROARING_BITMAP" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=`$PKG_CONFIG foo --cflags`
  dnl     LIBFOO_LIBDIR=`$PKG_CONFIG foo --libs`
  dnl     LIBFOO_VERSON=`$PKG_CONFIG foo --modversion`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, ROARING_BITMAP_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-roaring_bitmap -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/roaring_bitmap.h"  # you most likely want to change this
  dnl if test -r $PHP_ROARING_BITMAP/$SEARCH_FOR; then # path given as parameter
  dnl   ROARING_BITMAP_DIR=$PHP_ROARING_BITMAP
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for roaring_bitmap files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ROARING_BITMAP_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ROARING_BITMAP_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the roaring_bitmap distribution])
  dnl fi

  dnl # --with-roaring_bitmap -> add include path
  dnl PHP_ADD_INCLUDE($ROARING_BITMAP_DIR/include)

  dnl # --with-roaring_bitmap -> check for lib and symbol presence
  dnl LIBNAME=roaring_bitmap # you may want to change this
  dnl LIBSYMBOL=roaring_bitmap # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ROARING_BITMAP_DIR/$PHP_LIBDIR, ROARING_BITMAP_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ROARING_BITMAPLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong roaring_bitmap lib version or lib not found])
  dnl ],[
  dnl   -L$ROARING_BITMAP_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ROARING_BITMAP_SHARED_LIBADD)

  PHP_REQUIRE_CXX()

  PHP_NEW_EXTENSION(roaring_bitmap, roaring_bitmap.cpp, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1 -std=c++11)
fi
