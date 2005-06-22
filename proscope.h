/* -*- c -*- 
 * File: proscope.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu May 26 15:13:10 2005
 *
 * $Id: proscope.h,v 1.2 2005/06/16 17:07:30 igor Exp $
 */

struct ProLoopState {
  int  loop; 		/* current loop */
  int  maxloop;		/* max loop number */
  /* objects are wrapper-specific so pointer is void */
  void *loops_AV;	/* pointer to array of loop's dictionaries */
  void *param_HV;	/* pointer to dictionary of current loop   */
};

extern struct ProLoopState* CurrentScope;

extern int ScopeLevel;
#define CurScopeLevel() ScopeLevel

struct ProLoopState* GetScope(int depth);
void PopScope();
void PushScope2(int loops, void *loops_AV);
void SetRootScope(void* param_HV);
void Scope_init();
void Scope_free();

