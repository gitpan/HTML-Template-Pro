%pure-parser
%parse-param {struct tmplpro_state* state}
%lex-param {struct tmplpro_state* state}
%parse-param {PSTRING* expr_retval_ptr}
%{
#include <math.h>  /* For math functions, cos(), sin(), etc.  */
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */
#include <ctype.h> /* for yylex alnum */
#include "calc.h"  /* Contains definition of `symrec'.  */
#include "procore.h"
#include "prostate.h"
#include "exprtool.h"
#include "exprpstr.h"
  %}
%union {
  struct exprval numval;   /* For returning numbers.  */
  const symrec_const  *tptr;   /* For returning symbol-table pointers.  */
  void* extfunc;  /* for user-defined function name */
}
%{
  /* the second section is required as we use YYSTYPE here */
  static void yyerror (struct tmplpro_state* state, PSTRING* expr_retval_ptr, char const *);
  static int yylex (YYSTYPE *lvalp, struct tmplpro_state* state);
%}
%start line
%token <numval>  NUM        /*  poly type.  */
%token <extfunc> EXTFUNC    /* user-defined function */
%token <tptr> VAR FNCT   /* built-in Variable and Function.  */
%type  <numval>  numEXP
%type  <numval>  arglist
     
/*%right '='*/
%left OR
%left AND
%nonassoc strGT strGE strLT strLE strEQ strNE strCMP
%nonassoc numGT numGE numLT numLE numEQ numNE '<' '>'
%nonassoc reLIKE reNOTLIKE
%left '-' '+'
%left '*' '/' '%'
%left  '!' NOT NEG /* negation--unary minus */
%right '^'    /* exponentiation */
%% /* The grammar follows.  */

line: numEXP		
		 { 
		   if (EXPRPSTR == $1.type) {
		     *expr_retval_ptr=$1.val.strval;
		   } else {
		     expr_to_dbl1(state, &$1); 
		     *expr_retval_ptr=double_to_pstring($1.val.dblval,left_buffer,sizeof(left_buffer));
		   }
		 }
;
/* | error { yyerrok;                  } */

numEXP: NUM			{ $$ = $1;			}
| VAR				{ $$.type=EXPRDBL; $$.val.dblval = $1->var; }
/*| VAR '=' numEXP 		{ $$ = $3; $1->value.var = $3;	} */
| EXTFUNC '(' arglist ')'
                 {
		   $$ = state->param->CallExprUserfncFuncPtr(state->param, $1);
		 }
| EXTFUNC '(' ')'
                 {
		   state->param->InitExprArglistFuncPtr(state->param);
		   $$ = state->param->CallExprUserfncFuncPtr(state->param, $1);
		 }
| FNCT '(' numEXP ')'		
                 {
		   $$.type=EXPRDBL;
		   expr_to_dbl1(state, &$3);
		   $$.val.dblval = (*($1->fnctptr))($3.val.dblval); 
		 }
| numEXP '+' numEXP		{ DO_MATHOP(state, $$,+,$1,$3);	}
| numEXP '-' numEXP		{ DO_MATHOP(state, $$,-,$1,$3);	}
| numEXP '*' numEXP		{ DO_MATHOP(state, $$,*,$1,$3);	}
| numEXP '%' numEXP
		 { 
		   $$.type=EXPRINT;
		   expr_to_int(state, &$1,&$3);
		   $$.val.intval = $1.val.intval % $3.val.intval;
		 }
/* old division; now always return double (due to compains 1/3==0)
| numEXP '/' numEXP
                 {
		   switch ($$.type=expr_to_int_or_dbl(&$1,&$3)) {
		   case EXPRINT: 
                   if ($3.val.intval)
                     $$.val.intval = $1.val.intval / $3.val.intval;
                   else
                     {
                       $$.val.intval = 0;
		       expr_debug(state, "division by zero","");
                     }
		   ;break;
		   case EXPRDBL: 
                   if ($3.val.dblval)
                     $$.val.dblval = $1.val.dblval / $3.val.dblval;
                   else
                     {
                       $$.val.dblval = 0;
		       expr_debug(state, "division by zero","");
                     }
		   }
		   ;break;
		 }
*/
| numEXP '/' numEXP
                 {
		   $$.type=EXPRDBL;
		   expr_to_dbl(state, &$1,&$3);
                   if ($3.val.dblval)
                     $$.val.dblval = $1.val.dblval / $3.val.dblval;
                   else
                     {
                       $$.val.dblval = 0;
		       expr_debug(state, "division by zero","");
                     }
		 }
