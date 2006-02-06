#define ERR_PRO_CANT_OPEN_FILE 1

#include "pstring.h"
#include "tmpllog.h"

/* MS VC++ support;
 * thanks to Viacheslav Sheveliov <slavash@aha.ru>
 */
#ifdef _MSC_VER
#  define snprintf _snprintf
#endif

typedef int flag;

struct tmplpro_state;
struct tmplpro_param;

extern void tmpl_log_state (struct tmplpro_state *state, int level);

/* -------- Expr extension------------ */
typedef char exprtype;
#define EXPRINT 'i'
#define EXPRDBL 'd'
#define EXPRPSTR 'p'

struct exprval {
  exprtype type;
  union uval {
    int  intval; 		/* integer */
    double dblval;		/* double */
    PSTRING strval;
  } val;
};
void expnum_debug (struct exprval val, char* msg);
/* ------- end Expr extension -------- */

typedef void    (*writerfunc) (char* begin, char* endnext);
typedef PSTRING (*get_variable_func) (struct tmplpro_param* param, PSTRING name);
typedef int (*is_variable_true_func) (struct tmplpro_param* param, PSTRING name);
typedef int (*init_loop_func) (struct tmplpro_state* state, PSTRING name);
typedef int (*next_loop_func) (struct tmplpro_state* state);
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
  const char* filename; /* template file */
  PSTRING scalarref; /* memory area */
  /* currently used in Perl code */
  /* int search_path_on_include; */
  /* still unsupported  */
  int die_on_bad_params;
  /* int vanguard_compatibility_mode; */
  /* hooks to perl or other container */
  /* HTML::Template hooks */
  writerfunc WriterFuncPtr;
  get_variable_func GetVarFuncPtr;
  is_variable_true_func IsVarTrueFuncPtr;
  init_loop_func InitLoopFuncPtr;
  next_loop_func NextLoopFuncPtr;
  find_file_func FindFileFuncPtr;
  load_file_func LoadFileFuncPtr;
unload_file_func UnloadFileFuncPtr;
  /* HTML::Template::Expr hooks */
  init_expr_arglist_func InitExprArglistFuncPtr;
  push_expr_arglist_func PushExprArglistFuncPtr;
  call_expr_userfnc_func CallExprUserfncFuncPtr;
  is_expr_userfnc_func   IsExprUserfncFuncPtr;
  void* ExprFuncHash;
  void* ExprFuncArglist;
  /* private */
  int cur_includes; /* internal counter of include depth */
  const char* selfpath; /* file that included this file or empty string */
};

struct tmplpro_state {
  flag  is_visible;
  char* top;
  char* next_to_end;
  char* last_processed_pos;
  char* cur_pos;
  struct tmplpro_param* param;
  /* current tag */
  int   tag;
  flag  is_tag_closed;
  flag  is_tag_commented;
  flag  is_expr;
  char* tag_start; 
};

int exec_tmpl (const char* filename, struct tmplpro_param* ProParams);
int exec_tmpl_from_memory (PSTRING memarea, struct tmplpro_param* param);

void procore_init();
void procore_done();

/* internal initialization of struct tmplpro_param */
void param_init(struct tmplpro_param* param);


/* private : to hide */
extern void _tmpl_log_state (struct tmplpro_state *state, int level);

/* 
 * Local Variables:
 * mode: c 
 * End: 
 */
