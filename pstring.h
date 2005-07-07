/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id: pstring.h,v 1.3 2005/07/07 07:29:49 igor Exp $
 */

typedef struct pstring {
  char* begin;
  char* endnext;
} PSTRING;

PSTRING double_to_pstring (double);
PSTRING lowercase_pstring (PSTRING);
int is_pstring_true (PSTRING s);
int pstring_ge(PSTRING, PSTRING);
int pstring_le(PSTRING, PSTRING);
int pstring_ne(PSTRING, PSTRING);
int pstring_eq(PSTRING, PSTRING);
int pstring_gt(PSTRING, PSTRING);
int pstring_lt(PSTRING, PSTRING);

