/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id$
 */

#ifndef _PSTRING_H
#define _PSTRING_H	1

#include "pbuffer.h"

typedef struct PSTRING {
  char* begin;
  char* endnext;
} PSTRING;

/* 
 * use 1 internal pbuffer; be careful
 * todo: move pbuffer to state - like thread context
 */
PSTRING lowercase_pstring (pbuffer*, PSTRING);
void lowercase_pstring_inplace (PSTRING s);
int is_pstring_true (PSTRING s);

#endif /* pstring.h */
