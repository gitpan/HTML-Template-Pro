/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id: pstring.h,v 1.8 2005/10/06 13:14:59 igor Exp $
 */

#ifndef _EXPRPSTR_H
#define _EXPRPSTR_H	1

static
PSTRING double_to_pstring (double, char* buf);
static
PSTRING int_to_pstring (int,char* buf);
static
int pstring_ge(PSTRING, PSTRING);
static
int pstring_le(PSTRING, PSTRING);
static
int pstring_ne(PSTRING, PSTRING);
static
int pstring_eq(PSTRING, PSTRING);
static
int pstring_gt(PSTRING, PSTRING);
static
int pstring_lt(PSTRING, PSTRING);

static
int re_like(PSTRING, PSTRING);
static
int re_notlike(PSTRING, PSTRING);

#endif /* exprpstr.h */
