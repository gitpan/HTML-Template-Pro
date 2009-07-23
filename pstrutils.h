/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id$
 */

#ifndef _PSTRUTILS_H
#define _PSTRUTILS_H	1

#include "pbuffer.h"

/* 
 * use 1 internal pbuffer; be careful
 * todo: move pbuffer to state - like thread context
 */
PSTRING lowercase_pstring (pbuffer*, PSTRING);
int is_pstring_true (PSTRING s);

#endif /* pstrutils.h */
