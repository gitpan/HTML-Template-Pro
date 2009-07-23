/* 
 * File: tmplpro.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Sat Apr 15 21:15:24 2006
 *
 * $Id$
 */



#ifndef _TMPLPRO_H
#define _TMPLPRO_H	1

#include "pstring.h"
#include "pabstract.h"
#include "proparam.h"

/*
 * generic load/first use library and unload/last use library hooks.
 */
void tmplpro_procore_init();
void tmplpro_procore_done();

const char* tmplpro_version(void);

/* 
 * Constructor and destructor of tmplpro_param. 
 * Note that struct tmplpro_param is not part of the interface
 * and is subject to change without notice.
 */
struct tmplpro_param* tmplpro_param_init();
void tmplpro_param_free(struct tmplpro_param*);

int tmplpro_exec_tmpl (struct tmplpro_param*);

void tmplpro_set_expr_as_int64 (struct tmplpro_param*,EXPR_int64);
void tmplpro_set_expr_as_double (struct tmplpro_param*,double);
void tmplpro_set_expr_as_string (struct tmplpro_param*,char*);
void tmplpro_set_expr_as_pstring (struct tmplpro_param*,PSTRING);
int      tmplpro_get_expr_type (struct tmplpro_param*);
EXPR_int64 tmplpro_get_expr_as_int64 (struct tmplpro_param*);
double   tmplpro_get_expr_as_double (struct tmplpro_param*);
char*    tmplpro_get_expr_as_string (struct tmplpro_param*);
PSTRING  tmplpro_get_expr_as_pstring (struct tmplpro_param*);

#endif /* tmplpro.h */
