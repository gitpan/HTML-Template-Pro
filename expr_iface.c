/* 
 * File: expr_iface.c
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Sat Apr 15 21:15:24 2006
 */

#include <string.h>
#include "tmplpro.h"
#include "exprval.h"
#include "pparam.h"

API_IMPL 
void 
APICALL
tmplpro_set_expr_as_int64 (struct tmplpro_param* param,EXPR_int64 ival) {
  struct exprval* p = &(param->userfunc_call);
  p->type=EXPR_TYPE_INT;
  p->val.intval=ival;
}

API_IMPL 
void 
APICALL
tmplpro_set_expr_as_double (struct tmplpro_param* param,double dval) {
  struct exprval* p = &(param->userfunc_call);
  p->type=EXPR_TYPE_DBL;
  p->val.dblval=dval;
}

API_IMPL 
void 
APICALL
tmplpro_set_expr_as_string (struct tmplpro_param* param,char* sval) {
  struct exprval* p = &(param->userfunc_call);
  p->type=EXPR_TYPE_PSTR;
  p->val.strval.begin=sval;
  p->val.strval.endnext=sval;
  if (NULL!=sval) p->val.strval.endnext+=strlen(sval);
}

API_IMPL 
void 
APICALL
tmplpro_set_expr_as_pstring (struct tmplpro_param* param,PSTRING pval) {
  struct exprval* p = &(param->userfunc_call);
  p->type=EXPR_TYPE_PSTR;
  p->val.strval=pval;
}

API_IMPL 
int
APICALL
tmplpro_get_expr_type (struct tmplpro_param* param) {
  return (int) param->userfunc_call.type;
}

API_IMPL 
EXPR_int64 
APICALL
tmplpro_get_expr_as_int64 (struct tmplpro_param* param) {
  return param->userfunc_call.val.intval;
}

API_IMPL 
double
APICALL
tmplpro_get_expr_as_double (struct tmplpro_param* param) {
  return param->userfunc_call.val.dblval;
}

API_IMPL 
char*
APICALL
tmplpro_get_expr_as_string (struct tmplpro_param* param) {
  PSTRING pval = param->userfunc_call.val.strval;
  *(pval.endnext)=0;
  return pval.begin;
}

API_IMPL 
PSTRING
APICALL
tmplpro_get_expr_as_pstring (struct tmplpro_param* param) {
  return param->userfunc_call.val.strval;
}
