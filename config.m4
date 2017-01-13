dnl $Id$
dnl config.m4 for extension taboo

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(taboo, for taboo support,
[  --with-taboo             Include taboo support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(taboo, whether to enable taboo support,
dnl Make sure that the comment is aligned:
dnl [  --enable-taboo           Enable taboo support])

if test "$PHP_TABOO" != "no"; then
  SEARCH_PATH="/usr/local"
  SEARCH_FOR="/include/datrie/trie.h"
  if test -r $PHP_TABOO/$SEARCH_FOR; then
    TABOO_DIR=$PHP_TABOO
  else
    AC_MSG_CHECKING([for taboo files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        TABOO_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  if test -z "$TABOO_DIR"; then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR([Please install the libdatrie])
  fi

  dnl # --with-taboo -> add include path
  PHP_ADD_INCLUDE($TABOO_DIR/include)

  LIBNAME=datrie
  LIBSYMBOL=trie_new_from_file

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TABOO_DIR/$PHP_LIBDIR, TABOO_SHARED_LIBADD)
    AC_DEFINE(HAVE_TABOOLIB,1,[ libdatrie found and included ])
  ],[
    AC_MSG_ERROR([wrong libdatrie lib version or lib not found])
  ],[
    -L$TABOO_DIR/$PHP_LIBDIR -ldatrie
  ])

  PHP_SUBST(TABOO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(taboo, taboo.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
