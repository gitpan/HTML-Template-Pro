#define PERLIO_NOT_STDIO 0    /* For co-existence with stdio only */

#ifdef __cplusplus
extern "C" {
#endif
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#ifdef __cplusplus
}
#endif

#include <string.h>
#include <stdio.h>

#include "ppport.h"

#include "pbuffer.h"
#include "proscope.h"
#include "procore.h"

typedef PerlIO *        OutputStream;


PerlIO* OutputFile;
SV* OutputString;

SV* PerlSelfHTMLTemplatePro;
int debuglevel=0;

/* endnext points on next character to end of interval as in c++ */
void write_chars_to_file (char* begin, char* endnext) {
  register char* i;
  for (i=begin; i<endnext; i++) {
    PerlIO_putc(OutputFile,*i);
  }
}

/* endnext points on next character to end of interval as in c++ */
void write_chars_to_string (char* begin, char* endnext) {
  sv_catpvn(OutputString, begin, endnext-begin);
}

SV ** walk_through_nested_loops (struct tmplpro_param* param, PSTRING name) {
  int PrevHash;
  SV** hashvalptr=hv_fetch((HV*) CurrentScope->param_HV,name.begin, name.endnext-name.begin, 0);
  if ((0==param->global_vars) || (hashvalptr)) return hashvalptr;
  PrevHash=CurScopeLevel()-1;
  while (PrevHash>=0) {
    hashvalptr=hv_fetch((HV*)(GetScope(PrevHash)->param_HV),name.begin, name.endnext-name.begin, 0);
    if (hashvalptr!=NULL) return hashvalptr;
    PrevHash--;
  }
  return NULL;
}

PSTRING get_perl_var_value (struct tmplpro_param* param, PSTRING name) {
  STRLEN len=0;
  char *value;
  SV** hashvalptr;
  /* walking on nested loops */
  hashvalptr=walk_through_nested_loops(param, name);
  if ((hashvalptr==NULL) || (! SvOK(*hashvalptr))) {
    return (PSTRING) {NULL,NULL};
  } else {
    value=SvPV(*hashvalptr, len);
    return (PSTRING) {value,value+len};
  }
}

int is_perl_value_true (struct tmplpro_param* param, PSTRING name) {
  SV** hashvalptr;
  hashvalptr=walk_through_nested_loops(param, name);
  if (hashvalptr==NULL) return 0;
  if (SvROK(*hashvalptr)) {
    /* arrayptr : in HTML::Template, true if len(array)>0 */
    if ((SvTYPE(SvRV(*hashvalptr)) == SVt_PVAV)
	&& (av_len((AV *)SvRV(*hashvalptr))<0)) {
      return 0;
    } else {
      return 1;
    }
  }
  if(SvTRUE(*hashvalptr)) return 1;
  return 0;
}

int perl_next_loop (struct tmplpro_state* state) {
  SV** arrayvalptr;
  arrayvalptr=av_fetch((AV*)CurrentScope->loops_AV, CurrentScope->loop, 0);
  if ((arrayvalptr==NULL) || (!SvROK(*arrayvalptr)) || (SvTYPE(SvRV(*arrayvalptr)) != SVt_PVHV)) {
    warn("PARAM:LOOP:next_loop:hash pointer was expected but not found");
    return 0;
  } else {
    CurrentScope->param_HV=(HV *)SvRV(*arrayvalptr);
    return 1;
  }
}

int perl_init_loop (struct tmplpro_state* state, PSTRING name) {
  AV* loops_AV;
  int maxloop;
  SV** hashvalptr=hv_fetch((HV*)CurrentScope->param_HV,name.begin, name.endnext-name.begin, 0);
  if (hashvalptr==NULL) {
    return 0;
  } else {
    /* set loop array */
    if ((!SvROK(*hashvalptr)) || (SvTYPE(SvRV(*hashvalptr)) != SVt_PVAV))
      {
	warn("PARAM:LOOP:loop argument:array pointer was expected but not found");
	return 0;
      }
    loops_AV=(AV *)SvRV(*hashvalptr);
    maxloop=av_len(loops_AV);
    if (maxloop < 0) return 0; 
    PushScope2(maxloop, loops_AV);
    return 1;
  }
}

