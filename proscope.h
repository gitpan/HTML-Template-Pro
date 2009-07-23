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
  int  loop_count;	/* total number of loops or negative value if unknown */
  /* objects are wrapper-specific so pointer is void */
  ABSTRACT_ARRAY* loops_AV;	/* pointer to array of loop's dictionaries */
  ABSTRACT_MAP*   param_HV;	/* pointer to dictionary of current loop   */
};

/*
static int curScopeLevel(struct scope_stack*);
static struct ProLoopState* getCurrentScope(struct scope_stack*);
static struct ProLoopState* getScope(struct scope_stack*, int depth);
static void popScope(struct scope_stack* scopestack);
static void pushScope2(struct scope_stack*, int loop_count, void *loops_AV);
static void Scope_init_root(struct scope_stack*, void* param_HV);
static void Scope_free(struct scope_stack* scopestack);
*/
#endif /* _PROSCOPE_H */