| '-' numEXP  %prec NEG
		 { 
		   switch ($$.type=$2.type) {
		   case EXPRINT: 
		     $$.val.intval = -$2.val.intval;
		   ;break;
		   case EXPRDBL: 
		     $$.val.dblval = -$2.val.dblval;
		   ;break;
		   }
		 }
| numEXP '^' numEXP 		
                 { 
		   $$.type=EXPRDBL;
		   expr_to_dbl(state, &$1,&$3);
		   $$.val.dblval = pow ($1.val.dblval, $3.val.dblval);
                 }
| numEXP AND numEXP 		{ DO_LOGOP(state, $$,&&,$1,$3);	}
| numEXP OR  numEXP 		{ DO_LOGOP(state, $$,||,$1,$3);	}
| numEXP numGE numEXP 		{ DO_CMPOP(state, $$,>=,$1,$3);	}
| numEXP numLE numEXP 		{ DO_CMPOP(state, $$,<=,$1,$3);	}
| numEXP numNE numEXP 		{ DO_CMPOP(state, $$,!=,$1,$3);	}
| numEXP numEQ numEXP 		{ DO_CMPOP(state, $$,==,$1,$3);	}
| numEXP '>' numEXP %prec numGT	{ DO_CMPOP(state, $$,>,$1,$3);	}
| numEXP '<' numEXP %prec numLT	{ DO_CMPOP(state, $$,<,$1,$3);	}
| '!' numEXP  %prec NOT		{ DO_LOGOP1($$,!,$2);		}
| NOT numEXP			{ DO_LOGOP1($$,!,$2);		}
| '(' numEXP ')'		{ $$ = $2;			}
| numEXP strCMP numEXP 		{ 
  expr_to_str(&$1,&$3); 
  $$.type=EXPRINT; $$.val.intval = pstring_ge ($1.val.strval,$3.val.strval)-pstring_le ($1.val.strval,$3.val.strval);
}
| numEXP strGE numEXP 		{ DO_TXTOP($$,pstring_ge,$1,$3);}
| numEXP strLE numEXP 		{ DO_TXTOP($$,pstring_le,$1,$3);}
| numEXP strNE numEXP 		{ DO_TXTOP($$,pstring_ne,$1,$3);}
| numEXP strEQ numEXP 		{ DO_TXTOP($$,pstring_eq,$1,$3);}
| numEXP strGT numEXP		{ DO_TXTOP($$,pstring_gt,$1,$3);}
| numEXP strLT numEXP		{ DO_TXTOP($$,pstring_lt,$1,$3);}
| numEXP reLIKE numEXP		{ DO_TXTOP($$,re_like,$1,$3);}
| numEXP reNOTLIKE numEXP	{ DO_TXTOP($$,re_notlike,$1,$3);}
;

/*arglist: {state->param->InitExprArglistFuncPtr(state->param);}
  | numEXP 	 	 { */
arglist: numEXP 	 	 { 
  state->param->InitExprArglistFuncPtr(state->param);
  state->param->PushExprArglistFuncPtr(state->param,expr_unescape_pstring_val(state, $1));
}
| arglist ',' numEXP	 { state->param->PushExprArglistFuncPtr(state->param,expr_unescape_pstring_val(state, $3));	}
;

/* End of grammar.  */
%%

/* Called by yyparse on error.  */
static void
yyerror (struct tmplpro_state* state, PSTRING* expr_retval_ptr, char const *s)
{
  expr_debug(state, "not a valid expression:", s);
}