const char* get_filepath (const char* filename, const char* prevfilename) {
  dSP ;
  int count ;
  STRLEN len;
  char* filepath;
  SV* perlprevfile;
  SV* perlretval = sv_2mortal(newSVpv(filename,0));
  if (prevfilename) {
    perlprevfile=sv_2mortal(newSVpv(prevfilename,0));
  } else {
    perlprevfile=sv_2mortal(newSV(0));
  }
  ENTER ;
  SAVETMPS;
  PUSHMARK(SP) ;
  XPUSHs(PerlSelfHTMLTemplatePro);
  XPUSHs(perlretval);
  XPUSHs(perlprevfile);
  PUTBACK ;
  count = call_pv("_get_filepath", G_SCALAR);
  SPAGAIN ;
  if (count != 1) croak("Big troublen") ;
  POPs;
  PUTBACK ;
  FREETMPS ;
  LEAVE ;
  /* any memory leaks??? */  
  if (SvOK(perlretval)) {
    filepath = SvPV(perlretval, len);
  } else {
    filepath = NULL;
  }
  return filepath;
}

void* is_expr_userfnc (struct tmplpro_param* param, PSTRING name) {
  SV** hashvalptr=hv_fetch((HV *) param->ExprFuncHash, name.begin, name.endnext-name.begin, 0);
  /*fprintf(stderr, "is call: name=%s (%p)\n", name.begin, hashvalptr);*/
  return hashvalptr;
}

inline void free_expr_arglist(struct tmplpro_param* param)
{
  if (NULL!=param->ExprFuncArglist) {
    av_undef((AV*) param->ExprFuncArglist);
  }
}

void init_expr_arglist(struct tmplpro_param* param)
{
  free_expr_arglist(param);
  param->ExprFuncArglist=newAV();
}

void push_expr_arglist(struct tmplpro_param* param,  struct exprval exprval)
{
  SV* val=NULL;
  switch (exprval.type) {
  case EXPRINT:  val=newSViv(exprval.val.intval);break;
  case EXPRDBL:  val=newSVnv(exprval.val.dblval);break;
  case EXPRPSTR: val=newSVpv(exprval.val.strval.begin, exprval.val.strval.endnext-exprval.val.strval.begin);break;
  default: die ("FATAL INTERNAL ERROR:Unsupported type %d in exprval", exprval.type);
  }
  av_push ((AV*) param->ExprFuncArglist, val);
  if (debuglevel>6) expnum_debug (exprval, "EXPR: arglist: pushed ");
}

struct exprval call_expr_userfnc (struct tmplpro_param* param, void* hashvalptr) {
  dSP ;
  char* empty="";
  char* strval;
  SV ** arrval;
  SV * svretval;
  I32 i;
  I32 numretval;
  I32 arrlen=av_len((AV *) param->ExprFuncArglist);
  struct exprval retval = {EXPRPSTR};
  retval.val.strval=(PSTRING) {empty,empty};
  if (hashvalptr==NULL) {
    die ("FATAL INTERNAL ERROR:Call_EXPR:function called but not exists");
    return retval;
  } else if (! SvROK(*((SV**) hashvalptr)) || (SvTYPE(SvRV(*((SV**) hashvalptr))) != SVt_PVCV)) {
    die ("FATAL INTERNAL ERROR:Call_EXPR:not a function reference");
    return retval;
  }
  
  ENTER ;
  SAVETMPS ;
  
