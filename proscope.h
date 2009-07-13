/* -*- c -*- 
 * File: proscope.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu May 26 15:13:10 2005
 *
 * $Id$
 */

#ifndef _PROSCOPE_H
#define _PROSCOPE_H	1

#include "pabstract.h"

struct scope_stack {
  int level;
  int max;
  int _init_count;
  struct ProLoopState* root;
};

struct ProLoopState {
  int  loop; 		/* current loop */
  int  maxloop;		/* max loop number */
  /* objects are wrapper-specific so pointer is void */
  ABSTRACT_ARRAY* loops_AV;	/* pointer to array of loop's dictionaries */
  ABSTRACT_MAP*   param_HV;	/* pointer to dictionary of current loop   */
};

int curScopeLevel(struct scope_stack*);
struct ProLoopState* getCurrentScope(struct scope_stack*);
struct ProLoopState* getScope(struct scope_stack*, int depth);
void popScope(struct scope_stack* scopestack);
/* maxloop = number of loops - 1 in * loops_AV */
void pushScope2(struct scope_stack*, int maxloop, void *loops_AV);
void Scope_init_root(struct scope_stack*, void* param_HV);
void Scope_free(struct scope_stack* scopestack);

#endif /* _PROSCOPE_H */
