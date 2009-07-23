/* 
 * File: expr_iface.c
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Sat Apr 15 21:15:24 2006
 */

#include <string.h>
#include "pstring.h"
#include "pabstract.h"
#include "exprval.h"
#include "pparam.h"

void tmplpro_set_expr_as_int64 (struct tmplpro_param* param,EXPR_int64 ival) {
  struct exprval* p = &(param->userfunc_call);
  p->type=EXPR_TYPE_INT;
  p->val.intval=ival;
}
void tmplpro_set_expr_as_double (struct tmplpro_param* param,double dval) {
  struct exprval* p = &(param->userfunc_call);
  p->type=EXPR_TYPE_DBL;
  p->val.dblval=dval;
}
void tmplpro_set_expr_as_string (struct tmplpro_param* param,char* sval) {
  struct exprval* p = &(param->userfunc_call);
  p->type=EXPR_TYPE_PSTR;
  p->val.strval.begin=sval;
  p->val.strval.endnext=sval;
  if (NULL!=sval) p->val.strval.endnext+=strlen(sval);
}
void tmplpro_set_expr_as_pstring (struct tmplpro_param* param,PSTRING pval) {
  struct exprval* p = &(param->userfunc_call);
  p->type=EXPR_TYPE_PSTR;
  p->val.strval=pval;
}

int      tmplpro_get_expr_type (struct tmplpro_param* param) {
  return (int) param->userfunc_call.type;
}
EXPR_int64 tmplpro_get_expr_as_int64 (struct tmplpro_param* param) {
  return param->userfunc_call.val.intval;
}
double   tmplpro_get_expr_as_double (struct tmplpro_param* param) {
  return param->userfunc_call.val.dblval;
}
char*    tmplpro_get_expr_as_string (struct tmplpro_param* param) {
  PSTRING pval = param->userfunc_call.val.strval;
  *(pval.endnext)=0;
  return pval.begin;
}
PSTRING  tmplpro_get_expr_as_pstring (struct tmplpro_param* param) {
  return param->userfunc_call.val.strval;
}
