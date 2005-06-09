/* -*- c -*- 
 * File: proscope.c
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu May 26 15:25:57 2005
 *
 * $Id: proscope.c,v 1.1 2005/06/02 09:58:49 igor Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "proscope.h"

int ScopeLevel=-1;
int ScopeMax=-1;

#define START_NUMBER_OF_NESTED_LOOPS 64

struct ProLoopState* Scope;

struct ProLoopState* CurrentScope;

void Scope_init() {
  ScopeMax=START_NUMBER_OF_NESTED_LOOPS;
  Scope=(struct ProLoopState*) malloc (ScopeMax * sizeof(struct ProLoopState));
}

void Scope_free() {
  free(Scope);
  ScopeMax=-1;
}

inline void set_CurrentScope () {
  CurrentScope=&(Scope[ScopeLevel]);
}

struct ProLoopState* GetScope(int depth) {
  return &(Scope[depth]);
}

void PopScope() {
  if (ScopeLevel>0) ScopeLevel--;
  else fprintf(stderr, "WARN:PopScope:internal error:scope is exhausted\n");
  set_CurrentScope();
}

void PushScope2(int maxloop, void *loops_AV) {
  ++ScopeLevel;
  if (ScopeLevel>ScopeMax) 
    {
      if (ScopeMax<START_NUMBER_OF_NESTED_LOOPS) ScopeMax=START_NUMBER_OF_NESTED_LOOPS;
      ScopeMax*=2;
      Scope=(struct ProLoopState*) realloc (Scope, ScopeMax * sizeof(struct ProLoopState));
    }
  set_CurrentScope();
  CurrentScope->loop=-1;
  CurrentScope->maxloop = maxloop;
  CurrentScope->loops_AV=loops_AV;
  CurrentScope->param_HV=0;
}

void SetRootScope(void* param_HV) {
  ScopeLevel=0;
  set_CurrentScope();
  CurrentScope->param_HV=param_HV;
}
