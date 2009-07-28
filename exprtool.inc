/* -*- c -*- 
 * File: exprtool.c
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Mon Jul 25 15:29:17 2005
 *
 * $Id$
 */

/*
#include "exprtool.h"
#include <ctype.h> // for yylex alnum
#include <stdio.h> // for printf
*/

#define EXPR_CHECK_NUMBER(state, ptr)   switch (ptr->type) {	\
  case EXPR_TYPE_INT: case EXPR_TYPE_DBL: break; \
  case EXPR_TYPE_PSTR: expr_to_num(state, ptr); break;				\
  default: _tmplpro_expnum_debug(*ptr, "FATAL:internal exprtype error. please report\n"); \
  ptr->type = EXPR_TYPE_INT; \
}

#define EXPR_CHECK_LOGICAL(state, ptr)   switch (ptr->type) {	\
  case EXPR_TYPE_INT: case EXPR_TYPE_DBL: break; \
  case EXPR_TYPE_PSTR: expr_to_bool(state, ptr); break;			\
  default: _tmplpro_expnum_debug(*ptr, "FATAL:internal exprtype error. please report\n"); \
  ptr->type = EXPR_TYPE_INT; \
}

static
exprtype 
expr_to_int_or_dbl (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_NUMBER(state, val1);
  EXPR_CHECK_NUMBER(state, val2);
  if ((val1->type == EXPR_TYPE_INT) && (val2->type == EXPR_TYPE_INT))
    return EXPR_TYPE_INT;
  if ((val1->type == EXPR_TYPE_DBL) && (val2->type == EXPR_TYPE_DBL))
    return EXPR_TYPE_DBL;
  if (val1->type == EXPR_TYPE_INT) {
    val1->type=EXPR_TYPE_DBL;
    val1->val.dblval=val1->val.intval;
  }
  if (val2->type == EXPR_TYPE_INT) {
    val1->type=EXPR_TYPE_DBL;
    val2->val.dblval=val2->val.intval;
  }
  return EXPR_TYPE_DBL;
}

static
exprtype 
expr_to_int_or_dbl_logop (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_LOGICAL(state, val1);
  EXPR_CHECK_LOGICAL(state, val2);
  if (val1->type == EXPR_TYPE_INT && val2->type == EXPR_TYPE_INT)
    return EXPR_TYPE_INT;
  if (val1->type == EXPR_TYPE_DBL && val2->type == EXPR_TYPE_DBL)
    return EXPR_TYPE_DBL;
  if (val1->type == EXPR_TYPE_INT) {
    val1->type=EXPR_TYPE_DBL;
    val1->val.dblval=val1->val.intval;
  }
  if (val2->type == EXPR_TYPE_INT) {
    val1->type=EXPR_TYPE_DBL;
    val2->val.dblval=val2->val.intval;
  }
  return EXPR_TYPE_DBL;
}

static
void 
expr_to_dbl (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_NUMBER(state, val1);
  EXPR_CHECK_NUMBER(state, val2);
  if (val1->type == EXPR_TYPE_INT) {
    val1->type=EXPR_TYPE_DBL;
    val1->val.dblval=val1->val.intval;
  }
  if (val2->type == EXPR_TYPE_INT) {
    val1->type=EXPR_TYPE_DBL;
    val2->val.dblval=val2->val.intval;
  }
}

static
void 
expr_to_dbl1 (struct tmplpro_state* state, struct exprval* val1) {
  EXPR_CHECK_NUMBER(state, val1);
  if (val1->type == EXPR_TYPE_INT) {
    val1->type=EXPR_TYPE_DBL;
    val1->val.dblval=val1->val.intval;
  }
}

static
void 
expr_to_int (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_NUMBER(state, val1);
  EXPR_CHECK_NUMBER(state, val2);
  if (val1->type == EXPR_TYPE_DBL) {
    val1->type=EXPR_TYPE_INT;
    val1->val.intval=(EXPR_int64) val1->val.dblval;
    /* _tmplpro_expnum_debug(*val1, "WARN:converting to `int' from `double'"); */
  }
  if (val2->type == EXPR_TYPE_DBL) {
    val2->type=EXPR_TYPE_INT;
    val2->val.intval=(EXPR_int64) val2->val.dblval;
    /* _tmplpro_expnum_debug(*val2, "WARN:converting to `int' from `double'"); */
  }
}

static
void 
expr_to_str (struct exprval* val1, struct exprval* val2, struct tmplpro_param* param) {
  switch (val1->type) {
  case EXPR_TYPE_PSTR: break;
  case EXPR_TYPE_INT: val1->val.strval=int_to_pstring(val1->val.intval,param->left_buffer,sizeof(param->left_buffer)); break;
  case EXPR_TYPE_DBL: val1->val.strval=double_to_pstring(val1->val.dblval,param->left_buffer,sizeof(param->left_buffer)); break;
  default: _tmplpro_expnum_debug(*val1, "FATAL:internal expr string error. please report\n");
  }
  val1->type = EXPR_TYPE_PSTR;

  switch (val2->type) {
  case EXPR_TYPE_PSTR: break;
  case EXPR_TYPE_INT: val2->val.strval=int_to_pstring(val2->val.intval,param->right_buffer,sizeof(param->right_buffer)); break;
  case EXPR_TYPE_DBL: val2->val.strval=double_to_pstring(val2->val.dblval,param->right_buffer,sizeof(param->right_buffer)); break;
  default: _tmplpro_expnum_debug(*val2, "FATAL:internal expr string error. please report\n");
  }
  val2->type = EXPR_TYPE_PSTR;
}

