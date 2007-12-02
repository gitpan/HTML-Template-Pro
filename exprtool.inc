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
  case EXPRINT: case EXPRDBL: break; \
  case EXPRPSTR: expr_to_num(state, ptr); break;				\
  default: _tmplpro_expnum_debug(*ptr, "FATAL:internal exprtype error. please report\n"); \
  ptr->type = EXPRINT; \
}

#define EXPR_CHECK_LOGICAL(state, ptr)   switch (ptr->type) {	\
  case EXPRINT: case EXPRDBL: break; \
  case EXPRPSTR: expr_to_bool(state, ptr); break;			\
  default: _tmplpro_expnum_debug(*ptr, "FATAL:internal exprtype error. please report\n"); \
  ptr->type = EXPRINT; \
}

/* 
 * for snprintf %f
 * internal expr buffers for conversion int/double --> string 
 * static char left_buffer[40]; 
 * static char right_buffer[40];
 */

static
exprtype 
expr_to_int_or_dbl (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_NUMBER(state, val1);
  EXPR_CHECK_NUMBER(state, val2);
  if (val1->type == EXPRINT && val2->type == EXPRINT)
    return EXPRINT;
  if (val1->type == EXPRDBL && val2->type == EXPRDBL)
    return EXPRDBL;
  if (val1->type == EXPRINT) {
    val1->type=EXPRDBL;
    val1->val.dblval=val1->val.intval;
  }
  if (val2->type == EXPRINT) {
    val1->type=EXPRDBL;
    val2->val.dblval=val2->val.intval;
  }
  return EXPRDBL;
}

static
exprtype 
expr_to_int_or_dbl_logop (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_LOGICAL(state, val1);
  EXPR_CHECK_LOGICAL(state, val2);
  if (val1->type == EXPRINT && val2->type == EXPRINT)
    return EXPRINT;
  if (val1->type == EXPRDBL && val2->type == EXPRDBL)
    return EXPRDBL;
  if (val1->type == EXPRINT) {
    val1->type=EXPRDBL;
    val1->val.dblval=val1->val.intval;
  }
  if (val2->type == EXPRINT) {
    val1->type=EXPRDBL;
    val2->val.dblval=val2->val.intval;
  }
  return EXPRDBL;
}

static
void 
expr_to_dbl (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_NUMBER(state, val1);
  EXPR_CHECK_NUMBER(state, val2);
  if (val1->type == EXPRINT) {
    val1->type=EXPRDBL;
    val1->val.dblval=val1->val.intval;
  }
  if (val2->type == EXPRINT) {
    val1->type=EXPRDBL;
    val2->val.dblval=val2->val.intval;
  }
}

static
void 
expr_to_dbl1 (struct tmplpro_state* state, struct exprval* val1) {
  EXPR_CHECK_NUMBER(state, val1);
  if (val1->type == EXPRINT) {
    val1->type=EXPRDBL;
    val1->val.dblval=val1->val.intval;
  }
}

static
void 
expr_to_int (struct tmplpro_state* state, struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_NUMBER(state, val1);
  EXPR_CHECK_NUMBER(state, val2);
  if (val1->type == EXPRDBL) {
    val1->type=EXPRINT;
    val1->val.intval=val1->val.dblval;
    /* _tmplpro_expnum_debug(*val1, "WARN:converting to `int' from `double'"); */
  }
  if (val2->type == EXPRDBL) {
    val2->type=EXPRINT;
    val2->val.intval=val2->val.dblval;
    /* _tmplpro_expnum_debug(*val2, "WARN:converting to `int' from `double'"); */
  }
}

