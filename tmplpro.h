/* 
 * File: tmplpro.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Sat Apr 15 21:15:24 2006
 *
 * $Id$
 */



#ifndef _TMPLPRO_H
#define _TMPLPRO_H	1

#include "procore.h"
#include "proparam.h"

void tmplpro_procore_init();
void tmplpro_procore_done();

/* 
 * Constructor and destructor of tmplpro_param. 
 * Note that struct tmplpro_param is not part of the interface
 * and is subject to change without notice.
 */
struct tmplpro_param* tmplpro_param_init();
void tmplpro_param_free(struct tmplpro_param*);

int tmplpro_exec_tmpl (struct tmplpro_param*);

#endif /* tmplpro.h */
