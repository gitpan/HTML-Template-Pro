/* -*- c -*- 
 * File: proscope.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu May 26 15:13:10 2005
 *
 * $Id$
 */

struct ProLoopState {
  int  loop; 		/* current loop */
  int  maxloop;		/* max loop number */
  /* objects are wrapper-specific so pointer is void */
  void *loops_AV;	/* pointer to array of loop's dictionaries */
  void *param_HV;	/* pointer to dictionary of current loop   */
};

extern struct ProLoopState* CurrentScope;

extern int _ScopeLevel;
#define CurScopeLevel() _ScopeLevel

struct ProLoopState* GetScope(int depth);
void PopScope();
/* maxloop = number of loops - 1 in * loops_AV */
void PushScope2(int maxloop, void *loops_AV);
void SetRootScope(void* param_HV);
void Scope_init();
void Scope_free();

