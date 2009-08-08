/*! \file exprtype.h
    \brief description of EXPR variable type.
    
    EXPR variable type is passed to and from user-supplied functions.

    \author Igor Vlasenko <vlasenko@imath.kiev.ua>
    \warning This header file should never be included directly.
    Include <tmplpro.h> instead.
*/

#ifndef _EXPRTYPE_H
#define _EXPRTYPE_H	1

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

struct exprval;

#endif /* exprtype.h */

/*
 *  Local Variables:
 *  mode: c
 *  End:
 */
