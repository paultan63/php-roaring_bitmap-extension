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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_roaring_bitmap.h"

#include "my_php_define.h"

#include "roaring.hh"
#include "roaring.c"

/* If you declare any globals in php_roaring_bitmap.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(roaring_bitmap)
*/

/* True global resources - no need for thread safety here */
static int le_roaring_bitmap;

zend_object_handlers roaring_bitmap_object_handlers;

#if PHP_MAJOR_VERSION >= 7

typedef struct _roaring_bitmap_object {
    Roaring *roaring;
    Roaring::const_iterator iterator;
    zend_object std; // last
} roaring_bitmap_object;

static inline roaring_bitmap_object* php_roaring_bitmap_obj_from_obj(zend_object *obj) {
    return (roaring_bitmap_object*)((char*)(obj) - XtOffsetOf(roaring_bitmap_object, std));
}

#define Z_TSTOBJ_P(zv)  php_roaring_bitmap_obj_from_obj(Z_OBJ_P((zv)))

#else

typedef struct _roaring_bitmap_object {
    zend_object std; // first
    Roaring *roaring;
    Roaring::const_iterator iterator;
} roaring_bitmap_object;

static inline roaring_bitmap_object* php_roaring_bitmap_obj_from_zval(zval *obj) {
    return (roaring_bitmap_object *)zend_object_store_get_object(obj TSRMLS_CC);
}

#define Z_TSTOBJ_P(zv)  php_roaring_bitmap_obj_from_zval(zv)

#endif

zend_class_entry* roaring_bitmap_ce;

//------------------------ 64bit --------------------
/* True global resources - no need for thread safety here */
static int le_roaring_bitmap64;

zend_object_handlers roaring_bitmap64_object_handlers;


#if PHP_MAJOR_VERSION >= 7

typedef struct _roaring_bitmap64_object {
    Roaring64Map *roaring;
    Roaring64Map::const_iterator iterator;
    zend_object std; // last
} roaring_bitmap64_object;

static inline roaring_bitmap64_object* php_roaring_bitmap64_obj_from_obj(zend_object *obj) {
    return (roaring_bitmap64_object*)((char*)(obj) - XtOffsetOf(roaring_bitmap64_object, std));
}

#define Z_TSTOBJ64_P(zv)  php_roaring_bitmap64_obj_from_obj(Z_OBJ_P((zv)))

#else

typedef struct _roaring_bitmap64_object {
    zend_object std; // first
    Roaring64Map *roaring;
    Roaring64Map::const_iterator iterator;
} roaring_bitmap64_object;

static inline roaring_bitmap64_object* php_roaring_bitmap64_obj_from_zval(zval *obj) {
    return (roaring_bitmap64_object *)zend_object_store_get_object(obj TSRMLS_CC);
}

#define Z_TSTOBJ64_P(zv)  php_roaring_bitmap64_obj_from_zval(zv)

#endif

zend_class_entry* roaring_bitmap64_ce;

//----------------------- 64bit end -------------------

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("roaring_bitmap.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_roaring_bitmap_globals, roaring_bitmap_globals)
    STD_PHP_INI_ENTRY("roaring_bitmap.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_roaring_bitmap_globals, roaring_bitmap_globals)
PHP_INI_END()
*/
/* }}} */

/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

// ------------------------------------------------------------------
// ------------------------------------------------------------------
/* {{{ proto void roaring_bitmap::__construct()
   Create the roaring_bitmap object */
PHP_METHOD(roaring_bitmap, __construct)
{
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
        intern->roaring = new Roaring();    
    }
}
/* }}} */

/* {{{ proto long roaring_bitmap::add(long $n)
   Add value n */
PHP_METHOD(roaring_bitmap, add)
{
	long ret = 0;
	long n;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "l", &n) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	intern->roaring->add(n);
        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::addChecked(long $n)
   Add value n. Returns true if a new value was added, false if the value was already existing */
PHP_METHOD(roaring_bitmap, addChecked)
{
	bool ret = false;
	long n;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "l", &n) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->addChecked(n);
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto void roaring_bitmap::addRange(long $x, long $y)
   add if all values from x (included) to y (excluded) */
PHP_METHOD(roaring_bitmap, addRange)
{
	long ret = 0;
	long begin, end;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "ll", &begin, &end) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	intern->roaring->addRange(begin, end);
        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::addMany(array $arr)
   Add value from array $arr */
PHP_METHOD(roaring_bitmap, addMany)
{
	long ret = 0;
	ulong num_key;

	zval *val, *arr;
	HashTable *arr_hash;
	int array_count;

#if PHP_MAJOR_VERSION >= 7
  zend_string *key;
#else
  zval **data;
  HashPosition pointer;
#endif

  zval *zobj = getThis();
  roaring_bitmap_object *intern;

  if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "a", &arr) == FAILURE) {
          RETURN_NULL();
  }
    
  arr_hash = Z_ARRVAL_P(arr);
//  array_count = zend_hash_num_elements(arr_hash);
//  php_printf("The array passed contains %d elements\n", array_count);

  intern = Z_TSTOBJ_P(zobj);
  if(intern != NULL) {
#if PHP_MAJOR_VERSION >= 7    
    	ZEND_HASH_FOREACH_KEY_VAL(arr_hash, num_key, key, val){
#else
      for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {
        val = *data;
#endif        
/*    		
    		if(key){
    			php_printf("string key: %s\n", ZSTR_VAL(key));
    		}
    		else{
				php_printf("num_key:%u\n", num_key);
    		}
*/
    		switch (Z_TYPE_P(val)) {
    			case IS_LONG:
    				intern->roaring->add(Z_LVAL_P(val));
    				break;

    			case IS_STRING:
    				intern->roaring->add(strtoul(Z_STRVAL_P(val), NULL, 0));
    				break;

    			case IS_NULL:
#if PHP_MAJOR_VERSION >= 7          
    			case IS_TRUE:
    			case IS_FALSE:
#else
          case IS_BOOL:
#endif                    
    			case IS_ARRAY:
    			case IS_OBJECT:
    			case IS_RESOURCE:
    			default: // ignore
    				break;
    		}
#if PHP_MAJOR_VERSION >= 7
    	}ZEND_HASH_FOREACH_END();
#else
      }
#endif
//    	intern->roaring->addRange(begin, end);
        RETURN_LONG(ret);
  }

  RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::remove(long $n)
   Remove value n */
