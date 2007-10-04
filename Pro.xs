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

#include "tmplpro.h"

typedef PerlIO *        OutputStream;


static 
PerlIO* OutputFile;
static 
SV* OutputString;
static
AV* PerlFilteredTmpls=NULL;

static 
SV* PerlSelfHTMLTemplatePro;
static 
int debuglevel=0;

/* endnext points on next character to end of interval as in c++ */
static void write_chars_to_file (char* begin, char* endnext) {
  register char* i;
  for (i=begin; i<endnext; i++) {
    PerlIO_putc(OutputFile,*i);
  }
}

/* endnext points on next to end character of the interval */
static void write_chars_to_string (char* begin, char* endnext) {
  sv_catpvn(OutputString, begin, endnext-begin);
}

static
ABSTRACT_VALUE* get_ABSTRACT_VALUE_impl (ABSTRACT_MAP* ptr_HV, PSTRING name) {
  return hv_fetch((HV*) ptr_HV,name.begin, name.endnext-name.begin, 0);
}

static
PSTRING ABSTRACT_VALUE2PSTRING_impl (ABSTRACT_VALUE* valptr) {
  STRLEN len=0;
  PSTRING retval={NULL,NULL};
  SV* SVval;
  if (valptr==NULL) return retval;
  SVval = *((SV**) valptr);
  if (SvOK(SVval)) {
    retval.begin=SvPV(SVval, len);
    retval.endnext=retval.begin+len;
  }
  return retval;
}

static
int is_ABSTRACT_VALUE_TRUE_impl (ABSTRACT_VALUE* valptr) {
  SV* SVval;
  if (valptr==NULL) return 0;
  SVval = *((SV**) valptr);
  if (SvROK(SVval)) {
    /* arrayptr : in HTML::Template, true if len(array)>0 */
    if ((SvTYPE(SvRV(SVval)) == SVt_PVAV)
	&& (av_len((AV *)SvRV(SVval))<0)) {
      return 0;
    } else return 1;
  }
  if(SvTRUE(SVval)) return 1;
  return 0;
}

static 
int perl_next_loop (struct ProLoopState* currentScope) {
  SV** arrayvalptr;
  arrayvalptr=av_fetch((AV*)currentScope->loops_AV, currentScope->loop, 0);
  if ((arrayvalptr==NULL) || (!SvROK(*arrayvalptr)) || (SvTYPE(SvRV(*arrayvalptr)) != SVt_PVHV)) {
    warn("PARAM:LOOP:next_loop:hash pointer was expected but not found");
    return 0;
  } else {
    currentScope->param_HV=(HV *)SvRV(*arrayvalptr);
    return 1;
  }
}

static 
int perl_init_loop (struct scope_stack* variable_scope, PSTRING name) {
  AV* loops_AV;
  int maxloop;
  struct ProLoopState* currentScope = getCurrentScope(variable_scope);
  SV** hashvalptr=hv_fetch((HV*)currentScope->param_HV,name.begin, name.endnext-name.begin, 0);
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
    pushScope2(variable_scope, maxloop, loops_AV);
    return 1;
  }
}

static 
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
  perlretval=POPs;
  /* any memory leaks??? */  
  if (SvOK(perlretval)) {
    filepath = SvPV(perlretval, len);
    SvREFCNT_inc(perlretval);
  } else {
    filepath = NULL;
  }
  PUTBACK ;
  FREETMPS ;
  LEAVE ;
  return filepath;
}

static 
PSTRING load_file (const char* filepath) {
  dSP ;
  int count ;
  STRLEN len;
  PSTRING tmpl;
  SV* templateptr;
  SV* perlretval = sv_2mortal(newSVpv(filepath,0));
  ENTER ;
  SAVETMPS;
  PUSHMARK(SP) ;
  XPUSHs(PerlSelfHTMLTemplatePro);
  XPUSHs(perlretval);
  PUTBACK ;
  count = call_pv("_load_template", G_SCALAR);
  SPAGAIN ;
  if (count != 1) croak("Big troublen") ;
  templateptr=POPs;
  /* any memory leaks??? */  
  if (SvOK(templateptr) && SvROK(templateptr)) {
    tmpl.begin = SvPV(SvRV(templateptr), len);
    tmpl.endnext=tmpl.begin+len;
    av_push(PerlFilteredTmpls,templateptr);
    SvREFCNT_inc(SvRV(templateptr));
  } else {
    croak("Big trouble! _load_template internal fatal error\n") ;
  }
  PUTBACK ;
  FREETMPS ;
  LEAVE ;
  return tmpl;
}

static
int unload_file(PSTRING memarea) {
  /*
   * scalar is already mortal so we don't need dereference it
   * SvREFCNT_dec(SvRV(av_pop(PerlFilteredTmpls)));
   */
  av_pop(PerlFilteredTmpls); 
  return 0;
}

