/* -*- c -*- 
 * File: exprval.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Mon Jul 20 21:10:57 2009
 */

#ifndef _EXPRVAL_H
#define _EXPRVAL_H	1

#if HAVE_STDINT_H
# include <stdint.h>
#endif

#define EXPR_TYPE_INT 'i'
#define EXPR_TYPE_DBL 'd'
#define EXPR_TYPE_PSTR 'p'

#if defined INT64_NAME && ! defined int64_t && ! defined INT64_MAX
#define int64_t INT64_NAME
#endif 

#if defined INT64_MAX || defined int64_t
     typedef int64_t EXPR_int64;
#  ifdef _MSC_VER
#    define EXPR_modifier "I64"
#  else
#    define EXPR_modifier "ll"
#  endif
#else 
     typedef long EXPR_int64;
#    define EXPR_modifier "l"
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