  PUSHMARK(SP) ;
  for (i=0;i<=arrlen;i++) {
    arrval=av_fetch((AV *) param->ExprFuncArglist,i,0);
    if (arrval) XPUSHs(*arrval);
    else warn("INTERNAL: call: strange arrval");
  }
  PUTBACK ;
  numretval=call_sv(*((SV**) hashvalptr), G_SCALAR);
  SPAGAIN ;
  if (numretval) {
    svretval=POPs;
    if (SvOK(svretval)) {
      if (SvIOK(svretval)) {
	retval.type=EXPRINT;
	retval.val.intval=SvIV(svretval);
      } else if (SvNOK(svretval)) {
	retval.type=EXPRDBL;
	retval.val.dblval=SvNV(svretval);
      } else {
	STRLEN len=0;
	retval.type=EXPRPSTR;
	strval =SvPV(svretval, len);
	// hack !!!
	SvREFCNT_inc(svretval);
	retval.val.strval=(PSTRING) {strval, strval +len};
      }
    } else {
      warn ("user defined function returned undef");
    }
  } else {
    warn ("user defined function returned nothing");
  }

  FREETMPS ;
  LEAVE ;

  free_expr_arglist(param);
  if (debuglevel>6) expnum_debug (retval, "EXPR: function call: returned ");
  return retval;
}

int get_integer_from_hash(HV* TheHash, char* key) {
  SV** hashvalptr=hv_fetch(TheHash, key, strlen(key), 0);
  if (hashvalptr==NULL) return 0;
  return SvIV(*hashvalptr);
}

PSTRING get_string_from_hash(HV* TheHash, char* key) {
  SV** hashvalptr=hv_fetch(TheHash, key, strlen(key), 0);
  STRLEN len=0;
  char * begin;
  if (hashvalptr==NULL) return (PSTRING) {NULL,NULL};
  if (SvROK(*hashvalptr)) {
    /* if (SvTYPE(SvRV(*hashvalptr))!=SVt_PV) return (PSTRING) {NULL,NULL}; */
    begin=SvPV(SvRV(*hashvalptr),len);
  } else {
    if (! SvPOK(*hashvalptr)) return (PSTRING) {NULL,NULL};
    begin=SvPV(*hashvalptr,len);
  }
  return (PSTRING) {begin, begin+len};
}

struct tmplpro_param process_tmplpro_options (SV* PerlSelfPtr) {
  struct tmplpro_param param;
  HV* SelfHash;
  SV** hashvalptr;

  /* internal initialization */
  param_init(&param);
  /*   setting initial hooks */
  param.WriterFuncPtr=&write_chars_to_string;
  param.GetVarFuncPtr=&get_perl_var_value;
  param.IsVarTrueFuncPtr=&is_perl_value_true;
  param.InitLoopFuncPtr=&perl_init_loop;
  param.NextLoopFuncPtr=&perl_next_loop;
  param.FindFileFuncPtr=&get_filepath;
  param.LoadFileFuncPtr=NULL;
  param.UnloadFileFuncPtr=NULL;
  /*   setting initial Expr hooks */
  param.InitExprArglistFuncPtr=&init_expr_arglist;
  param.PushExprArglistFuncPtr=&push_expr_arglist;
  param.CallExprUserfncFuncPtr=&call_expr_userfnc;
  param.IsExprUserfncFuncPtr=&is_expr_userfnc;
  /* end setting initial hooks */

  /*   setting perl globals */
  PerlSelfHTMLTemplatePro=PerlSelfPtr;
  /*  end setting perl globals */

  if ((!SvROK(PerlSelfPtr)) || (SvTYPE(SvRV(PerlSelfPtr)) != SVt_PVHV))
    {
      die("FATAL:SELF:hash pointer was expected but not found");
    }
  SelfHash=(HV *)SvRV(PerlSelfPtr);
  
