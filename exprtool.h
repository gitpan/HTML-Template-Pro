/* -*- c -*- 
 * File: exprtool.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Mon Jul 25 15:29:04 2005
 *
 * $Id$
 */

#ifndef _EXPRTOOL_H
#define _EXPRTOOL_H	1

#include "pstring.h"

#define DO_MATHOP(z,op,x,y) switch (z.type=expr_to_int_or_dbl(&x,&y)) { \
case EXPRINT: z.val.intval=x.val.intval op y.val.intval;break; \
case EXPRDBL: z.val.dblval=x.val.dblval op y.val.dblval;break; \
}

#define DO_LOGOP(z,op,x,y) z.type=EXPRINT; switch (expr_to_int_or_dbl_logop(&x,&y)) { \
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

static
exprtype expr_to_int_or_dbl (struct exprval* val1, struct exprval* val2);
static
exprtype expr_to_int_or_dbl_logop (struct exprval* val1, struct exprval* val2);
static
void expr_to_dbl (struct exprval* val1, struct exprval* val2);
static
void expr_to_int (struct exprval* val1, struct exprval* val2);
static
void expr_to_dbl1 (struct exprval* val);
static
void expr_to_str (struct exprval* val1, struct exprval* val2);
static
void expr_to_num (struct exprval* val1);
static
void expr_to_bool (struct exprval* val1);
static
struct exprval exp_read_number (char* *curposptr, char* endchars);

/* this stuff is defined or used in expr.y */
static
void expr_debug(char const *,char const *);

/* 
 * for snprintf %f
 * internal expr buffers for conversion int/double --> string 
 */
static
char left_buffer[40]; 
static
char right_buffer[40];

#endif /* exprtool.h */
