/* -*- c -*- 
 * File: proscope.c
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu May 26 15:25:57 2005
 *
 * $Id$
 */

#include <stdlib.h>
#include "proscope.h"
#include "tmpllog.h"

#define START_NUMBER_OF_NESTED_LOOPS 64

static
void 
Scope_init(struct scope_stack* scopestack) {
    scopestack->max=START_NUMBER_OF_NESTED_LOOPS;
    scopestack->root=(struct ProLoopState*) malloc ((scopestack->max) * sizeof(struct ProLoopState));
    if (NULL==scopestack->root) tmpl_log(NULL,TMPL_LOG_ERROR, "DIE:Scope_init:internal error:not enough memory\n");
    scopestack->level=-1;
}

void 
Scope_free(struct scope_stack* scopestack) {
  if (--scopestack->_init_count<=0) {
    free(scopestack->root);
    scopestack->max=-1;
    scopestack->level=-1;
    scopestack->_init_count=0;
  }
}

int curScopeLevel(struct scope_stack* scopestack) {
  return scopestack->level;
}

struct ProLoopState* getCurrentScope(struct scope_stack* scopestack) {
  return scopestack->root+scopestack->level;
}

struct ProLoopState* 
getScope(struct scope_stack* scopestack, int depth) {
  return &(scopestack->root)[depth];
}

void 
popScope(struct scope_stack* scopestack) {
  if (scopestack->level>0) scopestack->level--;
  else tmpl_log(NULL,TMPL_LOG_ERROR, "WARN:PopScope:internal error:scope is exhausted\n");
}

void 
pushScope2(struct scope_stack* scopestack, int maxloop, void *loops_AV) {
  struct ProLoopState* CurrentScope;
  if (scopestack->max<0) {
    
    tmpl_log(NULL,TMPL_LOG_ERROR, "WARN:PushScope:internal warning:why scope is empty?\n");
    Scope_init(scopestack);
  }
  ++scopestack->level;
  if (scopestack->level>scopestack->max) 
    {
      if (scopestack->max<START_NUMBER_OF_NESTED_LOOPS) scopestack->max=START_NUMBER_OF_NESTED_LOOPS;
      scopestack->max*=2;
      scopestack->root=(struct ProLoopState*) realloc (scopestack->root, (scopestack->max) * sizeof(struct ProLoopState));
    }
  CurrentScope=scopestack->root+scopestack->level;
  CurrentScope->loop=-1;
  CurrentScope->maxloop = maxloop;
  CurrentScope->loops_AV=loops_AV;
  CurrentScope->param_HV=0;
}

void 
Scope_init_root(struct scope_stack* scopestack, void* param_HV) {
  if (scopestack->_init_count++==0) {
    Scope_init(scopestack);
    scopestack->level=0;
    scopestack->root->param_HV=param_HV;
  }
}