static 
void* is_expr_userfnc (struct tmplpro_param* param, PSTRING name) {
  SV** hashvalptr=hv_fetch((HV *) param->ExprFuncHash, name.begin, name.endnext-name.begin, 0);
  return hashvalptr;
}

static 
void free_expr_arglist(struct tmplpro_param* param)
{
  if (NULL!=param->ExprFuncArglist) {
    av_undef((AV*) param->ExprFuncArglist);
  }
}

static 
void init_expr_arglist(struct tmplpro_param* param)
{
  free_expr_arglist(param);
  param->ExprFuncArglist=newAV();
}

static 
void push_expr_arglist(struct tmplpro_param* param,  struct exprval exprval)
{
  SV* val=NULL;
  switch (exprval.type) {
  case EXPRINT:  val=newSViv(exprval.val.intval);break;
  case EXPRDBL:  val=newSVnv(exprval.val.dblval);break;
  case EXPRPSTR: val=newSVpvn(exprval.val.strval.begin, exprval.val.strval.endnext-exprval.val.strval.begin);break;
  default: die ("FATAL INTERNAL ERROR:Unsupported type %d in exprval", exprval.type);
  }
  av_push ((AV*) param->ExprFuncArglist, val);
  if (debuglevel>6) _tmplpro_expnum_debug (exprval, "EXPR: arglist: pushed ");
}

static 
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
  /* retval.val.strval=(PSTRING) {empty,empty}; */
  retval.val.strval.begin=empty;
  retval.val.strval.endnext=empty;
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
	/* hack !!! */
	SvREFCNT_inc(svretval);
	/* non-portable */
	/* retval.val.strval=(PSTRING) {strval, strval +len}; */
	retval.val.strval.begin=strval;
	retval.val.strval.endnext=strval +len;
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
  if (debuglevel>6) _tmplpro_expnum_debug (retval, "EXPR: function call: returned ");
  return retval;
}

static 
int get_integer_from_hash(HV* TheHash, char* key) {
  SV** hashvalptr=hv_fetch(TheHash, key, strlen(key), 0);
  if (hashvalptr==NULL) return 0;
  return SvIV(*hashvalptr);
}

static 
PSTRING get_string_from_hash(HV* TheHash, char* key) {
  SV** hashvalptr=hv_fetch(TheHash, key, strlen(key), 0);
  STRLEN len=0;
  char * begin;
  PSTRING retval={NULL,NULL};
  if (hashvalptr==NULL) return retval;
  if (SvROK(*hashvalptr)) {
    /* if (SvTYPE(SvRV(*hashvalptr))!=SVt_PV) return (PSTRING) {NULL,NULL}; */
    begin=SvPV(SvRV(*hashvalptr),len);
  } else {
    if (! SvPOK(*hashvalptr)) return retval;
    begin=SvPV(*hashvalptr,len);
  }
  retval.begin=begin;
  retval.endnext=begin+len;
  return retval;
}

static 
struct tmplpro_param* process_tmplpro_options (SV* PerlSelfPtr) {
  HV* SelfHash;
  SV** hashvalptr;
  char* tmpstring;
  int default_escape=HTML_TEMPLATE_OPT_ESCAPE_NO;
  /* internal initialization */
  struct tmplpro_param* param=tmplpro_param_init();
  /*   setting initial hooks */
  param->WriterFuncPtr=&write_chars_to_string;
  param->getAbstractValFuncPtr=&get_ABSTRACT_VALUE_impl;
  param->abstractVal2pstringFuncPtr=&ABSTRACT_VALUE2PSTRING_impl;
  param->isAbstractValTrueFuncPtr=&is_ABSTRACT_VALUE_TRUE_impl;
  param->InitLoopFuncPtr=&perl_init_loop;
  param->NextLoopFuncPtr=&perl_next_loop;
  param->FindFileFuncPtr=&get_filepath;
  param->LoadFileFuncPtr=&load_file;
  param->UnloadFileFuncPtr=&unload_file;
  /*   setting initial Expr hooks */
  param->InitExprArglistFuncPtr=&init_expr_arglist;
  param->PushExprArglistFuncPtr=&push_expr_arglist;
  param->CallExprUserfncFuncPtr=&call_expr_userfnc;
  param->IsExprUserfncFuncPtr=&is_expr_userfnc;
  /* end setting initial hooks */

  /*   setting perl globals */
  PerlSelfHTMLTemplatePro=PerlSelfPtr;
  PerlFilteredTmpls=newAV();
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
  param->ExprFuncHash=(HV *) SvRV(*hashvalptr);
  /* end setting expr_func */

