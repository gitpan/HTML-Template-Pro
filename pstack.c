#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pstack.h"

#define MAX_ENV_DEPTH 256

struct pstack_entry* pstack;
int pstack_pos=-1;
int pstack_depth=MAX_ENV_DEPTH;

void pstack_init() {
  pstack_depth=MAX_ENV_DEPTH;
  pstack_pos=-1;
  pstack=(struct pstack_entry*) malloc (pstack_depth * sizeof(struct pstack_entry));
}

void pstack_free() {
  pstack_depth=-1;
  pstack_pos=-1;
  free(pstack);
}

int pstack_notempty() {
  return pstack_pos>=0;
}

void pstack_debug(struct pstack_entry item) {
 fprintf(stderr,"vcontext = %d value=%d\n",item.vcontext,item.value);
}

struct pstack_entry pstack_top() {
  return *(pstack+pstack_pos);
}

struct pstack_entry* pstack_head() {
  return pstack+pstack_pos;
}

struct pstack_entry pstack_pop() {
  if (pstack_pos<0) {
    fprintf(stderr,"stack underflow:tags stack is empty\n");
    pstack_pos=0; 
  }
  return *(pstack+ pstack_pos--);
}


void pstack_push(struct pstack_entry item) {
  /* overflow check */
  if (++pstack_pos>=pstack_depth) {
    if (pstack_depth<MAX_ENV_DEPTH) pstack_depth=MAX_ENV_DEPTH;
    pstack_depth*=2;
    pstack=(struct pstack_entry*) realloc (pstack, pstack_depth * sizeof(struct pstack_entry));
  }
  *(pstack+pstack_pos)=item;
}

  /* stack tests */
int pstack_selftest() {
  struct pstack_entry azz;
  assert(! pstack_notempty());
  azz.vcontext=9;
  pstack_push(azz);
  azz.vcontext=6;
  assert(pstack_notempty());
  azz=pstack_pop();
  assert(! pstack_notempty());
  assert(azz.vcontext==9);
  return 0;
}

/* 
 * Local Variables:
 * mode: c 
 * End: 
 */
