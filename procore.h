#define ERR_PRO_CANT_OPEN_FILE 1

#include "pstring.h"
#include "exprtool.h"

typedef int flag;

struct tmplpro_state;
struct tmplpro_param;

typedef void (*writerfunc) (char* begin, char* endnext);
typedef PSTRING (*get_variable_func) (struct tmplpro_param* param, PSTRING name);
typedef int (*is_variable_true_func) (struct tmplpro_param* param, PSTRING name);
typedef int (*init_loop_func) (struct tmplpro_state* state, PSTRING name);
typedef int (*next_loop_func) (struct tmplpro_state* state);
typedef const char* (*find_file_func) (const char* filename, const char* prevfilename);
typedef void (*init_expr_arglist_func) (struct tmplpro_param* param);
typedef void (*push_expr_arglist_func) (struct tmplpro_param* param, struct exprval);
typedef struct exprval (*call_expr_userfnc_func) (struct tmplpro_param* param, void* extfunc);
typedef void* (*is_expr_userfnc_func) (struct tmplpro_param* param, PSTRING name);

struct tmplpro_param {
  int global_vars;
  int no_includes;
  int max_includes;
  int debug;
  int case_sensitive;
  int loop_context_vars;
  int strict;
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

void tag_warn  (struct tmplpro_state *state, char* message, PSTRING msg2);

/* 
 * Local Variables:
 * mode: c 
 * End: 
 */
