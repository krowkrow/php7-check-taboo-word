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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_taboo.h"


ZEND_DECLARE_MODULE_GLOBALS(taboo)


/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("taboo.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_taboo_globals, taboo_globals)
    STD_PHP_INI_ENTRY("taboo.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_taboo_globals, taboo_globals)
PHP_INI_END()
*/
/* }}} */


/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_taboo_init_globals
 */
static void php_taboo_init_globals(zend_taboo_globals *taboo_globals)
{
	taboo_globals->initNum = 0;
    taboo_globals->newNum = 0;
    taboo_globals->pTrie = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(taboo)
{
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(taboo)
{
    if (TABOO_G(pTrie) != NULL) {
        trie_free(TABOO_G(pTrie));
    }
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(taboo)
{
#if defined(COMPILE_DL_TABOO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(taboo)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(taboo)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "taboo support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ taboo_functions[]
 *
 * Every user visible function must have an entry in taboo_functions[].
 */
const zend_function_entry taboo_functions[] = {
	PHP_FE(taboo_search, NULL)
    PHP_FE(taboo_init, NULL)
    PHP_FE(taboo_istaboo, NULL)
	PHP_FE_END	/* Must be the last line in taboo_functions[] */
};
/* }}} */

/* {{{ taboo_module_entry
 */
zend_module_entry taboo_module_entry = {
	STANDARD_MODULE_HEADER,
	"taboo",
	taboo_functions,
	PHP_MINIT(taboo),
	PHP_MSHUTDOWN(taboo),
	PHP_RINIT(taboo),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(taboo),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(taboo),
	PHP_TABOO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TABOO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(taboo)
#endif



static int trie_search_one(Trie *trie, const AlphaChar *text, int *offset, TrieData *length)
{
	TrieState *s;
	const AlphaChar *p;
	const AlphaChar *base;

	base = text;
    if (! (s = trie_root(trie))) {
        return -1;
    }

	while (*text) {
		p = text;
		if (! trie_state_is_walkable(s, *p)) {
            trie_state_rewind(s);
			text++;
			continue;
		} else {
			trie_state_walk(s, *p++);
        }

		while (trie_state_is_walkable(s, *p) && ! trie_state_is_terminal(s))
			trie_state_walk(s, *p++);

		if (trie_state_is_terminal(s)) {
			*offset = text - base;
			*length = p - text;
            trie_state_free(s);

			return 1;
		}

        trie_state_rewind(s);
		text++;
	}
    trie_state_free(s);

	return 0;
}

/* {{{ proto array taboo_search(string centent)
   Returns info about first keyword, or false on error*/
PHP_FUNCTION(taboo_search)
{
	zend_string *text;

	int offset = -1, i, ret;
    TrieData length = 0;

	AlphaChar *alpha_text;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &text) == FAILURE) {
		RETURN_FALSE;
	}

    array_init(return_value);
    if (text->len < 1) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "input is empty");
		return;
	}

	alpha_text = emalloc(sizeof(AlphaChar) * ((text->len) + 1));
	for (i = 0; i < text->len; i++) {
		alpha_text[i] = (AlphaChar) ((unsigned char *) text->val)[i];
	}

	alpha_text[text->len] = TRIE_CHAR_TERM;

	ret = trie_search_one(TABOO_G(pTrie), alpha_text, &offset, &length);
    efree(alpha_text);
	if (ret == 0) {
        return;
    } else if (ret == 1) {
		add_next_index_long(return_value, offset);
		add_next_index_long(return_value, length);
	} else {
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto resource taboo_init(string dict_file_path)
   Returns SUCCESS, or FALSE on error*/
PHP_FUNCTION(taboo_init)
{
    zend_string *path;
    char separator[32] = { 0 };
    
    ++ TABOO_G(initNum);
    if(TABOO_G(pTrie) == NULL) {
        ++ TABOO_G(newNum);
        
        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &path) == FAILURE) {
            RETURN_NULL();
        }
        
        TABOO_G(pTrie) = trie_new_from_file(path->val);
        if (!TABOO_G(pTrie)) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to load %s", path->val);
            RETURN_NULL();
        }
    }
    
    // php_printf("initNum:%d, newNum:%d \n", initNum, newNum);
    php_sprintf(separator, "initNum:%d, newNum:%d \n", TABOO_G(initNum), TABOO_G(newNum));
    php_error(E_WARNING, separator);
    
    RETURN_TRUE;
}
/* }}} */

/* {{{ proto array taboo_istaboo(string centent)
   Returns FALSE, or TRUE on has filter words*/
PHP_FUNCTION(taboo_istaboo)
{
	zend_string *text;

	int offset = -1, i, ret;
    TrieData length = 0;

	AlphaChar *alpha_text;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &text) == FAILURE) {
		RETURN_FALSE;
	}

    if (text->len < 1) {
		RETURN_FALSE;
	}

	alpha_text = emalloc(sizeof(AlphaChar) * ((text->len) + 1));
	for (i = 0; i < text->len; i++) {
		alpha_text[i] = (AlphaChar) ((unsigned char *) text->val)[i];
	}

	alpha_text[text->len] = TRIE_CHAR_TERM;

	ret = trie_search_one(TABOO_G(pTrie), alpha_text, &offset, &length);
    efree(alpha_text);
	if (ret == 0) {
        RETURN_FALSE;
    } else if (ret == 1) {
        RETURN_TRUE;
    }
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