static
const symrec_const
const builtin_symrec[] =
  {
    /* built-in funcs */
    {"sin", FNCT,	0,	  sin},
    {"cos", FNCT,	0,	  cos},
    {"atan", FNCT,	0,	 atan},
    {"log", FNCT,	0,	  log},
    {"exp", FNCT,	0,	  exp},
    {"sqrt", FNCT,	0,	 sqrt},

    /* built-in ops */
    {"eq",  strEQ,	0,	NULL},
    {"ne",  strNE,	0,	NULL},
    {"gt",  strGT,	0,	NULL},
    {"ge",  strGE,	0,	NULL},
    {"lt",  strLT,	0,	NULL},
    {"le",  strLE,	0,	NULL},
    {"cmp", strCMP,	0,	NULL},
    {"or",  OR,	0,	NULL},
    {"and",AND,	0,	NULL},
    {"not",NOT,	0,	NULL},
    /* end mark */
    {0, 0, 0}
  };

#include "calc.inc"

PSTRING 
parse_expr (PSTRING expression, struct tmplpro_state* state)
{
  PSTRING expr_retval;
  state->expr_curpos=expression.begin;
  state->expr=expression;
  expr_retval.begin=expression.begin;
  expr_retval.endnext=expression.begin;
  state->is_expect_quote_like=1;
  yyparse (state, &expr_retval); 
  return expr_retval;
}

static
void 
expr_debug(struct tmplpro_state* state, char const *msg1, char const *msg2) {
  tmpl_log(NULL, TMPL_LOG_ERROR, "EXPR:at pos %d: %s %s\n", (state->expr_curpos)-(state->expr).begin,msg1,msg2);
}

static
PSTRING 
fill_symbuf (struct tmplpro_state* state, int is_accepted(char)) {
  register char c=*(state->expr_curpos);
  static char *symbuf = 0;
  static int symbuf_length = 0;
  int i=0;
  PSTRING retval;
  /* Initially make the buffer long enough
     for a 40-character symbol name.  */
  if (symbuf_length == 0)
    symbuf_length = 40, symbuf = (char *)malloc (symbuf_length + 1);
  do
    {
      /* If buffer is full, make it bigger.        */
      if (i == symbuf_length)
	{
	  symbuf_length *= 2;
	  symbuf = (char *) realloc (symbuf, symbuf_length + 1);
	}
      symbuf[i++] = c;
      c = *++(state->expr_curpos);
    }
  while ((state->expr_curpos)<=(state->expr).endnext && is_accepted(c));
  symbuf[i] = '\0';
  retval.begin=symbuf;
  retval.endnext=symbuf+i;
  return retval;
}

static 
int 
is_alnum_lex (char c)
{
  return (c == '_' || isalnum (c));
}

static 
int 
is_not_identifier_ext_end (char c)  
{ 
  return (c != '}');
} 

#define TESTOP(c1,c2,z)  if (c1 == c) { char d=*++(state->expr_curpos); if (c2 != d) return c; else (state->expr_curpos)++; return z; }
#define TESTOP3(c1,c2,c3,num2,str3)  if (c1 == c) { char d=*++(state->expr_curpos); if (c2 == d) {(state->expr_curpos)++; return num2;} else if (c3 == d) {(state->expr_curpos)++; state->is_expect_quote_like=1; return str3;} else return c; }

