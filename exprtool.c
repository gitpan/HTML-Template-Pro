/* -*- c -*- 
 * File: exprtool.c
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Mon Jul 25 15:29:17 2005
 *
 * $Id: exprtool.c,v 1.10 2005/07/26 16:51:09 igor Exp $
 */

#include "exprtool.h"
#include <ctype.h> /* for yylex alnum */
#include <stdio.h> /* for printf */

#define EXPR_CHECK_NUMBER(ptr)   switch (ptr->type) { \
  case EXPRINT: case EXPRDBL: break; \
  case EXPRPSTR: expr_to_num(ptr); break; \
  default: expnum_debug(*ptr, "FATAL:internal exprtype error. please report\n"); \
  ptr->type = EXPRINT; \
}

/* 
 * for sprintf %f
 * internal expr buffers for conversion int/double --> string 
 */
char left_buffer[40]; 
char right_buffer[40];

exprtype expr_to_int_or_dbl (struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_NUMBER(val1);
  EXPR_CHECK_NUMBER(val2);
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

void expr_to_dbl (struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_NUMBER(val1);
  EXPR_CHECK_NUMBER(val2);
  if (val1->type == EXPRINT) {
    val1->type=EXPRDBL;
    val1->val.dblval=val1->val.intval;
  }
  if (val2->type == EXPRINT) {
    val1->type=EXPRDBL;
    val2->val.dblval=val2->val.intval;
  }
}

void expr_to_dbl1 (struct exprval* val1) {
  EXPR_CHECK_NUMBER(val1);
  if (val1->type == EXPRINT) {
    val1->type=EXPRDBL;
    val1->val.dblval=val1->val.intval;
  }
}

void expr_to_int (struct exprval* val1, struct exprval* val2) {
  EXPR_CHECK_NUMBER(val1);
  EXPR_CHECK_NUMBER(val2);
  if (val1->type == EXPRDBL) {
    val1->type=EXPRINT;
    val1->val.intval=val1->val.dblval;
  }
  if (val2->type == EXPRDBL) {
    val2->type=EXPRINT;
    val2->val.intval=val2->val.dblval;
  }
}

void expr_to_str (struct exprval* val1, struct exprval* val2) {
  switch (val1->type) {
  case EXPRPSTR: break;
  case EXPRINT: val1->val.strval=int_to_pstring(val1->val.intval,left_buffer); break;
  case EXPRDBL: val1->val.strval=double_to_pstring(val1->val.dblval,left_buffer); break;
  default: expnum_debug(*val1, "FATAL:internal expr string error. please report\n");
  }
  val1->type = EXPRPSTR;

  switch (val2->type) {
  case EXPRPSTR: break;
  case EXPRINT: val2->val.strval=int_to_pstring(val2->val.intval,right_buffer); break;
  case EXPRDBL: val2->val.strval=double_to_pstring(val2->val.dblval,right_buffer); break;
  default: expnum_debug(*val2, "FATAL:internal expr string error. please report\n");
  }
  val2->type = EXPRPSTR;
}

inline int is_float_lex (char c)
{
  return (c == '.' || isdigit (c));
}

struct exprval exp_read_number (char* *curposptr, char* endchars) {
  char c = **curposptr;
  struct exprval retval;
  int iretval=0;
  double dretval=0;
  int offset=0;
  retval.type=EXPRINT;
  retval.val.intval=0;
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
	expr_debug("uninspected declimal point","");
	retval.val.dblval=dretval;
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
    retval.val.intval=iretval;
  } else {
    if (offset) dretval/=offset;
    retval.val.dblval=dretval;
  }
  return retval;
}

void expr_to_num (struct exprval* val1)
{
  if (val1->type == EXPRPSTR) {
    char* curpos=val1->val.strval.begin;
    *val1=exp_read_number (&curpos, val1->val.strval.endnext);
  }
}

void expnum_debug (struct exprval val, char* msg) 
{
  if (val.type == EXPRINT)
    fprintf(stderr,"--> debug %s:type %c ival=%d\n",msg,val.type,val.val.intval);
  else if (val.type == EXPRDBL)
    fprintf(stderr,"--> debug %s:type %c dval=%f\n",msg,val.type,val.val.dblval);
  else if (val.type == EXPRPSTR) {
    char* curpos=val.val.strval.begin;
    fprintf(stderr,"--> debug %s:type %c sval=",msg,val.type);
    while (curpos<val.val.strval.endnext) fputc(*curpos++,stderr);
    fputc('\n',stderr);
  } else {
    fprintf(stderr,"--> debug %s:type %c ival=%d\n",msg,val.type,val.val.intval);
  }
}
