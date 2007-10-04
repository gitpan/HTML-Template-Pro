#include <stdlib.h>
#include <assert.h>

#define INIT_ENV_DEPTH 256

static 
void pstack_init(struct pstack* pstack) {
  pstack->depth=INIT_ENV_DEPTH;
  pstack->pos=-1;
  pstack->entry=(struct pstack_entry*) malloc (pstack->depth * sizeof(struct pstack_entry));
}

static 
void pstack_free(struct pstack* pstack) {
  pstack->depth=-1;
  pstack->pos=-1;
  free(pstack->entry);
}

static 
int pstack_notempty(struct pstack* pstack) {
  return pstack->pos>=0;
}

static 
void pstack_debug(struct pstack* pstack, struct pstack_entry item) {
 tmpl_log(NULL,TMPL_LOG_DEBUG,"vcontext = %d value=%d\n",item.vcontext,item.value);
}

static 
struct pstack_entry pstack_top(struct pstack* pstack) {
  return *(pstack->entry+pstack->pos);
}

static 
struct pstack_entry* pstack_head(struct pstack* pstack) {
  return pstack->entry+pstack->pos;
}

static 
struct pstack_entry pstack_pop(struct pstack* pstack) {
  if (pstack->pos<0) {
    tmpl_log(NULL,TMPL_LOG_ERROR,"stack underflow:tags stack is empty\n");
    pstack->pos=0;
    if (pstack->depth<0) {
      tmpl_log(NULL,TMPL_LOG_ERROR,"FATAL:stack error:tags stack is uninitialized\n");
      pstack_init(pstack);
    }
  }
  return *(pstack->entry+ pstack->pos--);
}

static 
void pstack_push(struct pstack* pstack, struct pstack_entry item) {
  /* overflow check */
  if (++(pstack->pos)>=pstack->depth) {
    if (pstack->depth<INIT_ENV_DEPTH) pstack->depth=INIT_ENV_DEPTH;
    pstack->depth*=2;
    pstack->entry=(struct pstack_entry*) realloc (pstack->entry, pstack->depth * sizeof(struct pstack_entry));
  }
  *(pstack->entry+pstack->pos)=item;
}

  /* stack tests */
static 
int pstack_selftest(struct pstack* pstack) {
  struct pstack_entry azz;
  assert(! pstack_notempty(pstack));
  azz.vcontext=9;
  pstack_push(pstack, azz);
  azz.vcontext=6;
  assert(pstack_notempty(pstack));
  azz=pstack_pop(pstack);
  assert(! pstack_notempty(pstack));
  assert(azz.vcontext==9);
  return 0;
}

/* 
 * Local Variables:
 * mode: c 
 * End: 
 */
