/* -*- c -*- 
 * File: proscope.c
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu May 26 15:25:57 2005
 *
 * $Id: proscope.c,v 1.3 2005/06/16 17:07:30 igor Exp $
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
  if (NULL==Scope) fprintf(stderr, "DIE:Scope_init:internal error:not enough memory\n");
  ScopeLevel=-1;
}

void Scope_free() {
  /* fprintf(stderr, "Scope_free done\n"); */
  free(Scope);
  ScopeMax=-1;
  ScopeLevel=-1;
}

inline void set_CurrentScope () {
  CurrentScope=Scope+ScopeLevel;
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
  if (ScopeMax<0) {
    
    fprintf(stderr, "WARN:PushScope:internal warning:why scope is empty?\n");
    Scope_init();
  }
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
  if (ScopeMax<0) {
    fprintf(stderr, "WARN:SetRootScope:internal warning:why scope is empty?\n");
    Scope_init();
  }
  ScopeLevel=0;
  set_CurrentScope();
  CurrentScope->param_HV=param_HV;
}