static
int 
is_float_lex (char c)
{
  return (c == '.' || isdigit (c));
}

static
struct exprval 
exp_read_number (struct tmplpro_state* state, char* *curposptr, char* endchars) {
  char c = **curposptr;
  struct exprval retval;
  EXPR_int64 iretval=0;
  double dretval=0;
  EXPR_int64 offset=0;
  int sign=1;
  retval.type=EXPR_TYPE_INT;
  retval.val.intval=0;
  if ((*curposptr)<endchars && '-' == c) {
    sign=-1;
    c = *(++(*curposptr));
  }
  if (! (c == '.' || isdigit (c))) return retval;
  /* double reader
     yylval.dblval=atof(fill_symbuf(is_float_lex).begin);
     return dblNUM;
  */
  while ((*curposptr)<endchars && is_float_lex(c)) {
    if (c == '.') {
      if (retval.type == EXPR_TYPE_INT) {
	retval.type = EXPR_TYPE_DBL;
	dretval=iretval;
	offset=1;
      } else {
	/* (*curposptr)--; ??? */
	expr_debug(state, "uninspected declimal point","");
	retval.val.dblval=dretval*sign;
	retval.type=EXPR_TYPE_DBL;
	return retval;
      }
    } else {
      offset*=10;
      if (retval.type == EXPR_TYPE_INT) {
	iretval=iretval*10+c-'0';
      } else {
	dretval=dretval*10+c-'0';
      }
    }
    c = *(++(*curposptr));
  }
  if (retval.type == EXPR_TYPE_INT) {
    retval.val.intval=iretval*sign;
  } else {
    if (offset) dretval/=offset;
    retval.val.dblval=dretval*sign;
  }
  return retval;
}

static
void 
expr_to_num (struct tmplpro_state* state, struct exprval* val1)
{
  if (val1->type == EXPR_TYPE_PSTR) {
    char* curpos=val1->val.strval.begin;
    *val1=exp_read_number (state, &curpos, val1->val.strval.endnext);
  }
}

static
void 
expr_to_bool (struct tmplpro_state* state, struct exprval* val1)
{
  if (val1->type == EXPR_TYPE_PSTR) {
    char* begin=val1->val.strval.begin;
    char* end=val1->val.strval.endnext;
    char* curpos=begin;
    *val1=exp_read_number (state, &curpos, end);
    if (val1->type == EXPR_TYPE_INT) {
      if (val1->val.intval || (curpos == end)) return;
      else val1->val.intval=1; /* strings are true in perl */
    }
    else
      if (val1->type == EXPR_TYPE_DBL) {
	if (val1->val.dblval || (curpos == end)) return;
	else val1->val.dblval=1.0; /* strings are true in perl */
      }
  }
}

static
void 
_tmplpro_expnum_debug (struct exprval val, char* msg) 
{
  tmpl_log(NULL,TMPL_LOG_DEBUG,"--> debug %s:type %c ",msg,val.type);
  if (val.type == EXPR_TYPE_INT)
    tmpl_log(NULL,TMPL_LOG_DEBUG,"ival=%" EXPR_modifier "d\n",val.val.intval);
  else if (val.type == EXPR_TYPE_DBL)
    tmpl_log(NULL,TMPL_LOG_DEBUG,"dval=%f\n",val.val.dblval);
  else if (val.type == EXPR_TYPE_PSTR) {
    tmpl_log(NULL,TMPL_LOG_DEBUG,"pstr(%c):",(int) val.type);
    if (NULL==val.val.strval.begin) tmpl_log(NULL,TMPL_LOG_DEBUG,"{begin=NULL}");
    if (NULL==val.val.strval.endnext) tmpl_log(NULL,TMPL_LOG_DEBUG,"{endnext=NULL}");
    tmpl_log(NULL,TMPL_LOG_DEBUG,"sval=%.*s\n",(int)(val.val.strval.endnext-val.val.strval.begin),val.val.strval.begin);
  } else {
    tmpl_log(NULL,TMPL_LOG_DEBUG,"unknown(%c) as ival=%" EXPR_modifier "d\n",(int) val.type,val.val.intval);
  }
}

/* 
 * checks if it is stringval and unescape it (copies to the buffer).
 * it implies that only one string at a time can use buffer.
 */
static
PSTRING
expr_unescape_pstring_val(struct tmplpro_state* state, PSTRING val) {
  PSTRING retval;
    char* curpos = val.begin;
    char* endnext= val.endnext;
    char* buf=pbuffer_resize(&(state->expr_pusharg_buffer), endnext-curpos+1);
    char* bufpos = buf;
    while (curpos < endnext) {
      if (*curpos == '\\') {
	*bufpos=*(++curpos);
      } else {
	*bufpos=*curpos;
      }
      curpos++;
      bufpos++;
    }
    retval.begin = buf;
    retval.endnext = bufpos;
    return retval;
}
