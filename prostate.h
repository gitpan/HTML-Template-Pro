#include "pbuffer.h"

struct tagstack {
  struct tagstack_entry* entry;
  int pos;
  int depth;
};

struct tmplpro_param;

typedef int boolval;

struct tmplpro_state {
  boolval  is_visible;
  char* top;
  char* next_to_end;
  char* last_processed_pos;
  char* cur_pos;
  struct tmplpro_param* param;
  /* current tag */
  int   tag;
  boolval  is_tag_closed;
  boolval  is_tag_commented;
  boolval  is_expr;
  char* tag_start; 

/* internal buffers */
  /* main string buffer */
  pbuffer str_buffer;
  /* tag stack */
  struct tagstack tag_stack;

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

extern TMPLPRO_LOCAL void _tmpl_log_state (struct tmplpro_state *state, int level);

/* 
 * Local Variables:
 * mode: c 
 * End: 
 */
