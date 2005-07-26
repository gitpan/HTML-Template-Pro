/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id: pstring.h,v 1.6 2005/07/26 16:31:37 igor Exp $
 */

#ifndef _PSTRING_H
#define _PSTRING_H	1

typedef struct pstring {
  char* begin;
  char* endnext;
} PSTRING;

PSTRING double_to_pstring (double, char* buf);
PSTRING int_to_pstring (int,char* buf);
PSTRING lowercase_pstring (PSTRING);
int is_pstring_true (PSTRING s);
int pstring_ge(PSTRING, PSTRING);
int pstring_le(PSTRING, PSTRING);
int pstring_ne(PSTRING, PSTRING);
int pstring_eq(PSTRING, PSTRING);
int pstring_gt(PSTRING, PSTRING);
int pstring_lt(PSTRING, PSTRING);

#endif /* pstring.h */
