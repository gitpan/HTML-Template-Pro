#include "pbuffer.h"

struct pstack {
  struct pstack_entry* entry;
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
  struct pstack tag_stack;
  /* variable scope (nested loops) */
  struct scope_stack var_scope_stack;
};

extern void _tmpl_log_state (struct tmplpro_state *state, int level);

/* 
 * Local Variables:
 * mode: c 
 * End: 
 */
