/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id$
 */

#ifndef _PSTRING_H
#define _PSTRING_H	1

typedef struct PSTRING {
  char* begin;
  char* endnext;
} PSTRING;

#endif /* pstring.h */
