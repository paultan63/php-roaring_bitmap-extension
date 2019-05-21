#ifndef MY_PHP_DEFINE_H
#define MY_PHP_DEFINE_H

#include "php.h"

#define PRIu32 "u"
#define PRId32 "d"

#if PHP_VERSION_ID < 50399
#define PROP_INIT \
  zend_hash_copy( intern->std.properties, \
                        &ce->default_properties, \
                        (copy_ctor_func_t) zval_add_ref, \
                        (void *) &tmp, \
                        sizeof(zval *) )
#elif PHP_MAJOR_VERSION < 7
#define PROP_INIT \
  object_properties_init((zend_object*) &(intern->std), ce)
#endif

// -------------------- PHP 7 -------------------
#if PHP_MAJOR_VERSION >= 7

#define MY_RETURN_STRINGL(s, l, dup)    RETURN_STRINGL(s, l)  
#define MY_RETVAL_STRINGL(s, l, dup)    do{RETVAL_STRINGL(s, l); if (dup == 0) efree(s);}while(0)  
#define MY_RETVAL_STRING(s, dup)        do{RETVAL_STRING(s); if (dup == 0) efree(s);}while(0)  
#define MY_RETURN_STRING(val, dup)     RETURN_STRING(val)
#define MY_ZVAL_STRINGL(z, s, l, dup)         ZVAL_STRINGL(z, s, l)  
#define MY_ZVAL_STRING(z,s,dup)               ZVAL_STRING(z,s)  

#define RSRC_PTR (res->ptr)
#define RSRC_ENTRY_NAME zend_resource
#define RSRC_TYPE(r) (r.type)
#define RSRC_TYPE_P(r) (r->type)

#define RETURN_LONGLONG_STR(ret) \
  zend_string *strg = strpprintf(0, "%llu", (unsigned long long)ret); \
  RETURN_STR(strg)

#define MY_ADD_NEXT_INDEX_STRINGL(arr, str, len, dup) add_next_index_stringl(arr, str, len)

// ------------------- PHP 5 --------------------
#else

#define MY_RETURN_STRINGL    RETURN_STRINGL  
#define MY_RETVAL_STRINGL    RETVAL_STRINGL 
#define MY_RETVAL_STRING     RETVAL_STRING  
#define MY_RETURN_STRING     RETURN_STRING  
#define MY_ZVAL_STRINGL     ZVAL_STRINGL  
#define MY_ZVAL_STRING      ZVAL_STRING 

#define RSRC_PTR (rsrc->ptr)
#define RSRC_ENTRY_NAME zend_rsrc_list_entry
#define RSRC_TYPE(r) (r.type)
#define RSRC_TYPE_P(r) (r->type)

#define RETURN_LONGLONG_STR(ret) \
  char strg[32]; \
  int len = snprintf(strg, sizeof(strg), "%llu", (unsigned long long)ret); \
  if(len < 0) \
    len = 0; \
  RETURN_STRINGL(strg, len, 1)

#define MY_ADD_NEXT_INDEX_STRINGL(arr, str, len, dup) add_next_index_stringl(arr, str, len, dup)

#endif
// ----------------- END -------------------------

#endif
