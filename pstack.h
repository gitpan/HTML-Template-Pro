#ifndef _PSTACK_H
#define _PSTACK_H	1

struct pstack_entry {
  int tag;
  int value;
  int vcontext;
  char* position;
};

static 
void pstack_init(struct pstack* pstack);
static 
void pstack_free(struct pstack* pstack);
static 
void pstack_push(struct pstack* pstack, struct pstack_entry);
static 
struct pstack_entry pstack_pop(struct pstack* pstack);
static 
struct pstack_entry pstack_top(struct pstack* pstack);
static 
struct pstack_entry* pstack_head(struct pstack* pstack);
static 
int pstack_notempty(struct pstack* pstack);
static 
int pstack_selftest(struct pstack* pstack);

#endif /* pstack.h */

/*
 * Local Variables:
 * mode: c 
 * coding: cp1251
 * End: 
 */
