/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id: pstring.c,v 1.7 2005/07/26 15:55:10 igor Exp $
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pbuffer.h"

typedef struct pstring {
  char* begin;
  char* endnext;
} PSTRING;

PSTRING double_to_pstring (double number, char buffer[]) {
  size_t len=0;
  size_t tmplen=0;
  sprintf(buffer,"%f",number);
  len=strlen(buffer);
  tmplen=len;
  /* removing trailing 0 as 2.00000... */
  while (buffer[tmplen-1]=='0' && tmplen-->0); 
  if (buffer[tmplen-1]=='.') {
    tmplen--;
    len=tmplen;
  }
  return (PSTRING) {buffer, buffer+len};
}

PSTRING int_to_pstring (int number, char buffer[]) {
  size_t len=0;
  sprintf(buffer,"%d",number);
  len=strlen(buffer);
  return (PSTRING) {buffer, buffer+len};
}


PSTRING lowercase_pstring (PSTRING pstring) {
  size_t size=pstring.endnext-pstring.begin;
  char* buf=pbuffer_resize(size+1);
  char* inbuf=buf;
  char* i=pstring.begin;
  while (i<pstring.endnext) {
    *inbuf++=tolower(*i++);
  }
  *inbuf=0;
  //if (debug>1 && size >0) fprintf(stderr," (lovercased to %s) ",buf);
  return (PSTRING) {buf, buf+size};
}

int is_pstring_true (PSTRING s) {
  if (s.begin == NULL || s.begin == s.endnext) return 0;
  if (1==s.endnext-s.begin) {
    if (*(s.begin)=='0') return 0; else return 1;
  } else if (3==s.endnext-s.begin) {
    if ('0'==*(s.begin) && '.'==*(s.begin+1) && '0'==*(s.begin+2)) return 0; else return 1;
  } else return 1;
}

int pstring_ge(PSTRING a, PSTRING b) {
  char* in_a=a.begin;
  char* in_b=b.begin;
  while (in_a<a.endnext && in_b < b.endnext && *in_a++==*in_b++);
  if (in_b==b.endnext || *(--in_a) > *(--in_b) ) return 1; else return 0;
}

int pstring_le(PSTRING a, PSTRING b) {
  char* in_a=a.begin;
  char* in_b=b.begin;
  while (in_a<a.endnext && in_b < b.endnext && *in_a++==*in_b++);
  if (in_a==a.endnext || *(--in_a) < *(--in_b) ) return 1; else return 0;
}

int pstring_ne(PSTRING a, PSTRING b) {
  char* in_a=a.begin;
  char* in_b=b.begin;
  while (in_a<a.endnext && in_b < b.endnext && *in_a++==*in_b++);
  if (in_a==a.endnext && in_b==b.endnext && *(--in_a) == *(--in_b)) return 0; else return 1;
}

int pstring_eq(PSTRING a, PSTRING b) {
  char* in_a=a.begin;
  char* in_b=b.begin;
  while (in_a<a.endnext && in_b < b.endnext && *in_a++==*in_b++);
  if (in_a==a.endnext && in_b==b.endnext && *(--in_a) == *(--in_b)) return 1; else return 0;
}

int pstring_gt(PSTRING a, PSTRING b) {
  char* in_a=a.begin;
  char* in_b=b.begin;
  while (in_a<a.endnext && in_b < b.endnext && *in_a++==*in_b++);
  if ((in_b==b.endnext && in_a!=a.endnext)
      || (*(--in_a) > *(--in_b)) ) return 1; else return 0;
}

int pstring_lt(PSTRING a, PSTRING b) {
  char* in_a=a.begin;
  char* in_b=b.begin;
  while (in_a<a.endnext && in_b < b.endnext && *in_a++==*in_b++);
  if ((in_b!=b.endnext && in_a==a.endnext)
      ||  *(--in_a) < *(--in_b) ) return 1; else return 0;
}

