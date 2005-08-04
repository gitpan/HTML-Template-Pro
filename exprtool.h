/* -*- c -*- 
 * File: exprtool.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Mon Jul 25 15:29:04 2005
 *
 * $Id: exprtool.h,v 1.9 2005/07/28 11:21:29 igor Exp $
 */

#ifndef _EXPRTOOL_H
#define _EXPRTOOL_H	1

#include "pstring.h"

typedef char exprtype;

#define EXPRINT 'i'
#define EXPRDBL 'd'
#define EXPRPSTR 'p'

#define DO_MATHOP(z,op,x,y) switch (z.type=expr_to_int_or_dbl(&x,&y)) { \
case EXPRINT: z.val.intval=x.val.intval op y.val.intval;break; \
case EXPRDBL: z.val.dblval=x.val.dblval op y.val.dblval;break; \
}

#define DO_LOGOP(z,op,x,y) z.type=EXPRINT; switch (expr_to_int_or_dbl(&x,&y)) { \
case EXPRINT: z.val.intval=x.val.intval op y.val.intval;break; \
case EXPRDBL: z.val.intval=x.val.dblval op y.val.dblval;break; \
}

#define DO_LOGOP1(z,op,x) z.type=EXPRINT; switch (x.type) { \
case EXPRINT: z.val.intval= op x.val.intval;break; \
case EXPRDBL: z.val.intval= op x.val.dblval;break; \
}

#define DO_CMPOP(z,op,x,y) switch (expr_to_int_or_dbl(&x,&y)) { \
case EXPRINT: z.val.intval=x.val.intval op y.val.intval;break; \
case EXPRDBL: z.val.intval=x.val.dblval op y.val.dblval;break; \
}; z.type=EXPRINT;

#define DO_TXTOP(z,op,x,y) expr_to_str(&x,&y); z.type=EXPRINT; z.val.intval = op (x.val.strval,y.val.strval);


struct exprval {
  exprtype type;
  union uval {
    int  intval; 		/* integer */
    double dblval;		/* double */
    PSTRING strval;
  } val;
};

exprtype expr_to_int_or_dbl (struct exprval* val1, struct exprval* val2);
void expr_to_dbl (struct exprval* val1, struct exprval* val2);
void expr_to_int (struct exprval* val1, struct exprval* val2);
void expr_to_dbl1 (struct exprval* val);
void expr_to_str (struct exprval* val1, struct exprval* val2);
void expr_to_num (struct exprval* val1);
struct exprval exp_read_number (char* *curposptr, char* endchars);

void expnum_debug (struct exprval val, char* msg);

/* this stuff is defined or used in expr.y */
void expr_debug(char const *,char const *);

/* 
 * for sprintf %f
 * internal expr buffers for conversion int/double --> string 
 */
extern char left_buffer[40]; 
extern char right_buffer[40];

#endif /* exprtool.h */
