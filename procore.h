#define ERR_PRO_CANT_OPEN_FILE 1

#include "pstring.h"
#include "tmpllog.h"
#include "proscope.h"

/* MS VC++ support;
 * thanks to Viacheslav Sheveliov <slavash@aha.ru>
 */
#ifdef _MSC_VER
#  define snprintf _snprintf
#endif

typedef int flag;

struct tmplpro_param;

/* -------- Expr extension------------ */
typedef char exprtype;
#define EXPRINT 'i'
#define EXPRDBL 'd'
#define EXPRPSTR 'p'

#ifdef HAS_INT64_T
typedef int64_t EXPR_int;
#define EXPR_modifier "ll"
#else
#ifdef HAS_LONG_LONG
typedef long long EXPR_int;
#define EXPR_modifier "ll"
#else
typedef int EXPR_int;
#define EXPR_modifier ""
#endif
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
void _tmplpro_expnum_debug (struct exprval val, char* msg);
/* ------- end Expr extension -------- */

typedef void    (*writerfunc) (char* begin, char* endnext);

typedef int (*init_loop_func) (struct scope_stack*, PSTRING name);
typedef int (*next_loop_func) (struct ProLoopState*);

typedef ABSTRACT_VALUE* (*get_ABSTRACT_VALUE_func) (ABSTRACT_MAP*, PSTRING name);
typedef PSTRING (*ABSTRACT_VALUE2PSTRING_func) (ABSTRACT_VALUE*);
typedef int (*is_ABSTRACT_VALUE_TRUE_func) (ABSTRACT_VALUE*);

/* TODO: implement this code inside H::T::Pro */
typedef const char* (*find_file_func) (const char* filename, const char* prevfilename);

/* optional; we can use wrapper to load file and apply its filters before running itself */
/* note that this function should allocate region 1 byte nore than the file size	 */
typedef PSTRING (*load_file_func) (const char* filename);
typedef int     (*unload_file_func) (PSTRING memarea);

/* those are needed for EXPR= extension */
typedef void    (*init_expr_arglist_func) (struct tmplpro_param* param);
typedef void    (*push_expr_arglist_func) (struct tmplpro_param* param, struct exprval);
typedef struct exprval (*call_expr_userfnc_func) (struct tmplpro_param* param, void* extfunc);
typedef void*   (*is_expr_userfnc_func) (struct tmplpro_param* param, PSTRING name);

#define HTML_TEMPLATE_OPT_ESCAPE_NO   0
#define HTML_TEMPLATE_OPT_ESCAPE_HTML 1
#define HTML_TEMPLATE_OPT_ESCAPE_URL  2
#define HTML_TEMPLATE_OPT_ESCAPE_JS   3

struct tmplpro_param {
  int global_vars;
  int max_includes;
  int debug;
  flag no_includes;
  flag case_sensitive;
  flag loop_context_vars;
  flag strict;
  /* filters --- indicates whether to use 
   * external file loader hook specified as LoadFileFuncPtr. 
   * Set it to 1 if you want to preprocess file with filters
   * before they'll be processed by exec_tmpl */
  flag filters;
  int default_escape; /* one of HTML_TEMPLATE_OPT_ESCAPE_* */
  const char* filename; /* template file */
  PSTRING scalarref; /* memory area */
  /* currently used in Perl code */
  /* int search_path_on_include; */
  /* still unsupported  */
  flag die_on_bad_params;
  /* int vanguard_compatibility_mode; */
  /* hooks to perl or other container */
  /* HTML::Template hooks */
  writerfunc WriterFuncPtr;
  get_ABSTRACT_VALUE_func getAbstractValFuncPtr;
  ABSTRACT_VALUE2PSTRING_func abstractVal2pstringFuncPtr;
  /* user-supplied --- optional; we use it for full emulation of perl quirks */
  is_ABSTRACT_VALUE_TRUE_func isAbstractValTrueFuncPtr;
  init_loop_func InitLoopFuncPtr;
  next_loop_func NextLoopFuncPtr;
  find_file_func FindFileFuncPtr;
  load_file_func LoadFileFuncPtr;
unload_file_func UnloadFileFuncPtr;
  /* HTML::Template::Expr hooks */
  init_expr_arglist_func InitExprArglistFuncPtr;
  /**
     important note: 
     PushExprArglistFuncPtr should always copy the supplied pstring arg
     as it could point to a temporary location.
   */
  push_expr_arglist_func PushExprArglistFuncPtr;
  call_expr_userfnc_func CallExprUserfncFuncPtr;
  is_expr_userfnc_func   IsExprUserfncFuncPtr;
  void* ExprFuncHash;
  void* ExprFuncArglist;
  /* private */
  int cur_includes; /* internal counter of include depth */
  const char* selfpath; /* file that has included this file, or empty string */
  /* hack until param and state will be merged */
  ABSTRACT_MAP* rootHV;


};

int tmplpro_exec_tmpl (const char* filename, struct tmplpro_param* ProParams);
int tmplpro_exec_tmpl_in_memory (PSTRING memarea, struct tmplpro_param* param);


/* private : to hide */
struct tmplpro_state;

PSTRING get_variable_value (struct tmplpro_state *state, PSTRING name);


/* 
 * Local Variables:
 * mode: c 
 * End: 
 */
