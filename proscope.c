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

int _ScopeLevel=-1;
static int ScopeMax=-1;

#define START_NUMBER_OF_NESTED_LOOPS 64

static 
struct ProLoopState* Scope;

struct ProLoopState* CurrentScope;

void 
Scope_init() {
  ScopeMax=START_NUMBER_OF_NESTED_LOOPS;
  Scope=(struct ProLoopState*) malloc (ScopeMax * sizeof(struct ProLoopState));
  if (NULL==Scope) tmpl_log(NULL,TMPL_LOG_ERROR, "DIE:Scope_init:internal error:not enough memory\n");
  _ScopeLevel=-1;
}

void 
Scope_free() {
  free(Scope);
  ScopeMax=-1;
  _ScopeLevel=-1;
}

static
void 
set_CurrentScope () {
  CurrentScope=Scope+_ScopeLevel;
}

struct ProLoopState* 
GetScope(int depth) {
  return &(Scope[depth]);
}

void 
PopScope() {
  if (_ScopeLevel>0) _ScopeLevel--;
  else tmpl_log(NULL,TMPL_LOG_ERROR, "WARN:PopScope:internal error:scope is exhausted\n");
  set_CurrentScope();
}

void 
PushScope2(int maxloop, void *loops_AV) {
  if (ScopeMax<0) {
    
    tmpl_log(NULL,TMPL_LOG_ERROR, "WARN:PushScope:internal warning:why scope is empty?\n");
    Scope_init();
  }
  ++_ScopeLevel;
  if (_ScopeLevel>ScopeMax) 
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

void 
SetRootScope(void* param_HV) {
  if (ScopeMax<0) {
    tmpl_log(NULL,TMPL_LOG_ERROR, "WARN:SetRootScope:internal warning:why scope is empty?\n");
    Scope_init();
  }
  _ScopeLevel=0;
  set_CurrentScope();
  CurrentScope->param_HV=param_HV;
}
