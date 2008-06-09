#include "pbuffer.h"

struct tagstack {
  struct tagstack_entry* entry;
  int pos;
  int depth;
};

struct tmplpro_param;

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

/* internal buffers */
  /* main string buffer */
  pbuffer str_buffer;
  /* tag stack */
  struct tagstack tag_stack;

  /* moved from state; are passed to include */
  /* variable scope (nested loops) */
  /* struct scope_stack var_scope_stack;*/
  /* expr state variables */
  /* expr string buffer; used to unescape pstring arg */
  /* TODO: test and move it too (otherwise it will cause bugs in included exprs?) */
  pbuffer expr_pusharg_buffer;

  char* expr_curpos;
  PSTRING expr;
/* 
 * is_expect_quote_like allows recognization of quotelike.
 * if not is_expect_quote_like we look only for 'str' and, possibly, "str"
 * if is_expect_quote_like we also look for /str/.
 */
  int is_expect_quote_like;
};

extern void _tmpl_log_state (struct tmplpro_state *state, int level);

/* 
 * Local Variables:
 * mode: c 
 * End: 
 */
