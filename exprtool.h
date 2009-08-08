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
#include "exprval.h"

#define DO_MATHOP(state, z,op,x,y) switch (z.type=expr_to_int_or_dbl(state, &x,&y)) { \
case EXPR_TYPE_INT: z.val.intval=x.val.intval op y.val.intval;break; \
case EXPR_TYPE_DBL: z.val.dblval=x.val.dblval op y.val.dblval;break; \
}

#define DO_LOGOP(state, z,op,x,y) z.type=EXPR_TYPE_INT; switch (expr_to_int_or_dbl_logop(state, &x,&y)) { \
case EXPR_TYPE_INT: z.val.intval=x.val.intval op y.val.intval;break; \
case EXPR_TYPE_DBL: z.val.intval=x.val.dblval op y.val.dblval;break; \
}

#define DO_LOGOP1(z,op,x) z.type=EXPR_TYPE_INT; switch (x.type) { \
case EXPR_TYPE_INT: z.val.intval= op x.val.intval;break; \
case EXPR_TYPE_DBL: z.val.intval= op x.val.dblval;break; \
}

#define DO_CMPOP(state, z,op,x,y) switch (expr_to_int_or_dbl(state, &x,&y)) { \
case EXPR_TYPE_INT: z.val.intval=x.val.intval op y.val.intval;break; \
case EXPR_TYPE_DBL: z.val.intval=x.val.dblval op y.val.dblval;break; \
}; z.type=EXPR_TYPE_INT;

#define DO_TXTOP(z,op,x,y,buf) expr_to_str(&x,&y,buf); z.type=EXPR_TYPE_INT; z.val.intval = op (x.val.strval,y.val.strval);

static
EXPR_char expr_to_int_or_dbl (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2);
static
EXPR_char expr_to_int_or_dbl_logop (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2);
static
void expr_to_dbl (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2);
static
void expr_to_int (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2);
static
void expr_to_dbl1 (struct tmplpro_state* state, struct exprval* val);
static
void expr_to_str (struct exprval* val1, struct exprval* val2, struct tmplpro_param* param);
static
void expr_to_num (struct tmplpro_state* state, struct exprval* val1);
static
void expr_to_bool (struct tmplpro_state* state, struct exprval* val1);
static
struct exprval exp_read_number (struct tmplpro_state* state, char* *curposptr, char* endchars);

/* this stuff is defined or used in expr.y */
static
void expr_debug(struct tmplpro_state* state, char const *,char const *);

static
PSTRING expr_unescape_pstring_val(struct tmplpro_state* state, PSTRING val);

static
void _tmplpro_expnum_debug (struct exprval val, char* msg);


struct user_func_call {
  ABSTRACT_USERFUNC* func;  /* for user-defined function name */
  ABSTRACT_ARGLIST* arglist;
};

static
struct exprval call_expr_userfunc(struct tmplpro_state* state, struct user_func_call extfunc);
static
void pusharg_expr_userfunc(struct tmplpro_state* state, struct user_func_call extfunc, struct exprval arg);

#endif /* exprtool.h */