static
void 
expr_to_str (struct exprval* val1, struct exprval* val2) {
  switch (val1->type) {
  case EXPRPSTR: break;
  case EXPRINT: val1->val.strval=int_to_pstring(val1->val.intval,left_buffer,sizeof(left_buffer)); break;
  case EXPRDBL: val1->val.strval=double_to_pstring(val1->val.dblval,left_buffer,sizeof(left_buffer)); break;
  default: _tmplpro_expnum_debug(*val1, "FATAL:internal expr string error. please report\n");
  }
  val1->type = EXPRPSTR;

  switch (val2->type) {
  case EXPRPSTR: break;
  case EXPRINT: val2->val.strval=int_to_pstring(val2->val.intval,right_buffer,sizeof(right_buffer)); break;
  case EXPRDBL: val2->val.strval=double_to_pstring(val2->val.dblval,right_buffer,sizeof(right_buffer)); break;
  default: _tmplpro_expnum_debug(*val2, "FATAL:internal expr string error. please report\n");
  }
  val2->type = EXPRPSTR;
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
  EXPR_int iretval=0;
  double dretval=0;
  EXPR_int offset=0;
  int sign=1;
  retval.type=EXPRINT;
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
      if (retval.type == EXPRINT) {
	retval.type = EXPRDBL;
	dretval=iretval;
	offset=1;
      } else {
	/* (*curposptr)--; ??? */
	expr_debug(state, "uninspected declimal point","");
	retval.val.dblval=dretval*sign;
	retval.type=EXPRDBL;
	return retval;
      }
    } else {
      offset*=10;
      if (retval.type == EXPRINT) {
	iretval=iretval*10+c-'0';
      } else {
	dretval=dretval*10+c-'0';
      }
    }
    c = *(++(*curposptr));
  }
  if (retval.type == EXPRINT) {
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
  if (val1->type == EXPRPSTR) {
    char* curpos=val1->val.strval.begin;
    *val1=exp_read_number (state, &curpos, val1->val.strval.endnext);
  }
}

static
void 
expr_to_bool (struct tmplpro_state* state, struct exprval* val1)
{
  if (val1->type == EXPRPSTR) {
    char* begin=val1->val.strval.begin;
    char* end=val1->val.strval.endnext;
    char* curpos=begin;
    *val1=exp_read_number (state, &curpos, end);
    if (val1->type == EXPRINT) {
      if (val1->val.intval || (curpos == end)) return;
      else val1->val.intval=1; /* strings are true in perl */
    }
    else
      if (val1->type == EXPRDBL) {
	if (val1->val.dblval || (curpos == end)) return;
	else val1->val.dblval=1.0; /* strings are true in perl */
      }
  }
}

void 
_tmplpro_expnum_debug (struct exprval val, char* msg) 
{
  tmpl_log(NULL,TMPL_LOG_DEBUG,"--> debug %s:type %c ",msg,val.type);
  if (val.type == EXPRINT)
    tmpl_log(NULL,TMPL_LOG_DEBUG,"ival=%" EXPR_modifier "d\n",val.val.intval);
  else if (val.type == EXPRDBL)
    tmpl_log(NULL,TMPL_LOG_DEBUG,"dval=%f\n",val.val.dblval);
  else if (val.type == EXPRPSTR) {
    tmpl_log(NULL,TMPL_LOG_DEBUG,"sval=%.*s\n",(int)(val.val.strval.endnext-val.val.strval.begin),val.val.strval.begin);
  } else {
    tmpl_log(NULL,TMPL_LOG_DEBUG,"ival=%" EXPR_modifier "d\n",val.val.intval);
  }
}

/* 
 * checks if it is stringval and unescape it if the flag is set
 */
static
struct exprval
expr_unescape_pstring_val(struct tmplpro_state* state, struct exprval val) {
  if (val.type == EXPRPSTR && val.strval_escape_flag) {
    // TODO: write the code!
    char* curpos = val.val.strval.begin;
    char* endnext= val.val.strval.endnext;
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
    val.val.strval.begin = buf;
    val.val.strval.endnext = bufpos;
    val.strval_escape_flag = 0;
    return val;
  } else return val;
}
