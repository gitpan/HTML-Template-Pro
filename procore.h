#define ERR_PRO_CANT_OPEN_FILE 1

#include "pstring.h"
#include "tmpllog.h"
#include "pabstract.h"

/* MS VC++ support;
 * thanks to Viacheslav Sheveliov <slavash@aha.ru>
 */
#ifdef _MSC_VER
#  define snprintf _snprintf
#endif

/* used in pparam, prostate */
typedef int flag;

struct tmplpro_param;

/* -------- Expr extension------------ */
typedef char exprtype;
#define EXPRINT 'i'
#define EXPRDBL 'd'
#define EXPRPSTR 'p'

#ifdef INT64_NAME
     typedef INT64_NAME EXPR_int;
#  ifdef _MSC_VER
#    define EXPR_modifier "I64"
#  else
#    define EXPR_modifier "ll"
#  endif
#else 
     typedef int EXPR_int;
#    define EXPR_modifier ""
#endif 

struct exprval {
  exprtype type;
  /* this flag indicates that supplied pstring should be unescaped */
  char strval_escape_flag;
  union uval {
    EXPR_int  intval; 		/* integer */
    double dblval;		/* double */
    PSTRING strval;
  } val;
};
/* ------- end Expr extension -------- */

typedef void    (*writer_functype) (char* begin, char* endnext);

typedef ABSTRACT_VALUE* (*get_ABSTRACT_VALUE_functype) (ABSTRACT_MAP*, PSTRING name);
typedef PSTRING (*ABSTRACT_VALUE2PSTRING_functype) (ABSTRACT_VALUE*);
typedef int (*is_ABSTRACT_VALUE_true_functype) (ABSTRACT_VALUE*);
typedef ABSTRACT_ARRAY* (*ABSTRACT_VALUE2ABSTRACT_ARRAY_functype) (ABSTRACT_VALUE*);
typedef int (*get_ABSTRACT_ARRAY_length_functype) (ABSTRACT_ARRAY*);
typedef ABSTRACT_MAP* (*get_ABSTRACT_MAP_functype) (ABSTRACT_ARRAY*,int);

/* TODO: implement this code inside H::T::Pro */
typedef const char* (*find_file_functype) (const char* filename, const char* prevfilename);

/* optional; we can use wrapper to load file and apply its filters before running itself */
/* note that this function should allocate region 1 byte nore than the file size	 */
typedef PSTRING (*load_file_functype) (const char* filename);
typedef int     (*unload_file_functype) (PSTRING memarea);

/* those are needed for EXPR= extension */
typedef ABSTRACT_USERFUNC* (*is_expr_userfnc_functype) (ABSTRACT_FUNCMAP*, PSTRING name);
typedef ABSTRACT_ARGLIST*  (*init_expr_arglist_functype) ();
typedef void    (*push_expr_arglist_functype) (ABSTRACT_ARGLIST*, struct exprval);
typedef void    (*free_expr_arglist_functype) (ABSTRACT_ARGLIST*);
typedef struct exprval (*call_expr_userfnc_functype) (ABSTRACT_ARGLIST*, ABSTRACT_USERFUNC*);

#define HTML_TEMPLATE_OPT_ESCAPE_NO   0
#define HTML_TEMPLATE_OPT_ESCAPE_HTML 1
#define HTML_TEMPLATE_OPT_ESCAPE_URL  2
#define HTML_TEMPLATE_OPT_ESCAPE_JS   3

#include "pparam.h"

int tmplpro_exec_tmpl_filename (struct tmplpro_param* ProParams,const char* filename);
int tmplpro_exec_tmpl_scalarref (struct tmplpro_param* ProParams, PSTRING memarea);

/* 
 * Local Variables:
 * mode: c 
 * End: 
 */