  /* setting param_map */
  hashvalptr=hv_fetch(SelfHash, "param_map", 9, 0); /* 9=strlen("param_map") */
  if (!hashvalptr || !SvROK(*hashvalptr) || (SvTYPE(SvRV(*hashvalptr)) != SVt_PVHV))
    die("FATAL:output:param_map not found");

  //setRootScope((HV *)SvRV(*hashvalptr));
  param->rootHV = (HV *)SvRV(*hashvalptr);

  /* end setting param_map */

  /* setting filter */
  hashvalptr=hv_fetch(SelfHash, "filter", 6, 0); /* 6=strlen("filter") */
  if (!hashvalptr || !SvROK(*hashvalptr) || (SvTYPE(SvRV(*hashvalptr)) != SVt_PVAV))
    die("FATAL:output:filter not found");
  if (av_len((AV*)SvRV(*hashvalptr))>=0) param->filters=1;
  /* end setting param_map */

  param->selfpath=NULL; /* we are not included by somthing */
  param->filename=get_string_from_hash(SelfHash,"filename").begin;
  param->scalarref=get_string_from_hash(SelfHash,"scalarref");
  param->max_includes=get_integer_from_hash(SelfHash,"max_includes");
  param->no_includes=get_integer_from_hash(SelfHash,"no_includes");
  /* search_path_on_include proccessed directly in perl code */
  /* param->search_path_on_include=get_integer_from_hash(SelfHash,"search_path_on_include"); */
  param->global_vars=get_integer_from_hash(SelfHash,"global_vars");
  param->debug=get_integer_from_hash(SelfHash,"debug");
  param->loop_context_vars=get_integer_from_hash(SelfHash,"loop_context_vars");
  param->case_sensitive=get_integer_from_hash(SelfHash,"case_sensitive");
  /* still unsupported */
  param->strict=get_integer_from_hash(SelfHash,"strict");
  param->die_on_bad_params=get_integer_from_hash(SelfHash,"die_on_bad_params");
  
  tmpstring=get_string_from_hash(SelfHash,"default_escape").begin;
  if (tmpstring && *tmpstring) {
    switch (*tmpstring) {
    case '1': case 'H': case 'h': 	/* HTML*/
      default_escape = HTML_TEMPLATE_OPT_ESCAPE_HTML;
      break;
    case 'U': case 'u': 		/* URL */
      default_escape = HTML_TEMPLATE_OPT_ESCAPE_URL;
      break;
    case 'J': case 'j':		/* JS  */
      default_escape = HTML_TEMPLATE_OPT_ESCAPE_JS;
      break;
    case '0': 
      default_escape = HTML_TEMPLATE_OPT_ESCAPE_NO;
      break;
    default:
      warn("unsupported value of default_escape=%s. Valid values are HTML, URL or JS.\n",tmpstring);
    }
    param->default_escape=default_escape;
  }
  return param;
}

MODULE = HTML::Template::Pro		PACKAGE = HTML::Template::Pro

void 
_init()
    CODE:
	tmplpro_procore_init();

void 
_done()
    CODE:
	tmplpro_procore_done();


int
exec_tmpl(selfoptions,possible_output)
	SV* selfoptions;
	OutputStream possible_output;
    CODE:
	struct tmplpro_param* proparam=process_tmplpro_options(selfoptions);
	if (possible_output == NULL){
	  warn("bad file descriptor. Use output=stdout\n");
	  OutputFile=PerlIO_stdout();
	} else {
	  OutputFile=possible_output;
	  if (debuglevel) warn("output=given descriptor\n");
	}
	proparam->WriterFuncPtr=&write_chars_to_file;
	if (proparam->filename!=NULL) {
	  RETVAL = tmplpro_exec_tmpl(proparam->filename,proparam);
	  tmplpro_param_free(proparam);
	} else if (proparam->scalarref.begin!=NULL) {
	  RETVAL = tmplpro_exec_tmpl_in_memory(proparam->scalarref,proparam);
	  tmplpro_param_free(proparam);
	} else {
	  tmplpro_param_free(proparam);
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
	struct tmplpro_param* proparam=process_tmplpro_options(selfoptions);
	if (debuglevel) warn("output=string\n");
	OutputString=newSV(256); /* 256 allocated bytes -- should be approx. filesize*/
	sv_setpvn(OutputString, "", 0);
	proparam->WriterFuncPtr=&write_chars_to_string;
	if (proparam->filename!=NULL) {
	  retstate = tmplpro_exec_tmpl(proparam->filename,proparam);
	  tmplpro_param_free(proparam);
	} else if (proparam->scalarref.begin!=NULL) {
	  retstate = tmplpro_exec_tmpl_in_memory(proparam->scalarref,proparam);
	  tmplpro_param_free(proparam);
	} else {
	  tmplpro_param_free(proparam);
	  die ("bad arguments: expected filename or scalarref");
	}
	RETVAL = OutputString;
    OUTPUT:
	RETVAL

