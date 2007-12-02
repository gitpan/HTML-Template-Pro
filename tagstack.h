#ifndef _TAGSTACK_H
#define _TAGSTACK_H	1

struct tagstack_entry {
  int tag;
  int value;
  int vcontext;
  char* position;
};

static 
void tagstack_init(struct tagstack* tagstack);
static 
void tagstack_free(struct tagstack* tagstack);
static 
void tagstack_push(struct tagstack* tagstack, struct tagstack_entry);
static 
struct tagstack_entry tagstack_pop(struct tagstack* tagstack);
static 
struct tagstack_entry tagstack_top(struct tagstack* tagstack);
static 
struct tagstack_entry* tagstack_head(struct tagstack* tagstack);
static 
int tagstack_notempty(struct tagstack* tagstack);
static 
int tagstack_selftest(struct tagstack* tagstack);

#endif /* tagstack.h */

/*
 * Local Variables:
 * mode: c 
 * coding: cp1251
 * End: 
 */