static 
int
yylex (YYSTYPE *lvalp, struct tmplpro_state* state)
{
  register char c;
  int is_identifier_ext; 
  /* Ignore white space, get first nonwhite character.  */
  while ((state->expr_curpos)<(state->expr).endnext && ((c = *(state->expr_curpos)) == ' ' || c == '\t')) (state->expr_curpos)++;
  if ((state->expr_curpos)>=(state->expr).endnext) return 0;

  /* Char starts a quote => read a string */
  if ('\''==c || '"'==c || (state->is_expect_quote_like && '/'==c) ) {
    PSTRING strvalue;
    char terminal_quote=c;
    char escape_flag = 0;
    c =* ++(state->expr_curpos);
    strvalue.begin = (state->expr_curpos);
    strvalue.endnext = (state->expr_curpos);

    while ((state->expr_curpos)<(state->expr).endnext && c != terminal_quote) {
      /* any escaped char with \ , incl. quote */
      if ('\\' == c) {
	escape_flag = 1;
	state->expr_curpos+=2;
	c =*(state->expr_curpos);
      } else {
	c = * ++(state->expr_curpos);
      }
    }

    strvalue.endnext=(state->expr_curpos);
    if ((state->expr_curpos)<(state->expr).endnext && ((c = *(state->expr_curpos)) == terminal_quote)) (state->expr_curpos)++;
    (*lvalp).numval.val.strval=strvalue;
    (*lvalp).numval.type=EXPRPSTR;
    (*lvalp).numval.strval_escape_flag=escape_flag;
    state->is_expect_quote_like=0;
    return NUM;
  }
	
  state->is_expect_quote_like=0;
  /* Char starts a number => parse the number.         */
  if (c == '.' || isdigit (c))
    {
      (*lvalp).numval=exp_read_number (state, &(state->expr_curpos), (state->expr).endnext);
      return NUM;
    }

  /* 
   * Emiliano Bruni extension to Expr:
   * original HTML::Template allows almost arbitrary chars in parameter names,
   * but original HTML::Template::Expr (as to 0.04) allows only
   * var to be m![A-Za-z_][A-Za-z0-9_]*!.
   * with this extension, arbitrary chars can be used 
   * if bracketed in {}, as, for example, EXPR="{foo.bar} eq 'a'".
   *
   * COMPATIBILITY WARNING.
   * Currently, this extension is not present in HTML::Template::Expr (as of 0.04).
   */
  /* Let's try to see if this is an identifier between two { } - Emiliano */
  is_identifier_ext = (int) (c == '{'); 

  /* Char starts an identifier => read the name.       */
  /* variables with _leading_underscore are allowed too */
  if (isalpha (c) || c=='_' || is_identifier_ext)
    {
      const symrec_const *s;
      PSTRING name;
      if (is_identifier_ext) {
	(state->expr_curpos)++; /* jump over { */
	name=fill_symbuf(state, is_not_identifier_ext_end);
	if ((state->expr_curpos)<(state->expr).endnext) (state->expr_curpos)++; /* Jump the last } - Emiliano */
      } else {
	name=fill_symbuf(state, is_alnum_lex);
      }
      s = getsym (builtin_symrec, name.begin);
      if (s != 0) {
	(*lvalp).tptr = s;
	return s->type;
      } else if (((*lvalp).extfunc=(state->param->IsExprUserfncFuncPtr)(state->param, name))) {
	/* tmpl_log(NULL, TMPL_LOG_ERROR, "lex:detected func %s\n", name.begin); */
	return EXTFUNC;
      } else {
	PSTRING varvalue;
	if (! state->param->case_sensitive) {
	  lowercase_pstring_inplace(name);
	}
	varvalue=get_variable_value(state, name);
	/* tmpl_log(NULL, TMPL_LOG_ERROR, "lex:detected var %s=%s\n", name.begin,varvalue.begin); */
	if (varvalue.begin==NULL) {
	  /*(*lvalp).numval.val.strval=(PSTRING) {(state->expr_curpos), (state->expr_curpos)};*/
	  (*lvalp).numval.val.strval.begin=(state->expr_curpos);
	  (*lvalp).numval.val.strval.endnext=(state->expr_curpos);
	  if (state->param->strict) expr_debug(state, "non-initialized variable", name.begin);
	} else (*lvalp).numval.val.strval=varvalue;
	(*lvalp).numval.type=EXPRPSTR;
	(*lvalp).numval.strval_escape_flag=0;
	return NUM;
      }
    }

  TESTOP3('=','=','~',numEQ,reLIKE)
  TESTOP3('!','=','~',numNE,reNOTLIKE)
  /*  
      TESTOP('=','=',numEQ)
      TESTOP('!','=',numNE)
  */
  TESTOP('>','=',numGE)
  TESTOP('<','=',numLE)
  TESTOP('&','&',AND)
  TESTOP('|','|',OR)

  /* Any other character is a token by itself. */
  (state->expr_curpos)++;
  return c;
}

#include "exprtool.inc"
#include "exprpstr.inc"
