/* -*- c -*- 
 * File: exprval.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Mon Jul 20 21:10:57 2009
 */

#ifndef _EXPRVAL_H
#define _EXPRVAL_H	1

#if HAVE_INTTYPES_H
#  include <inttypes.h>
#else
#  if HAVE_STDINT_H
#    include <stdint.h>
#  endif
#endif

#define EXPR_TYPE_INT 'i'
#define EXPR_TYPE_DBL 'd'
#define EXPR_TYPE_PSTR 'p'

#if defined INT64_MAX || defined int64_t
   typedef int64_t EXPR_int64;
#else 
#  if defined SIZEOF_LONG_LONG && SIZEOF_LONG_LONG == 8
     typedef long long int EXPR_int64;
#  else 
#    if defined INT64_NAME
       typedef  INT64_NAME EXPR_int64;
#    else
       typedef long int EXPR_int64;
#    endif 
#  endif 
#endif 

#if defined PRId64
#    define EXPR_PRId64 PRId64
#else
#  if defined SIZEOF_LONG_LONG && SIZEOF_LONG_LONG == 8
#    define EXPR_PRId64 "lld"
#  else
#    ifdef _MSC_VER
#      define EXPR_PRId64 "I64d"
#    else 
#      define EXPR_PRId64 "ld"
#    endif
#  endif
#endif 


/* 
 * note that struct exprval is private structure,
 * not a part of the API, and is subject to change without prior notice.
 */
typedef char exprtype;
struct exprval {
  exprtype type;
  union uval {
    EXPR_int64  intval; 	/* integer */
    double dblval;		/* double */
    PSTRING strval;
  } val;
};

#endif /* exprval.h */

/*
 *  Local Variables:
 *  mode: c
 *  End:
 */
