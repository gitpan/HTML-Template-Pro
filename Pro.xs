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
AV* PoolForPerlVars=NULL;

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
int is_ABSTRACT_VALUE_true_impl (ABSTRACT_VALUE* valptr) {
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
ABSTRACT_ARRAY* ABSTRACT_VALUE2ABSTRACT_ARRAY_impl (ABSTRACT_VALUE* abstrvalptr) {
  SV** valptr = (SV**) abstrvalptr;
  if ((!SvROK(*valptr)) || (SvTYPE(SvRV(*valptr)) != SVt_PVAV)) return 0;
  return (ABSTRACT_ARRAY*) SvRV(*valptr);
}

static 
int get_ABSTRACT_ARRAY_length_impl (ABSTRACT_ARRAY* loops_AV) {
  return av_len((AV *)loops_AV)+1;
}

static 
ABSTRACT_MAP* get_ABSTRACT_MAP_impl (ABSTRACT_ARRAY* loops_AV, int loop) {
  SV** arrayvalptr;
  arrayvalptr=av_fetch((AV*)loops_AV, loop, 0);
  if ((arrayvalptr==NULL) || (!SvROK(*arrayvalptr)) || (SvTYPE(SvRV(*arrayvalptr)) != SVt_PVHV)) {
    return NULL;
  } else {
    return (ABSTRACT_MAP *)SvRV(*arrayvalptr);
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
    av_push(PoolForPerlVars,perlretval);
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
ABSTRACT_USERFUNC* is_expr_userfnc (ABSTRACT_FUNCMAP* FuncHash, PSTRING name) {
  SV** hashvalptr=hv_fetch((HV *) FuncHash, name.begin, name.endnext-name.begin, 0);
  return hashvalptr;
}

static 
void free_expr_arglist(ABSTRACT_ARGLIST* arglist)
{
  if (NULL!=arglist) {
    av_undef((AV*) arglist);
    SvREFCNT_dec(arglist);
  }
}

static 
ABSTRACT_ARGLIST* init_expr_arglist()
{
  return newAV();
}

static 
void push_expr_arglist(ABSTRACT_ARGLIST* arglist,  struct exprval exprval)
{
  SV* val=NULL;
  switch (exprval.type) {
  case EXPRINT:  val=newSViv(exprval.val.intval);break;
  case EXPRDBL:  val=newSVnv(exprval.val.dblval);break;
  case EXPRPSTR: val=newSVpvn(exprval.val.strval.begin, exprval.val.strval.endnext-exprval.val.strval.begin);break;
  default: die ("FATAL INTERNAL ERROR:Unsupported type %d in exprval", exprval.type);
  }
  av_push ((AV*) arglist, val);
}

static 
struct exprval call_expr_userfnc (ABSTRACT_ARGLIST* arglist, ABSTRACT_USERFUNC* hashvalptr) {
  dSP ;
  char* empty="";
  char* strval;
  SV ** arrval;
  SV * svretval;
  I32 i;
  I32 numretval;
  I32 arrlen=av_len((AV *) arglist);
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
    arrval=av_fetch((AV *) arglist,i,0);
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
	av_push(PoolForPerlVars,svretval);
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

  /* main arguments */
  PSTRING filename;
  PSTRING scalarref;

  /* internal initialization */
  struct tmplpro_param* param=tmplpro_param_init();

  /*   setting initial hooks */
  set_option_WriterFuncPtr(param, &write_chars_to_string);
  set_option_GetAbstractValFuncPtr(param, &get_ABSTRACT_VALUE_impl);
  set_option_AbstractVal2pstringFuncPtr(param, &ABSTRACT_VALUE2PSTRING_impl);
  set_option_AbstractVal2abstractArrayFuncPtr(param, &ABSTRACT_VALUE2ABSTRACT_ARRAY_impl);
  set_option_GetAbstractArrayLengthFuncPtr(param, &get_ABSTRACT_ARRAY_length_impl);
  set_option_IsAbstractValTrueFuncPtr(param, &is_ABSTRACT_VALUE_true_impl);
  set_option_GetAbstractMapFuncPtr(param, &get_ABSTRACT_MAP_impl);
  set_option_FindFileFuncPtr(param, &get_filepath);
  set_option_LoadFileFuncPtr(param, &load_file);
  set_option_UnloadFileFuncPtr(param, &unload_file);
  /*   setting initial Expr hooks */
  set_option_InitExprArglistFuncPtr(param, &init_expr_arglist);
  set_option_FreeExprArglistFuncPtr(param, &free_expr_arglist);
  set_option_PushExprArglistFuncPtr(param, &push_expr_arglist);
  set_option_CallExprUserfncFuncPtr(param, &call_expr_userfnc);
  set_option_IsExprUserfncFuncPtr(param, &is_expr_userfnc);
  /* end setting initial hooks */

  /*   setting perl globals */
  PerlSelfHTMLTemplatePro=PerlSelfPtr;
  if(PerlFilteredTmpls != NULL){
    av_undef(PerlFilteredTmpls);
  }else{
    PerlFilteredTmpls=newAV();
  }
  /*  end setting perl globals */

  /* setting pool for perl vars */
  PoolForPerlVars = newAV();
  /* end setting pool for perl vars */

  if ((!SvROK(PerlSelfPtr)) || (SvTYPE(SvRV(PerlSelfPtr)) != SVt_PVHV))
    {
      die("FATAL:SELF:hash pointer was expected but not found");
    }
  SelfHash=(HV *)SvRV(PerlSelfPtr);

  /* checking main arguments */
  filename=get_string_from_hash(SelfHash,"filename");
  scalarref=get_string_from_hash(SelfHash,"scalarref");
  set_option_filename(param, filename.begin);
  set_option_scalarref(param, scalarref);
  if (filename.begin==NULL && scalarref.begin==NULL) {
    die ("bad arguments: expected filename or scalarref");
  }
  
  /* setting expr_func */
  hashvalptr=hv_fetch(SelfHash, "expr_func", 9, 0); /* 9=strlen("expr_func") */
  if (!hashvalptr || !SvROK(*hashvalptr) || (SvTYPE(SvRV(*hashvalptr)) != SVt_PVHV))
    die("FATAL:output:EXPR user functions not found");
  set_option_expr_func_map(param, (HV *) SvRV(*hashvalptr));
  /* end setting expr_func */

  /* setting param_map */
  hashvalptr=hv_fetch(SelfHash, "param_map", 9, 0); /* 9=strlen("param_map") */
  if (!hashvalptr || !SvROK(*hashvalptr) || (SvTYPE(SvRV(*hashvalptr)) != SVt_PVHV))
    die("FATAL:output:param_map not found");

  set_option_root_param_map(param,  (HV *)SvRV(*hashvalptr));

  /* end setting param_map */

  /* setting filter */
  hashvalptr=hv_fetch(SelfHash, "filter", 6, 0); /* 6=strlen("filter") */
  if (!hashvalptr || !SvROK(*hashvalptr) || (SvTYPE(SvRV(*hashvalptr)) != SVt_PVAV))
    die("FATAL:output:filter not found");
  if (av_len((AV*)SvRV(*hashvalptr))>=0) set_option_filters(param, 1);
  /* end setting param_map */

  set_option_max_includes(param, get_integer_from_hash(SelfHash,"max_includes"));
  set_option_no_includes(param, get_integer_from_hash(SelfHash,"no_includes"));
  /* search_path_on_include proccessed directly in perl code */
  /* set_option_search_path_on_include(param,get_integer_from_hash(SelfHash,"search_path_on_include")); */
  set_option_global_vars(param, get_integer_from_hash(SelfHash,"global_vars"));
  set_option_debug(param, get_integer_from_hash(SelfHash,"debug"));
  set_option_loop_context_vars(param, get_integer_from_hash(SelfHash,"loop_context_vars"));
  set_option_case_sensitive(param, get_integer_from_hash(SelfHash,"case_sensitive"));
  set_option_path_like_variable_scope(param, get_integer_from_hash(SelfHash,"path_like_variable_scope"));
  /* still unsupported */
  set_option_strict(param, get_integer_from_hash(SelfHash,"strict"));
  set_option_die_on_bad_params(param, get_integer_from_hash(SelfHash,"die_on_bad_params"));
  
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
    case '0': case 'N': case 'n': /* 0 or NONE */
      default_escape = HTML_TEMPLATE_OPT_ESCAPE_NO;
      break;
    default:
      warn("unsupported value of default_escape=%s. Valid values are HTML, URL or JS.\n",tmpstring);
    }
    set_option_default_escape(param, default_escape);
  }
  return param;
}

static void
release_tmplpro_options(struct tmplpro_param* param)
{
  if(PoolForPerlVars != NULL){
    SvREFCNT_dec(PoolForPerlVars);
    PoolForPerlVars = NULL;
  }
  tmplpro_param_free(param);
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
	set_option_WriterFuncPtr(proparam,&write_chars_to_file);
	RETVAL = tmplpro_exec_tmpl(proparam);
	release_tmplpro_options(proparam);
	if (RETVAL!=0) warn ("non-zero exit code");
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
	set_option_WriterFuncPtr(proparam,&write_chars_to_string);
	retstate = tmplpro_exec_tmpl(proparam);
	release_tmplpro_options(proparam);
	if (retstate!=0) warn ("non-zero exit code");
	RETVAL = OutputString;
    OUTPUT:
	RETVAL

