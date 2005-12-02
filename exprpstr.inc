/* -*- c -*- 
 * File: pstring.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Fri Jul  1 20:11:51 2005
 *
 * $Id: exprpstr.inc,v 1.2 2005/11/22 20:55:34 igor Exp $
 */

#include <stdio.h>
#include <string.h>
#include "pstring.h"
#include "tmpllog.h"

PSTRING double_to_pstring (double number, char buffer[], size_t bufsize) {
  size_t len=0;
  size_t tmplen=0;
  snprintf(buffer,bufsize,"%f",number);
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

PSTRING int_to_pstring (int number, char buffer[], size_t bufsize) {
  size_t len=0;
  snprintf(buffer, bufsize,"%d",number);
  len=strlen(buffer);
  return (PSTRING) {buffer, buffer+len};
}



int pstring_ge(PSTRING a, PSTRING b) {
  char* in_a=a.begin;
  char* in_b=b.begin;
  while (in_a<a.endnext && in_b < b.endnext && *in_a++==*in_b++);
  if ((in_a==a.endnext && in_b==b.endnext) || *(--in_a) >= *(--in_b) ) return 1; else return 0;
}

int pstring_le(PSTRING a, PSTRING b) {
  char* in_a=a.begin;
  char* in_b=b.begin;
  while (in_a<a.endnext && in_b < b.endnext && *in_a++==*in_b++);
  if ((in_a==a.endnext && in_b==b.endnext) || *(--in_a) <= *(--in_b) ) return 1; else return 0;
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

int re_notlike(PSTRING a, PSTRING b) {
  return ! re_like(a,b);
}

#ifndef HAVE_PCRE
int re_like(PSTRING a, PSTRING b) {
    tmpl_log(NULL,TMPL_LOG_ERROR," (sorry, Stanislav Yadykin extension is disabled at compile time) \n");
  return 0;
}
#else
#include <pcre.h>
int re_like(PSTRING a, PSTRING b) {
  pcre* re;
  int ovector[30];
  int rc, erroffset;
  const char* error;
  char* subject=a.begin;
  int subject_length=(int)(a.endnext-a.begin);
  char* pattern=(char*)malloc(b.endnext-b.begin);
  strncpy(pattern, b.begin, (b.endnext-b.begin));
  *(pattern+(b.endnext-b.begin))=0;
  re = pcre_compile(pattern, 0, &error, &erroffset, NULL); // default character set
  free(pattern);
  if (re==NULL) {
    tmpl_log(NULL,TMPL_LOG_ERROR, "PCRE compilation failed at offset %d: %s\n",
      erroffset, error);
    return 0;
  }
  rc=pcre_exec(re, NULL, subject, subject_length, 0, 0, ovector, 30);
  return (rc<0)?0:1;
}
#endif
