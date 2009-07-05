/* -*- c -*- 
 * File: provalue.c
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Sun Jul  5 17:38:20 2009
 */

#include "procore.h"
#include<string.h>

ABSTRACT_VALUE* walk_through_nested_loops (struct tmplpro_param *param, PSTRING name) {
  int PrevHash;
  struct ProLoopState* currentScope;
  ABSTRACT_VALUE* valptr;
  /* Shigeki Morimoto path_like_variable_scope extension */
  if (param->path_like_variable_scope) {
    if(*(name.begin) == '/' || strncmp(name.begin, "../", 3) == 0){
      PSTRING tmp_name;
      int GoalHash;
      if(*(name.begin) == '/'){
	tmp_name.begin   = name.begin+1; // skip '/'
	tmp_name.endnext = name.endnext;
	GoalHash = 0;
      }else{
	tmp_name.begin   = name.begin;
	tmp_name.endnext = name.endnext;
	GoalHash = curScopeLevel(&param->var_scope_stack);
	while(strncmp(tmp_name.begin, "../", 3) == 0){
	  tmp_name.begin   = tmp_name.begin + 3; // skip '../'
	  GoalHash --;
	}
      }
      valptr = param->getAbstractValFuncPtr(getScope(&param->var_scope_stack, GoalHash)->param_HV, tmp_name);
      return valptr;
    }
  }
  /* end Shigeki Morimoto path_like_variable_scope extension */

  currentScope = getCurrentScope(&param->var_scope_stack);
  valptr= param->getAbstractValFuncPtr(currentScope->param_HV, name);
  if ((0==param->global_vars) || (valptr)) return valptr;
  PrevHash=curScopeLevel(&param->var_scope_stack)-1;
  while (PrevHash>=0) {
    valptr=param->getAbstractValFuncPtr(getScope(&param->var_scope_stack, PrevHash)->param_HV,name);
    if (valptr!=NULL) return valptr;
    PrevHash--;
  }
  return NULL;
}

/*
 *  Local Variables:
 *  mode: c
 *  End:
 */
