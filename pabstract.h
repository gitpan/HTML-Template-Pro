/* -*- c -*- 
 * File: proabstract.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu Jul  9 19:26:20 2009
 */

#ifndef _PROABSTRACT_H
#define _PROABSTRACT_H	1

#include "pstring.h"
#include "exprval.h"

struct tmplpro_param;

typedef void ABSTRACT_WRITER;
typedef void ABSTRACT_FINDFILE;
typedef void ABSTRACT_FILTER;
typedef void ABSTRACT_CALLER;

typedef void ABSTRACT_ARRAY;
typedef void ABSTRACT_MAP;
typedef void ABSTRACT_VALUE;

typedef void ABSTRACT_FUNCMAP;
typedef void ABSTRACT_ARGLIST;
typedef void ABSTRACT_USERFUNC;

typedef void (*writer_functype) (ABSTRACT_WRITER*,const char* begin, const char* endnext);

typedef ABSTRACT_VALUE* (*get_ABSTRACT_VALUE_functype) (ABSTRACT_MAP*, PSTRING name);
typedef PSTRING (*ABSTRACT_VALUE2PSTRING_functype) (ABSTRACT_VALUE*);
/* optional */
typedef int (*is_ABSTRACT_VALUE_true_functype) (ABSTRACT_VALUE*);

typedef ABSTRACT_ARRAY* (*ABSTRACT_VALUE2ABSTRACT_ARRAY_functype) (ABSTRACT_VALUE*);
typedef int (*get_ABSTRACT_ARRAY_length_functype) (ABSTRACT_ARRAY*);
typedef ABSTRACT_MAP* (*get_ABSTRACT_MAP_functype) (ABSTRACT_ARRAY*,int);

/* optional */
typedef void (*end_loop_functype) (ABSTRACT_MAP*, int newlevel);
typedef void (*select_loop_scope_functype) (ABSTRACT_MAP*, int level);


/* TODO: implement this code inside H::T::Pro */
typedef const char* (*find_file_functype) (ABSTRACT_FINDFILE*, const char* filename, const char* prevfilename);

/* optional; we can use wrapper to load file and apply its filters before running itself */
/* note that this function should allocate region 1 byte nore than the file size	 */
typedef PSTRING (*load_file_functype) (ABSTRACT_FILTER*, const char* filename);
typedef int     (*unload_file_functype) (ABSTRACT_FILTER*, PSTRING memarea);

#define HTML_TEMPLATE_OPT_ESCAPE_NO   0
#define HTML_TEMPLATE_OPT_ESCAPE_HTML 1
#define HTML_TEMPLATE_OPT_ESCAPE_URL  2
#define HTML_TEMPLATE_OPT_ESCAPE_JS   3

/* -------- Expr extension------------ */

/* those are needed for EXPR= extension */
typedef ABSTRACT_USERFUNC* (*is_expr_userfnc_functype) (ABSTRACT_FUNCMAP*, PSTRING name);
typedef ABSTRACT_ARGLIST*  (*init_expr_arglist_functype) (ABSTRACT_CALLER*);
typedef void (*push_expr_arglist_functype) (ABSTRACT_ARGLIST*, struct tmplpro_param* param);
typedef void (*free_expr_arglist_functype) (ABSTRACT_ARGLIST*);
typedef void (*call_expr_userfnc_functype) (ABSTRACT_CALLER*, ABSTRACT_ARGLIST*, ABSTRACT_USERFUNC*, struct tmplpro_param* param);

/* ------- end Expr extension -------- */

#endif /* _PROABSTRACT_H */

/*
 *  Local Variables:
 *  mode: c
 *  End:
 */