PHP_METHOD(roaring_bitmap, remove)
{
	long ret = 0;
	long n;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "l", &n) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	intern->roaring->remove(n);

        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::removeChecked(long $n)
   Remove value n. Returns true if a new value was removed, false if the value was not existing. */
PHP_METHOD(roaring_bitmap, removeChecked)
{
	bool ret = false;
	long n;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "l", &n) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->removeChecked(n);

        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::maximum()
   Return the largest value (if not empty) */
PHP_METHOD(roaring_bitmap, maximum)
{
	long ret = 0;
	
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->maximum();
    	
        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::minimum()
   Return the smallest value (if not empty) */
PHP_METHOD(roaring_bitmap, minimum)
{
	long ret = 0;
	
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->minimum();
    	
        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::contains(long $n)
   Check if value n is present */
PHP_METHOD(roaring_bitmap, contains)
{
	bool ret = false;
	long n;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "l", &n) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->contains(n);

    	RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::contains(long $x, long $y)
   Check if all values from x (included) to y (excluded) are present */
PHP_METHOD(roaring_bitmap, containsRange)
{
	bool ret = false;
	long begin, end;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "ll", &begin, &end) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->containsRange(begin, end);

        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::rank(long $n)
   Returns the number of integers that are smaller or equal to n. */
PHP_METHOD(roaring_bitmap, rank)
{
	long ret = 0;
	long n;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "l", &n) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->rank(n);

    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::select(long $rank)
   If the size of the roaring bitmap is strictly greater than rank, then 
   this function returns the element of given rank. 
   Otherwise, it returns false. */
PHP_METHOD(roaring_bitmap, select)
{
	bool ret = false;
	long n;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "l", &n) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	uint32_t val;
    	ret = intern->roaring->select(n, &val);
    	if(ret == true){
    		RETURN_LONG((long)val);
    	}

    	RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::flip(long $begin, long $end)
   compute the negation of the roaring bitmap within a specified interval. areas outside the range are passed through unchanged. */
PHP_METHOD(roaring_bitmap, flip)
{
	bool ret = false;
	long begin, end;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "ll", &begin, &end) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	intern->roaring->flip(begin, end);
    	ret = true;

        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::isEmpty()
   Returns true if the bitmap is empty (cardinality is zero). */
PHP_METHOD(roaring_bitmap, isEmpty)
{
	bool ret = false;
	
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->isEmpty();
    	
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::cardinality()
   Get the cardinality of the bitmap (number of elements). */
PHP_METHOD(roaring_bitmap, cardinality)
{
	long ret = 0;
	
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->cardinality();
    	
        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::getSizeInBytes([bool $portable = false])
   How many bytes are required to serialize this bitmap.
   Setting the portable flag to false enable a custom format that can save 
   space compared to the portable format (e.g., for very sparse bitmaps).*/
PHP_METHOD(roaring_bitmap, getSizeInBytes)
{
	long ret = 0;
	bool portable = false;

    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "|b", &portable) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->getSizeInBytes(portable);
    	
        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */


/* {{{ proto long roaring_bitmap::replace(roaring_bitmap $another)
   Copies the content of the provided bitmap, and discard the current content. */
PHP_METHOD(roaring_bitmap, replace)
{
	long ret = 0;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	*(intern->roaring) = *(other->roaring);

    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::eq(roaring_bitmap $another)
   Return true if the two bitmaps contain the same elements. */
PHP_METHOD(roaring_bitmap, eq)
{
  bool ret = false;
  zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
      if(*(intern->roaring) == *(other->roaring))
        ret = true;
      else
        ret = false;

      RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::sub(roaring_bitmap $another)
   Compute the difference between the current bitmap and the provided bitmap, 
   writing the result in the current bitmap. The provided bitmap is not modified.*/
PHP_METHOD(roaring_bitmap, sub)
{
	long ret = 0;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	*(intern->roaring) -= *(other->roaring);

    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::and(roaring_bitmap $another)
   Copies the content of the provided bitmap, and discard the current content. */
PHP_METHOD(roaring_bitmap, and)
{
	long ret = 0;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	*(intern->roaring) &= *(other->roaring);

    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::or(roaring_bitmap $another)
   Compute the union between the current bitmap and the provided bitmap, 
   writing the result in the current bitmap. The provided bitmap is not modified.*/
PHP_METHOD(roaring_bitmap, or)
{
	long ret = 0;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	*(intern->roaring) |= *(other->roaring);

    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::xor(roaring_bitmap $another)
   Compute the symmetric union between the current bitmap and the provided bitmap,
   writing the result in the current bitmap. The provided bitmap is not modified. */
PHP_METHOD(roaring_bitmap, xor)
{
	long ret = 0;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	*(intern->roaring) ^= *(other->roaring);

    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::and_cardinality(roaring_bitmap $another)
   Computes the size of the intersection between two bitmaps. */
PHP_METHOD(roaring_bitmap, and_cardinality)
{
	long ret = 0;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	ret = intern->roaring->and_cardinality(*(other->roaring));
    	
    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::or_cardinality(roaring_bitmap $another)
   Computes the size of the union between two bitmaps. */
PHP_METHOD(roaring_bitmap, or_cardinality)
{
	long ret = 0;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	ret = intern->roaring->or_cardinality(*(other->roaring));
    	
    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::andnot_cardinality(roaring_bitmap $another)
   Computes the size of the difference (andnot) between two bitmaps. */
PHP_METHOD(roaring_bitmap, andnot_cardinality)
{
	long ret = 0;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	ret = intern->roaring->andnot_cardinality(*(other->roaring));
    	
    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::xor_cardinality(roaring_bitmap $another)
   Computes the size of the symmetric difference (andnot) between two bitmaps. */
PHP_METHOD(roaring_bitmap, xor_cardinality)
{
	long ret = 0;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	ret = intern->roaring->xor_cardinality(*(other->roaring));
    	
    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto double roaring_bitmap::jaccard_index(roaring_bitmap $another)
   Computes the Jaccard index between two bitmaps. (Also known as the Tanimoto distance, 
   or the Jaccard similarity coefficient)
   The Jaccard index is undefined if both bitmaps are empty. */
PHP_METHOD(roaring_bitmap, jaccard_index)
{
	double ret = 0.0;
	zval* other_val; 
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	ret = intern->roaring->jaccard_index(*(other->roaring));
    	
    	RETURN_DOUBLE(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::isSubset(roaring_bitmap $another)
   Returns true if the bitmap is subset of the other. */
PHP_METHOD(roaring_bitmap, isSubset)
{
	bool ret = false;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	ret = intern->roaring->isSubset(*(other->roaring));

    	RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::isStrictSubset(roaring_bitmap $another)
   Returns true if the bitmap is strict subset of the other. */
PHP_METHOD(roaring_bitmap, isStrictSubset)
{
	bool ret = false;
	zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);

    roaring_bitmap_object *other;
    other = Z_TSTOBJ_P(other_val);

    if(intern != NULL && other != NULL){
    	ret = intern->roaring->isStrictSubset(*(other->roaring));

    	RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::iterator_begin()
   Iterate over the bitmap elements. Move the iterator to the first value. */
PHP_METHOD(roaring_bitmap, iterator_begin)
{
	bool ret = true;
	
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	intern->iterator = intern->roaring->begin();
    	
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::iterator_end()
   Return false means that the iteration should continue while true means that it should stop. */
PHP_METHOD(roaring_bitmap, iterator_end)
{
	bool ret = true;
	
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	if(intern->iterator == intern->roaring->end())
    		ret = true;
    	else
    		ret = false;
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::iterator_advance()
   Move the iterator to the next. */
PHP_METHOD(roaring_bitmap, iterator_advance)
{
	bool ret = true;
	
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	++(intern->iterator);
    	
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::iterator_value()
   Return current iterator value. */
PHP_METHOD(roaring_bitmap, iterator_value)
{
	long ret = 0;
	
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = *(intern->iterator);;
    	
        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto array roaring_bitmap::toArray()
   Convert the bitmap to an array and return it. array size is: this->cardinality() */
PHP_METHOD(roaring_bitmap, toArray)
{
	array_init(return_value);
	
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	/*
    	for(Roaring::const_iterator it = intern->roaring->begin() ; it != intern->roaring->end() ; it++) {
			add_next_index_long(return_value, *it);
		}
		*/
		uint32_t num = intern->roaring->cardinality();
		uint32_t* vals = (uint32_t*)malloc(sizeof(uint32_t) * num);
		if(vals == NULL){
			RETURN_NULL();
		}
		intern->roaring->toUint32Array(vals);
		uint32_t* p = vals;
		for(uint32_t i=0; i < num; i++, p++){
			add_next_index_long(return_value, *p);
		}
		free(vals);

        return;
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap::toString()
   Print the content of the bitmap into a string and return it. */
PHP_METHOD(roaring_bitmap, toString)
{
    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	std::string str = intern->roaring->toString();

      MY_RETURN_STRINGL(str.c_str(), str.length(), 1);      
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap::write([bool $portable = false])
   write a bitmap to a char buffer. Returns how many bytes were written which should be getSizeInBytes(). 
   Setting the portable flag to false enable a custom format that can save space compared to the portable format 
   (e.g., for very sparse bitmaps). */
PHP_METHOD(roaring_bitmap, write)
{
	bool portable = false;

    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "|b", &portable) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	uint32_t size = intern->roaring->getSizeInBytes(portable);
    	char* buf = (char*)emalloc(sizeof(char) * size);
    	if(buf == NULL){
    		RETURN_NULL();
    	}
    	size_t len = intern->roaring->write(buf, portable);
     
      MY_RETVAL_STRINGL(buf, len, 0);

    	return;
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::read([bool $portable = false])
   read a bitmap from a serialized version. Setting the portable flag to false enable a custom format that 
   can save space compared to the portable format (e.g., for very sparse bitmaps). */
PHP_METHOD(roaring_bitmap, read)
{
	zval* data;
	bool portable = false;

    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "z|b", &data, &portable) == FAILURE) {
          RETURN_NULL();
    }
    if(Z_TYPE_P(data) != IS_STRING){
      RETURN_FALSE;
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	try{
//    		Roaring new_roaring = Roaring::readSafe(Z_STRVAL_P(data), Z_STRLEN_P(data));
    		Roaring new_roaring = Roaring::read(Z_STRVAL_P(data), portable);
    		intern->roaring->swap(new_roaring);
    	}
    	catch(std::exception& e){
    		 RETURN_NULL();
    	}

    	RETURN_TRUE;
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::getCopyOnWrite()
   Whether or not copy and write is active. */
PHP_METHOD(roaring_bitmap, getCopyOnWrite)
{
	bool bval = false;

    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	bval = intern->roaring->getCopyOnWrite();

    	RETURN_BOOL(bval);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::setCopyOnWrite()
   Whether or not we apply copy and write. */
PHP_METHOD(roaring_bitmap, setCopyOnWrite)
{
	bool bval = false;

    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "|b", &bval) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	intern->roaring->setCopyOnWrite(bval);

    	RETURN_TRUE;
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::removeRunCompression()
   Remove run-length encoding even when it is more space efficient return whether a change was applied. */
PHP_METHOD(roaring_bitmap, removeRunCompression)
{
	bool bval = false;

    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	bval = intern->roaring->removeRunCompression();

    	RETURN_BOOL(bval);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap::runOptimize()
   convert array and bitmap containers to run containers when it is more efficient; also convert from run 
   containers when more space efficient.  Returns true if the result has at least one run container. 
   Additional savings might be possible by calling shrinkToFit(). */
PHP_METHOD(roaring_bitmap, runOptimize)
{
	bool bval = false;

    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	bval = intern->roaring->runOptimize();

    	RETURN_BOOL(bval);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap::shrinkToFit()
   If needed, reallocate memory to shrink the memory usage. Returns the number of bytes saved. */
PHP_METHOD(roaring_bitmap, shrinkToFit)
{
	long ret = 0;

    zval *zobj = getThis();
    roaring_bitmap_object *intern;

    intern = Z_TSTOBJ_P(zobj);
    if(intern != NULL) {
    	ret = intern->roaring->shrinkToFit();

    	RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

// ------------------------------------------------------------------ 
// ------------------------------------------------------------------

// ------------------------------------------------------------------ 
// ------------------------------------------------------------------
static void roaring_bitmap_object_free(zend_object *object)
{
    roaring_bitmap_object *my_obj;
    my_obj = (roaring_bitmap_object *)((char *)
        object - XtOffsetOf(roaring_bitmap_object, std));
    delete my_obj->roaring;
    my_obj->roaring = NULL;

    // Free the object using Zend macro.
    zend_object_std_dtor(object); 

#if PHP_MAJOR_VERSION < 7
	efree(my_obj);
#endif    
}

#if PHP_MAJOR_VERSION >= 7
zend_object* roaring_bitmap_object_new(zend_class_entry *ce TSRMLS_DC)
#else
zend_object_value roaring_bitmap_object_new(zend_class_entry *ce TSRMLS_DC)
#endif
{
#if PHP_MAJOR_VERSION >= 7  
    roaring_bitmap_object *intern = (roaring_bitmap_object*)ecalloc(1,
            sizeof(roaring_bitmap_object) +
            zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &roaring_bitmap_object_handlers;

    return &intern->std;
#else
    zval *tmp;
    zend_object_value retval;

    roaring_bitmap_object *intern = (roaring_bitmap_object *)ecalloc(1, sizeof(roaring_bitmap_object));
    intern->std.ce = ce;

    ALLOC_HASHTABLE(intern->std.properties);
    zend_hash_init(intern->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

    PROP_INIT;

    retval.handle = zend_objects_store_put(intern, NULL,
        (zend_objects_free_object_storage_t)roaring_bitmap_object_free, NULL TSRMLS_CC);

    retval.handlers = &roaring_bitmap_object_handlers;

    return retval;
#endif    
}

static void roaring_bitmap_object_destroy(zend_object *object)
{
#if PHP_MAJOR_VERSION >= 7  
    roaring_bitmap_object *my_obj;
    my_obj = (roaring_bitmap_object*)((char *)
        object - XtOffsetOf(roaring_bitmap_object, std));

    // Call __destruct() from user-land.
    
    zend_objects_destroy_object(object);
#else
    
#endif    
}

// ------------------------------------------------------------------ 
// ------------------------------------------------------------------

/* {{{ php_roaring_bitmap_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_roaring_bitmap_init_globals(zend_roaring_bitmap_globals *roaring_bitmap_globals)
{
	roaring_bitmap_globals->global_value = 0;
	roaring_bitmap_globals->global_string = NULL;
}
*/
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(roaring_bitmap)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(roaring_bitmap)
{
#if defined(COMPILE_DL_ROARING_BITMAP) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(roaring_bitmap)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(roaring_bitmap)
{
	php_info_print_table_start();
	php_info_print_table_header(2, PHP_ROARING_BITMAP_EXTNAME " support", "enabled");
	php_info_print_table_row( 2, "Version",     PHP_ROARING_BITMAP_VERSION );
    php_info_print_table_row( 2, "Anthor",      "Paul Tan" );
    php_info_print_table_row( 2, "Copyright",   "Licensed under the Apache License, Version 2.0 (the \"License\");" );
	php_info_print_table_end();

	char version[20];
	snprintf(version, sizeof(version), "%u.%u.%u", ROARING_VERSION_MAJOR, ROARING_VERSION_MINOR, ROARING_VERSION_REVISION);
	
	php_info_print_table_start();
	php_info_print_table_header( 2, "Library",  "Version" );
	php_info_print_table_row( 2, "CRoaring",   version );
	php_info_print_table_row( 2, "Url",     "https://github.com/RoaringBitmap/CRoaring" );
	php_info_print_table_end();

	php_info_print_table_start();
	php_info_print_table_header( 2,     "Build Environment", "");
	php_info_print_table_row( 2, "PHP",         PHP_VERSION );
	php_info_print_table_end();
	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ roaring_bitmap_functions[]
 *
 * Every user visible function must have an entry in roaring_bitmap_functions[].
 */
const zend_function_entry roaring_bitmap_functions[] = {
	PHP_ME(roaring_bitmap, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(roaring_bitmap, add,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, addChecked,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, addRange,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, addMany,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, remove,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, removeChecked,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, maximum,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, minimum,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, contains,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, containsRange,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, rank,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, select,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, cardinality,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, flip,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, isEmpty,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, getSizeInBytes,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, replace,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, eq,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, sub,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, and,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, or,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, xor,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, and_cardinality,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, or_cardinality,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, andnot_cardinality,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, xor_cardinality,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, jaccard_index,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, isSubset,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, isStrictSubset,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, iterator_begin,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, iterator_end,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, iterator_advance,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, iterator_value,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, toArray,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, toString,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, write,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, read,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, getCopyOnWrite,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, setCopyOnWrite,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, removeRunCompression,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, runOptimize,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap, shrinkToFit,  NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END	/* Must be the last line in roaring_bitmap_functions[] */
};
/* }}} */


//------------------------- 64bit ------------------------------


// ------------------------------------------------------------------
// ------------------------------------------------------------------
/* {{{ proto void roaring_bitmap64::__construct()
   Create the roaring_bitmap64 object */
PHP_METHOD(roaring_bitmap64, __construct)
{
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        intern->roaring = new Roaring64Map();     
    }
}
/* }}} */

/* {{{ proto long roaring_bitmap64::add(string $n)
   Add value n(numeric string) */
PHP_METHOD(roaring_bitmap64, add)
{
    long ret = 0;
    char *arg = NULL;
    size_t arg_len;
    uint64_t n;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
        RETURN_NULL();
    }
    n = strtoull(arg, NULL, 0);

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        intern->roaring->add(n);
        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::addChecked(string $n)
   Add value n. Returns true if a new value was added, false if the value was already existing */
PHP_METHOD(roaring_bitmap64, addChecked)
{
    bool ret = false;
    char *arg = NULL;
    size_t arg_len;
    uint64_t n;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
        RETURN_NULL();
    }
    n = strtoull(arg, NULL, 0);

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->addChecked(n);
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap64::addMany(array $arr)
   Add value from array $arr */
PHP_METHOD(roaring_bitmap64, addMany)
{
    long ret = 0;
    ulong num_key;
    zval *val, *arr;
    HashTable *arr_hash;
    int array_count;

#if PHP_MAJOR_VERSION >= 7
    zend_string *key;
#else
  zval **data;
  HashPosition pointer;
#endif

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "a", &arr) == FAILURE) {
          RETURN_NULL();
    }
    
    arr_hash = Z_ARRVAL_P(arr);
//    array_count = zend_hash_num_elements(arr_hash);
//    php_printf("The array passed contains %d elements\n", array_count);

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
#if PHP_MAJOR_VERSION >= 7      
        ZEND_HASH_FOREACH_KEY_VAL(arr_hash, num_key, key, val){
#else
        for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {
            val = *data;
#endif                   
/*          
            if(key){
                php_printf("string key: %s\n", ZSTR_VAL(key));
            }
            else{
                php_printf("num_key:%u\n", num_key);
            }
*/
            switch (Z_TYPE_P(val)) {
                case IS_LONG:
                    intern->roaring->add((uint64_t)Z_LVAL_P(val));
                    break;

                case IS_STRING:
                    intern->roaring->add((uint64_t)strtoull(Z_STRVAL_P(val), NULL, 0));
                    break;

                case IS_NULL:
#if PHP_MAJOR_VERSION >= 7                
                case IS_TRUE:
                case IS_FALSE:
#else
                case IS_BOOL:
#endif                                
                case IS_ARRAY:
                case IS_OBJECT:
                case IS_RESOURCE:
                default: // ignore
                    break;
            }
#if PHP_MAJOR_VERSION >= 7
        }ZEND_HASH_FOREACH_END();
#else
      }
#endif
//      intern->roaring->addRange(begin, end);
        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap64::remove(string $n)
   Remove value n */
PHP_METHOD(roaring_bitmap64, remove)
{
    long ret = 0;
    char *arg = NULL;
    size_t arg_len;
    uint64_t n;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
        RETURN_NULL();
    }
    n = strtoull(arg, NULL, 0);

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        intern->roaring->remove(n);

        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::removeChecked(string $n)
   Remove value n. Returns true if a new value was removed, false if the value was not existing. */
PHP_METHOD(roaring_bitmap64, removeChecked)
{
    bool ret = false;
    char *arg = NULL;
    size_t arg_len;
    uint64_t n;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
        RETURN_NULL();
    }
    n = strtoull(arg, NULL, 0);

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->removeChecked(n);

        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap64::maximum()
   Return the largest value (if not empty) */
PHP_METHOD(roaring_bitmap64, maximum)
{
    uint64_t ret = 0;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->maximum();

        RETURN_LONGLONG_STR(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap64::minimum()
   Return the smallest value (if not empty) */
PHP_METHOD(roaring_bitmap64, minimum)
{
    uint64_t ret = 0;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->minimum();

        RETURN_LONGLONG_STR(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::contains(string $n)
   Check if value n is present */
PHP_METHOD(roaring_bitmap64, contains)
{
    bool ret = false;
    char *arg = NULL;
    size_t arg_len;
    uint64_t n;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
        RETURN_NULL();
    }
    n = strtoull(arg, NULL, 0);

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->contains(n);

        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap64::rank(string $n)
   Returns the number of integers that are smaller or equal to n. */
PHP_METHOD(roaring_bitmap64, rank)
{
    uint64_t ret = 0;
    char *arg = NULL;
    size_t arg_len;
    uint64_t n;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
        RETURN_NULL();
    }
    n = strtoull(arg, NULL, 0);

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->rank(n);
        
        RETURN_LONGLONG_STR(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap64::select(string $rank)
   If the size of the roaring bitmap is strictly greater than rank, then 
   this function returns the element of given rank. 
   Otherwise, it returns false. */
PHP_METHOD(roaring_bitmap64, select)
{
    bool ret = false;
    char *arg = NULL;
    size_t arg_len;
    uint64_t n;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
        RETURN_NULL();
    }
    n = strtoull(arg, NULL, 0);

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        uint64_t val;
        ret = intern->roaring->select(n, &val);
        if(ret == true){
          RETURN_LONGLONG_STR(val);
        }

        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::flip(string $begin, string $end)
   compute the negation of the roaring bitmap within a specified interval. 
   areas outside the range are passed through unchanged. */
PHP_METHOD(roaring_bitmap64, flip)
{
    bool ret = false;
    char *barg = NULL, *earg = NULL;
    size_t barg_len, earg_len;
    uint64_t begin, end;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "ss", &barg, &barg_len, &earg, &earg_len) == FAILURE) {
          RETURN_NULL();
    }
    begin = strtoull(barg, NULL, 0);
    end = strtoull(earg, NULL, 0);

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        intern->roaring->flip(begin, end);
        ret = true;

        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::isEmpty()
   Returns true if the bitmap is empty (cardinality is zero). */
PHP_METHOD(roaring_bitmap64, isEmpty)
{
    bool ret = false;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->isEmpty();
        
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap64::cardinality()
   Get the cardinality of the bitmap (number of elements). */
PHP_METHOD(roaring_bitmap64, cardinality)
{
    uint64_t ret = 0;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->cardinality();
        
        RETURN_LONGLONG_STR(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap64::getSizeInBytes([bool $portable = false])
   How many bytes are required to serialize this bitmap.
   Setting the portable flag to false enable a custom format that can save 
   space compared to the portable format (e.g., for very sparse bitmaps).*/
PHP_METHOD(roaring_bitmap64, getSizeInBytes)
{
    uint64_t ret = 0;
    bool portable = false;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "|b", &portable) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->getSizeInBytes(portable);

        RETURN_LONGLONG_STR(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap64::replace(roaring_bitmap64 $another)
   Copies the content of the provided bitmap, and discard the current content. */
PHP_METHOD(roaring_bitmap64, replace)
{
    long ret = 0;
    zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap64_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);

    roaring_bitmap64_object *other;
    other = Z_TSTOBJ64_P(other_val);

    if(intern != NULL && other != NULL){
        *(intern->roaring) = *(other->roaring);

        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::eq(roaring_bitmap64 $another)
   Return true if the two bitmaps contain the same elements. */
PHP_METHOD(roaring_bitmap64, eq)
{
    bool ret = false;
    zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap64_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);

    roaring_bitmap64_object *other;
    other = Z_TSTOBJ64_P(other_val);

    if(intern != NULL && other != NULL){
        if(*(intern->roaring) == *(other->roaring))
          ret = true;
        else
          ret = false;
        
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap64::sub(roaring_bitmap64 $another)
   Compute the difference between the current bitmap and the provided bitmap, 
   writing the result in the current bitmap. The provided bitmap is not modified.*/
PHP_METHOD(roaring_bitmap64, sub)
{
    long ret = 0;
    zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap64_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);

    roaring_bitmap64_object *other;
    other = Z_TSTOBJ64_P(other_val);

    if(intern != NULL && other != NULL){
        *(intern->roaring) -= *(other->roaring);

        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap64::and(roaring_bitmap64 $another)
   Copies the content of the provided bitmap, and discard the current content. */
PHP_METHOD(roaring_bitmap64, and)
{
    long ret = 0;
    zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap64_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);

    roaring_bitmap64_object *other;
    other = Z_TSTOBJ64_P(other_val);

    if(intern != NULL && other != NULL){
        *(intern->roaring) &= *(other->roaring);

        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap64::or(roaring_bitmap64 $another)
   Compute the union between the current bitmap and the provided bitmap, 
   writing the result in the current bitmap. The provided bitmap is not modified.*/
PHP_METHOD(roaring_bitmap64, or)
{
    long ret = 0;
    zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap64_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);

    roaring_bitmap64_object *other;
    other = Z_TSTOBJ64_P(other_val);

    if(intern != NULL && other != NULL){
        *(intern->roaring) |= *(other->roaring);

        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap64::xor(roaring_bitmap64 $another)
   Compute the symmetric union between the current bitmap and the provided bitmap,
   writing the result in the current bitmap. The provided bitmap is not modified. */
PHP_METHOD(roaring_bitmap64, xor)
{
    long ret = 0;
    zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap64_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);

    roaring_bitmap64_object *other;
    other = Z_TSTOBJ64_P(other_val);

    if(intern != NULL && other != NULL){
        *(intern->roaring) ^= *(other->roaring);

        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::isSubset(roaring_bitmap64 $another)
   Returns true if the bitmap is subset of the other. */
PHP_METHOD(roaring_bitmap64, isSubset)
{
    bool ret = false;
    zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap64_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);

    roaring_bitmap64_object *other;
    other = Z_TSTOBJ64_P(other_val);

    if(intern != NULL && other != NULL){
        ret = intern->roaring->isSubset(*(other->roaring));

        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::isStrictSubset(roaring_bitmap64 $another)
   Returns true if the bitmap is strict subset of the other. */
PHP_METHOD(roaring_bitmap64, isStrictSubset)
{
    bool ret = false;
    zval* other_val;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "O", &other_val, roaring_bitmap64_ce) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);

    roaring_bitmap64_object *other;
    other = Z_TSTOBJ64_P(other_val);

    if(intern != NULL && other != NULL){
        ret = intern->roaring->isStrictSubset(*(other->roaring));

        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::iterator_begin()
   Iterate over the bitmap elements. Move the iterator to the first value. */
PHP_METHOD(roaring_bitmap64, iterator_begin)
{
    bool ret = true;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        intern->iterator = intern->roaring->begin();
        
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::iterator_end()
   Return false means that the iteration should continue while true means that it should stop. */
PHP_METHOD(roaring_bitmap64, iterator_end)
{
    bool ret = true;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        if(intern->iterator == intern->roaring->end())
            ret = true;
        else
            ret = false;
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::iterator_advance()
   Move the iterator to the next. */
PHP_METHOD(roaring_bitmap64, iterator_advance)
{
    bool ret = true;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ++(intern->iterator);
        
        RETURN_BOOL(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap64::iterator_value()
   Return current iterator value. */
PHP_METHOD(roaring_bitmap64, iterator_value)
{
    uint64_t ret = 0;
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = *(intern->iterator);;
        
        RETURN_LONGLONG_STR(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto array roaring_bitmap64::toArray()
   Convert the bitmap to an array and return it. array size is: this->cardinality() */
PHP_METHOD(roaring_bitmap64, toArray)
{
    array_init(return_value);
    
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        /*
        for(Roaring64Map::const_iterator it = intern->roaring->begin() ; it != intern->roaring->end() ; it++) {
            add_next_index_long(return_value, *it);
        }
        */
        uint64_t num = intern->roaring->cardinality();
        uint64_t* vals = (uint64_t*)malloc(sizeof(uint64_t) * num);
        if(vals == NULL){
            RETURN_NULL();
        }
        intern->roaring->toUint64Array(vals);
        uint64_t* p = vals;
        unsigned int len = 0;
        char buf[32];
        for(uint64_t i=0; i < num; i++, p++){
            len = snprintf(buf, sizeof(buf), "%llu", (unsigned long long)*p);
            MY_ADD_NEXT_INDEX_STRINGL(return_value, buf, len, 1);
        }
        free(vals);

        return;
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto string roaring_bitmap64::toString()
   Print the content of the bitmap into a string and return it. */
PHP_METHOD(roaring_bitmap64, toString)
{
    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        std::string str = intern->roaring->toString();

        MY_RETURN_STRINGL(str.c_str(), str.length(), 1);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::write([bool $portable = false])
   write a bitmap to a char buffer. Returns how many bytes were written which should be getSizeInBytes(). 
   Setting the portable flag to false enable a custom format that can save space compared to the portable format 
   (e.g., for very sparse bitmaps). */
PHP_METHOD(roaring_bitmap64, write)
{
    bool portable = false;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "|b", &portable) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        uint32_t size = intern->roaring->getSizeInBytes(portable);
        char* buf = (char*)emalloc(sizeof(char) * size);
        if(buf == NULL){
            RETURN_NULL();
        }
        size_t len = intern->roaring->write(buf, portable);
       
        MY_RETVAL_STRINGL(buf, len, 0);

        return;
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::read([bool $portable = false])
   read a bitmap from a serialized version. Setting the portable flag to false enable a custom format that 
   can save space compared to the portable format (e.g., for very sparse bitmaps). */
PHP_METHOD(roaring_bitmap64, read)
{
    zval* data;
    bool portable = false;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "z|b", &data, &portable) == FAILURE) {
          RETURN_NULL();
    }
    if(Z_TYPE_P(data) != IS_STRING){
      RETURN_FALSE;
    }

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        try{
//          Roaring64Map new_roaring = Roaring64Map::readSafe(Z_STRVAL_P(data), Z_STRLEN_P(data));
            Roaring64Map new_roaring = Roaring64Map::read(Z_STRVAL_P(data), portable);
            intern->roaring->swap(new_roaring);
        }
        catch(std::exception& e){
             RETURN_NULL();
        }

        RETURN_TRUE;
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::getCopyOnWrite()
   Whether or not copy and write is active. */
PHP_METHOD(roaring_bitmap64, getCopyOnWrite)
{
    bool bval = false;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        bval = intern->roaring->getCopyOnWrite();

        RETURN_BOOL(bval);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::setCopyOnWrite()
   Whether or not we apply copy and write. */
PHP_METHOD(roaring_bitmap64, setCopyOnWrite)
{
    long ret = 0;
    bool bval = false;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    if (zend_parse_parameters(
      ZEND_NUM_ARGS() TSRMLS_CC, "|b", &bval) == FAILURE) {
          RETURN_NULL();
    }

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        intern->roaring->setCopyOnWrite(bval);

        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::removeRunCompression()
   Remove run-length encoding even when it is more space efficient return whether a change was applied. */
PHP_METHOD(roaring_bitmap64, removeRunCompression)
{
    bool bval = false;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        bval = intern->roaring->removeRunCompression();

        RETURN_BOOL(bval);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto bool roaring_bitmap64::runOptimize()
   convert array and bitmap containers to run containers when it is more efficient; also convert from run 
   containers when more space efficient.  Returns true if the result has at least one run container. 
   Additional savings might be possible by calling shrinkToFit(). */
PHP_METHOD(roaring_bitmap64, runOptimize)
{
    bool bval = false;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        bval = intern->roaring->runOptimize();

        RETURN_BOOL(bval);
    }

    RETURN_NULL();
}
/* }}} */

/* {{{ proto long roaring_bitmap64::shrinkToFit()
   If needed, reallocate memory to shrink the memory usage. Returns the number of bytes saved. */
PHP_METHOD(roaring_bitmap64, shrinkToFit)
{
    long ret = 0;

    zval *zobj = getThis();
    roaring_bitmap64_object *intern;

    intern = Z_TSTOBJ64_P(zobj);
    if(intern != NULL) {
        ret = intern->roaring->shrinkToFit();

        RETURN_LONG(ret);
    }

    RETURN_NULL();
}
/* }}} */

// ------------------------------------------------------------------ 
// ------------------------------------------------------------------

// ------------------------------------------------------------------ 
// ------------------------------------------------------------------

static void roaring_bitmap64_object_free(zend_object *object)
{
    roaring_bitmap64_object *my_obj;
    my_obj = (roaring_bitmap64_object *)((char *)
        object - XtOffsetOf(roaring_bitmap64_object, std));
    delete my_obj->roaring;
    my_obj->roaring = NULL;

    // Free the object using Zend macro.
    zend_object_std_dtor(object); 

#if PHP_MAJOR_VERSION < 7
	efree(my_obj);
#endif    
}

#if PHP_MAJOR_VERSION >= 7
zend_object* roaring_bitmap64_object_new(zend_class_entry *ce TSRMLS_DC)
#else
zend_object_value roaring_bitmap64_object_new(zend_class_entry *ce TSRMLS_DC)
#endif
{
#if PHP_MAJOR_VERSION >= 7  
    roaring_bitmap64_object *intern = (roaring_bitmap64_object*)ecalloc(1,
            sizeof(roaring_bitmap64_object) +
            zend_object_properties_size(ce));

    zend_object_std_init(&intern->std, ce TSRMLS_CC);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = &roaring_bitmap64_object_handlers;

    return &intern->std;
#else
    zval *tmp;
    zend_object_value retval;

    roaring_bitmap64_object *intern = (roaring_bitmap64_object *)ecalloc(1, sizeof(roaring_bitmap64_object));
    intern->std.ce = ce;

    ALLOC_HASHTABLE(intern->std.properties);
    zend_hash_init(intern->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    
    PROP_INIT;

    retval.handle = zend_objects_store_put(intern, NULL,
        (zend_objects_free_object_storage_t)roaring_bitmap64_object_free, NULL TSRMLS_CC);
    retval.handlers = &roaring_bitmap64_object_handlers;

    return retval;
#endif    
}

static void roaring_bitmap64_object_destroy(zend_object *object)
{
#if PHP_MAJOR_VERSION >= 7  
    roaring_bitmap64_object *my_obj;
    my_obj = (roaring_bitmap64_object*)((char *)
        object - XtOffsetOf(roaring_bitmap64_object, std));

    // Call __destruct() from user-land.    
    zend_objects_destroy_object(object);
#else
    
#endif    
}

// ------------------------------------------------------------------ 
// ------------------------------------------------------------------




/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(roaring_bitmap64)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(roaring_bitmap64)
{
#if defined(COMPILE_DL_ROARING_BITMAP64) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(roaring_bitmap64)
{
    return SUCCESS;
}
/* }}} */


/* {{{ roaring_bitmap64_functions[]
 *
 * Every user visible function must have an entry in roaring_bitmap64_functions[].
 */
const zend_function_entry roaring_bitmap64_functions[] = {
    PHP_ME(roaring_bitmap64, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(roaring_bitmap64, add,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, addChecked,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, addMany,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, remove,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, removeChecked,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, maximum,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, minimum,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, contains,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, rank,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, select,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, cardinality,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, flip,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, isEmpty,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, getSizeInBytes,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, replace,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, eq,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, sub,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, and,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, or,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, xor,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, isSubset,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, isStrictSubset,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, iterator_begin,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, iterator_end,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, iterator_advance,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, iterator_value,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, toArray,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, toString,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, write,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, read,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, getCopyOnWrite,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, setCopyOnWrite,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, removeRunCompression,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, runOptimize,  NULL, ZEND_ACC_PUBLIC)
    PHP_ME(roaring_bitmap64, shrinkToFit,  NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END  /* Must be the last line in roaring_bitmap64_functions[] */
};
/* }}} */

#if PHP_MAJOR_VERSION < 7

static ZEND_RSRC_DTOR_FUNC( destroy_roaring_bitmap )
{
    if ( RSRC_PTR ) {
        delete (Roaring *)RSRC_PTR;   
        RSRC_PTR = NULL;
    }
}

static ZEND_RSRC_DTOR_FUNC( destroy_roaring_bitmap64 )
{
    if ( RSRC_PTR ) {
        delete (Roaring64Map *)RSRC_PTR;     
        RSRC_PTR = NULL;
    }
}

#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(roaring_bitmap)
{
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "roaring_bitmap", roaring_bitmap_functions);
    roaring_bitmap_ce = zend_register_internal_class(&ce TSRMLS_CC);
    roaring_bitmap_ce->create_object = roaring_bitmap_object_new;

    memcpy(&roaring_bitmap_object_handlers, 
        zend_get_std_object_handlers(), 
        sizeof(roaring_bitmap_object_handlers));

#if PHP_MAJOR_VERSION >= 7
    // Handler for free'ing the object.
    roaring_bitmap_object_handlers.free_obj = roaring_bitmap_object_free;

    // Handler for the destructor.
    roaring_bitmap_object_handlers.dtor_obj = roaring_bitmap_object_destroy; 

    // Offset into the engine.
    roaring_bitmap_object_handlers.offset = XtOffsetOf(roaring_bitmap_object, std);
#else
    le_roaring_bitmap  = zend_register_list_destructors_ex( destroy_roaring_bitmap,  NULL, "roaring_bitmap",  module_number );    
#endif

    //--------------- 64 bit ------------------------
    zend_class_entry ce64;
    INIT_CLASS_ENTRY(ce64, "roaring_bitmap64", roaring_bitmap64_functions);
    roaring_bitmap64_ce = zend_register_internal_class(&ce64 TSRMLS_CC);
    roaring_bitmap64_ce->create_object = roaring_bitmap64_object_new;

    memcpy(&roaring_bitmap64_object_handlers, 
        zend_get_std_object_handlers(), 
        sizeof(roaring_bitmap64_object_handlers));

#if PHP_MAJOR_VERSION >= 7 
    // Handler for free'ing the object.
    roaring_bitmap64_object_handlers.free_obj = roaring_bitmap64_object_free;

    // Handler for the destructor.
    roaring_bitmap64_object_handlers.dtor_obj = roaring_bitmap64_object_destroy; 

    // Offset into the engine.
    roaring_bitmap64_object_handlers.offset = XtOffsetOf(roaring_bitmap64_object, std);
#else
    le_roaring_bitmap64  = zend_register_list_destructors_ex( destroy_roaring_bitmap64,  NULL, "roaring_bitmap64",  module_number );    
#endif


    return SUCCESS;
}
/* }}} */



//------------------------- 64bit end --------------------------


/* {{{ roaring_bitmap_module_entry
 */
zend_module_entry roaring_bitmap_module_entry = {
    STANDARD_MODULE_HEADER,
    "roaring_bitmap",
    roaring_bitmap_functions,
    PHP_MINIT(roaring_bitmap),
    PHP_MSHUTDOWN(roaring_bitmap),
    PHP_RINIT(roaring_bitmap),      /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(roaring_bitmap),  /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(roaring_bitmap),
    PHP_ROARING_BITMAP_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */



#ifdef COMPILE_DL_ROARING_BITMAP
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif

ZEND_GET_MODULE(roaring_bitmap)

#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
