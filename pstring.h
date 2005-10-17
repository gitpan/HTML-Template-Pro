/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id: pstring.h,v 1.8 2005/10/06 13:14:59 igor Exp $
 */

#ifndef _PSTRING_H
#define _PSTRING_H	1

typedef struct PSTRING {
  char* begin;
  char* endnext;
} PSTRING;

PSTRING lowercase_pstring (PSTRING);
int is_pstring_true (PSTRING s);

#endif /* pstring.h */
