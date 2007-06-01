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

void tmplpro_procore_init();
void tmplpro_procore_done();

/* 
 * Constructor and destructor of tmplpro_param. Note that different versions
 * of tmplpro may have different sizeof(struct tmplpro_param), but even if 
 * they are linked dynamically, they are still binary compatible 
 * if you are using tmplpro_param_init and tmplpro_param_free.
 */
struct tmplpro_param* tmplpro_param_init();
void tmplpro_param_free(struct tmplpro_param*);


#endif /* tmplpro.h */
