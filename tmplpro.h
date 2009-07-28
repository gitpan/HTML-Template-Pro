/* 
 * File: tmplpro.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Sat Apr 15 21:15:24 2006
 *
 * $Id$
 */



#ifndef _TMPLPRO_H
#define _TMPLPRO_H	1

#include "pabidecl.h"
#include "pstring.h"
#include "pabstract.h"
#include "proparam.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * generic load/first use library and unload/last use library hooks.
 */
HTMLTMPLPRO_API void tmplpro_procore_init();
HTMLTMPLPRO_API void tmplpro_procore_done();

HTMLTMPLPRO_API const char* tmplpro_version(void);

/* 
 * Constructor and destructor of tmplpro_param. 
 * Note that struct tmplpro_param is not part of the interface
 * and is subject to change without notice.
 */
HTMLTMPLPRO_API struct tmplpro_param* tmplpro_param_init();
HTMLTMPLPRO_API void tmplpro_param_free(struct tmplpro_param*);

HTMLTMPLPRO_API int tmplpro_exec_tmpl (struct tmplpro_param*);

HTMLTMPLPRO_API void tmplpro_set_expr_as_int64 (struct tmplpro_param*,EXPR_int64);
HTMLTMPLPRO_API void tmplpro_set_expr_as_double (struct tmplpro_param*,double);
HTMLTMPLPRO_API void tmplpro_set_expr_as_string (struct tmplpro_param*,char*);
HTMLTMPLPRO_API void tmplpro_set_expr_as_pstring (struct tmplpro_param*,PSTRING);
HTMLTMPLPRO_API int      tmplpro_get_expr_type (struct tmplpro_param*);
HTMLTMPLPRO_API EXPR_int64 tmplpro_get_expr_as_int64 (struct tmplpro_param*);
HTMLTMPLPRO_API double   tmplpro_get_expr_as_double (struct tmplpro_param*);
HTMLTMPLPRO_API char*    tmplpro_get_expr_as_string (struct tmplpro_param*);
HTMLTMPLPRO_API PSTRING  tmplpro_get_expr_as_pstring (struct tmplpro_param*);

#ifdef __cplusplus
}
#endif

#endif /* tmplpro.h */