  /* setting expr_func */
  hashvalptr=hv_fetch(SelfHash, "expr_func", 9, 0); /* 9=strlen("expr_func") */
  if (!hashvalptr || !SvROK(*hashvalptr) || (SvTYPE(SvRV(*hashvalptr)) != SVt_PVHV))
    die("FATAL:output:EXPR user functions not found");
  param.ExprFuncHash=(HV *) SvRV(*hashvalptr);
  /* end setting expr_func */

  /* setting param_map */
  hashvalptr=hv_fetch(SelfHash, "param_map", 9, 0); /* 9=strlen("param_map") */
  if (!hashvalptr || !SvROK(*hashvalptr) || (SvTYPE(SvRV(*hashvalptr)) != SVt_PVHV))
    die("FATAL:output:param_map not found");
  SetRootScope((HV *)SvRV(*hashvalptr));
  /* end setting param_map */

  param.selfpath=NULL; /* we are not included by somthing */
  param.filename=get_string_from_hash(SelfHash,"filename").begin;
  param.scalarref=get_string_from_hash(SelfHash,"scalarref");
  param.max_includes=get_integer_from_hash(SelfHash,"max_includes");
  param.no_includes=get_integer_from_hash(SelfHash,"no_includes");
  /* search_path_on_include proccessed directly in perl code */
  /* param.search_path_on_include=get_integer_from_hash(SelfHash,"search_path_on_include"); */
  param.global_vars=get_integer_from_hash(SelfHash,"global_vars");
  param.debug=get_integer_from_hash(SelfHash,"debug");
  param.loop_context_vars=get_integer_from_hash(SelfHash,"loop_context_vars");
  param.case_sensitive=get_integer_from_hash(SelfHash,"case_sensitive");
  /* still unsupported */
  param.strict=get_integer_from_hash(SelfHash,"strict");
  param.die_on_bad_params=get_integer_from_hash(SelfHash,"die_on_bad_params");
  return param;
}

MODULE = HTML::Template::Pro		PACKAGE = HTML::Template::Pro

void 
_init()
    CODE:
	procore_init(); 

void 
_done()
    CODE:
	procore_done(); 


int
exec_tmpl(selfoptions,possible_output)
	SV* selfoptions;
	OutputStream possible_output;
    CODE:
	struct tmplpro_param proparam=process_tmplpro_options(selfoptions);
	if (possible_output == NULL){
	  warn("bad file descriptor. Use output=stdout\n");
	  OutputFile=PerlIO_stdout();
	} else {
	  OutputFile=possible_output;
	  if (debuglevel) warn("output=given descriptor\n");
	}
	proparam.WriterFuncPtr=&write_chars_to_file;
	if (proparam.filename!=NULL) {
	  RETVAL = exec_tmpl(proparam.filename,&proparam);
	} else if (proparam.scalarref.begin!=NULL) {
	  RETVAL = exec_tmpl_from_memory(proparam.scalarref,&proparam);
	} else {
	  die ("bad arguments: expected filename or scalarref");
	}
    OUTPUT:
	RETVAL



SV*
exec_tmpl_string(selfoptions)
	SV* selfoptions;
    CODE:
	int retstate;
	/* TODO: */
	/* 1) estimate string as 2*filesize */
	/* 2) make it mortal ? auto...  */
	struct tmplpro_param proparam=process_tmplpro_options(selfoptions);
	if (debuglevel) warn("output=string\n");
	OutputString=newSV(256); /* 256 allocated bytes -- should be approx. filesize*/
	sv_setpvn(OutputString, "", 0);
	proparam.WriterFuncPtr=&write_chars_to_string;
	if (proparam.filename!=NULL) {
	  retstate = exec_tmpl(proparam.filename,&proparam);
	} else if (proparam.scalarref.begin!=NULL) {
	  retstate = exec_tmpl_from_memory(proparam.scalarref,&proparam);
	} else {
	  die ("bad arguments: expected filename or scalarref");
	}
	RETVAL = OutputString;
    OUTPUT:
	RETVAL

