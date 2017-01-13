/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#include <iconv.h>
#include <datrie/trie.h>

#ifndef PHP_TABOO_H
#define PHP_TABOO_H

extern zend_module_entry taboo_module_entry;
#define phpext_taboo_ptr &taboo_module_entry

#define PHP_TABOO_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_TABOO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_TABOO_API __attribute__ ((visibility("default")))
#else
#	define PHP_TABOO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define ALPHA_CHARSET	"UCS-4LE"
#define PHP_TRIE_FILTER_RES_NAME "Trie tree taboo"

ZEND_BEGIN_MODULE_GLOBALS(taboo)
	Trie* pTrie;
    int initNum;
    int newNum;
ZEND_END_MODULE_GLOBALS(taboo)

PHP_MINIT_FUNCTION(taboo);
PHP_MSHUTDOWN_FUNCTION(taboo);
PHP_RINIT_FUNCTION(taboo);
PHP_RSHUTDOWN_FUNCTION(taboo);
PHP_MINFO_FUNCTION(taboo);
PHP_GINIT_FUNCTION(taboo);

PHP_FUNCTION(taboo_search);
PHP_FUNCTION(taboo_init);
PHP_FUNCTION(taboo_istaboo);

/* Always refer to the globals in your function as TABOO_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define TABOO_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(taboo, v)

#if defined(ZTS) && defined(COMPILE_DL_TABOO)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

//extern ZEND_DECLARE_MODULE_GLOBALS(taobao);

#endif	/* PHP_TABOO_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
