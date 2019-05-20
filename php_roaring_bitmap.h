/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
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

#ifndef PHP_ROARING_BITMAP_H
#define PHP_ROARING_BITMAP_H

extern zend_module_entry roaring_bitmap_module_entry;
#define roaring_bitmap_module_ptr &roaring_bitmap_module_entry
#define phpext_roaring_bitmap_ptr &roaring_bitmap_module_entry

#define PHP_ROARING_BITMAP_EXTNAME  "roaring_bitmap"
#define PHP_ROARING_BITMAP_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_ROARING_BITMAP_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_ROARING_BITMAP_API __attribute__ ((visibility("default")))
#else
#	define PHP_ROARING_BITMAP_API
#endif

//--------------------- 64bit ---------------------

extern zend_module_entry roaring_bitmap64_module_entry;
#define phpext_roaring_bitmap64_ptr &roaring_bitmap64_module_entry

#define PHP_ROARING_BITMAP64_EXTNAME  "roaring_bitmap64"
#define PHP_ROARING_BITMAP64_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
# define PHP_ROARING_BITMAP64_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_ROARING_BITMAP64_API __attribute__ ((visibility("default")))
#else
# define PHP_ROARING_BITMAP64_API
#endif


#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
  #include "php.h"
}

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(roaring_bitmap)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(roaring_bitmap)
*/

/* Always refer to the globals in your function as ROARING_BITMAP_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define ROARING_BITMAP_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(roaring_bitmap, v)

#if defined(ZTS) && defined(COMPILE_DL_ROARING_BITMAP)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

/* Always refer to the globals in your function as ROARING_BITMAP64_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define ROARING_BITMAP64_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(roaring_bitmap64, v)

#if defined(ZTS) && defined(COMPILE_DL_ROARING_BITMAP64)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_ROARING_BITMAP_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
