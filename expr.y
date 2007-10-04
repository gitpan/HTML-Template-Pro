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
  static int yylex (void);
  static void yyerror (char const *);
  /* expr-specific globals needed by yylex */
  static PSTRING expr_retval;
  static struct tmplpro_state* state;
  %}
%union {
  struct exprval numval;   /* For returning numbers.  */
  const symrec_const  *tptr;   /* For returning symbol-table pointers.  */
  PSTRING strname;  /* for user-defined function name */
  void* extfunc;  /* for user-defined function name */

}
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
		     expr_retval=$1.val.strval;
		   } else {
		     expr_to_dbl1(&$1); 
		     expr_retval=double_to_pstring($1.val.dblval,left_buffer,sizeof(left_buffer));
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
		   expr_to_dbl1(&$3);
		   $$.val.dblval = (*($1->fnctptr))($3.val.dblval); 
		 }
| numEXP '+' numEXP		{ DO_MATHOP($$,+,$1,$3);	}
| numEXP '-' numEXP		{ DO_MATHOP($$,-,$1,$3);	}
| numEXP '*' numEXP		{ DO_MATHOP($$,*,$1,$3);	}
| numEXP '%' numEXP
		 { 
		   $$.type=EXPRINT;
		   expr_to_int(&$1,&$3);
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
		       expr_debug("division by zero","");
                     }
		   ;break;
		   case EXPRDBL: 
                   if ($3.val.dblval)
                     $$.val.dblval = $1.val.dblval / $3.val.dblval;
                   else
                     {
                       $$.val.dblval = 0;
		       expr_debug("division by zero","");
                     }
		   }
		   ;break;
		 }
*/
| numEXP '/' numEXP
                 {
		   $$.type=EXPRDBL;
		   expr_to_dbl(&$1,&$3);
                   if ($3.val.dblval)
                     $$.val.dblval = $1.val.dblval / $3.val.dblval;
                   else
                     {
                       $$.val.dblval = 0;
		       expr_debug("division by zero","");
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
		   expr_to_dbl(&$1,&$3);
		   $$.val.dblval = pow ($1.val.dblval, $3.val.dblval);
                 }
| numEXP AND numEXP 		{ DO_LOGOP($$,&&,$1,$3);	}
| numEXP OR  numEXP 		{ DO_LOGOP($$,||,$1,$3);	}
| numEXP numGE numEXP 		{ DO_CMPOP($$,>=,$1,$3);	}
| numEXP numLE numEXP 		{ DO_CMPOP($$,<=,$1,$3);	}
| numEXP numNE numEXP 		{ DO_CMPOP($$,!=,$1,$3);	}
| numEXP numEQ numEXP 		{ DO_CMPOP($$,==,$1,$3);	}
| numEXP '>' numEXP %prec numGT	{ DO_CMPOP($$,>,$1,$3);		}
| numEXP '<' numEXP %prec numLT	{ DO_CMPOP($$,<,$1,$3);		}
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
  state->param->PushExprArglistFuncPtr(state->param,$1);
}
| arglist ',' numEXP	 { state->param->PushExprArglistFuncPtr(state->param,$3);	 }
;

/* End of grammar.  */
%%

/* Called by yyparse on error.  */
static void
yyerror (char const *s)
{
  expr_debug("not a valid expression:", s);
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

static
PSTRING expr;
static
char* curpos;

/* 
 * is_expect_quote_like allows recognization of quotelike.
 * if not is_expect_quote_like we look only for 'str' and, possibly, "str"
 * if is_expect_quote_like we also look for /str/.
 */
static int is_expect_quote_like;

PSTRING 
parse_expr (PSTRING expression, struct tmplpro_state* state_arg)
{
  expr=expression;
  curpos=expr.begin;
  expr_retval.begin=expression.begin;
  expr_retval.endnext=expression.begin;
  state=state_arg;
  is_expect_quote_like=1;
  yyparse ();
  return expr_retval;
}

static
void 
expr_debug(char const *msg1, char const *msg2) {
  tmpl_log(NULL, TMPL_LOG_ERROR, "EXPR:at pos %d: %s %s\n", curpos-expr.begin,msg1,msg2);
}

static
PSTRING 
fill_symbuf (int is_accepted(char)) {
  register char c=*curpos;
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
      c = *++curpos;
    }
  while (curpos<=expr.endnext && is_accepted(c));
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

#define TESTOP(c1,c2,z)  if (c1 == c) { char d=*++curpos; if (c2 != d) return c; else curpos++; return z; }
#define TESTOP3(c1,c2,c3,num2,str3)  if (c1 == c) { char d=*++curpos; if (c2 == d) {curpos++; return num2;} else if (c3 == d) {curpos++; is_expect_quote_like=1; return str3;} else return c; }

static 
int
yylex (void)
{
  register char c;
  int is_identifier_ext; 
  /* Ignore white space, get first nonwhite character.  */
  while (curpos<expr.endnext && ((c = *curpos) == ' ' || c == '\t')) curpos++;
  if (curpos>=expr.endnext) return 0;

  /* Char starts a quote => read a string */
  if ('\''==c || '"'==c || (is_expect_quote_like && '/'==c) ) {
    PSTRING strvalue;
    char terminal_quote=c;
    curpos++;
    strvalue.begin = curpos;
    strvalue.endnext = curpos;
    while (curpos<expr.endnext && 
	   (
	    /* it's buggy, but Expr 0.0.4 compatible :( */
	    ('\\' == c && ((c =*curpos) || 1)) /* any escaped char with \ , incl. quote */
	    || 
	    ((c = *curpos) != terminal_quote)) /* not end of string */
	    ) curpos++;
    strvalue.endnext=curpos;
    if (curpos<expr.endnext && ((c = *curpos) == terminal_quote)) curpos++;
    yylval.numval.val.strval=strvalue;
    yylval.numval.type=EXPRPSTR;
    is_expect_quote_like=0;
    return NUM;
  }
	
  is_expect_quote_like=0;
  /* Char starts a number => parse the number.         */
  if (c == '.' || isdigit (c))
    {
      yylval.numval=exp_read_number (&curpos, expr.endnext);
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
	curpos++; /* jump over { */
	name=fill_symbuf(is_not_identifier_ext_end);
	if (curpos<expr.endnext) curpos++; /* Jump the last } - Emiliano */
      } else {
	name=fill_symbuf(is_alnum_lex);
      }
      s = getsym (builtin_symrec, name.begin);
      if (s != 0) {
	yylval.tptr = s;
	return s->type;
      } else if ((yylval.extfunc=(state->param->IsExprUserfncFuncPtr)(state->param, name))) {
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
	  /*yylval.numval.val.strval=(PSTRING) {curpos, curpos};*/
	  yylval.numval.val.strval.begin=curpos;
	  yylval.numval.val.strval.endnext=curpos;
	  if (state->param->strict) expr_debug("non-initialized variable", name.begin);
	} else yylval.numval.val.strval=varvalue;
	yylval.numval.type=EXPRPSTR;
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
  curpos++;
  return c;
}

#include "exprtool.inc"
#include "exprpstr.inc"
