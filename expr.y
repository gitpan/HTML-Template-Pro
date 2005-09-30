%{
#include <math.h>  /* For math functions, cos(), sin(), etc.  */
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */
#include <ctype.h> /* for yylex alnum */
#include "calc.h"  /* Contains definition of `symrec'.  */
#include "procore.h"
#include "exprtool.h"
  static int yylex (void);
  static void yyerror (char const *);
  /* expr-specific globals needed by yylex */
  static PSTRING expr_retval;
  static struct tmplpro_param* param;
  %}
%union {
  struct exprval numval;   /* For returning numbers.  */
  symrec  *tptr;   /* For returning symbol-table pointers.  */
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
		     expr_retval=double_to_pstring($1.val.dblval,left_buffer);
		   }
		 }
;
/* | error { yyerrok;                  } */

numEXP: NUM			{ $$ = $1;			}
| VAR				{ $$.type=EXPRDBL; $$.val.dblval = $1->value.var; }
/*| VAR '=' numEXP 		{ $$ = $3; $1->value.var = $3;	} */
| EXTFUNC '(' arglist ')'
                 {
		   $$ = param->CallExprUserfncFuncPtr(param, $1);
		 }
| EXTFUNC '(' ')'
                 {
		   param->InitExprArglistFuncPtr(param);
		   $$ = param->CallExprUserfncFuncPtr(param, $1);
		 }
| FNCT '(' numEXP ')'		
                 {
		   $$.type=EXPRDBL;
		   expr_to_dbl1(&$3);
		   $$.val.dblval = (*($1->value.fnctptr))($3.val.dblval); 
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
/*| numEXP '/' numEXP		{ $$ = $1 / $3;    }*/
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

/*arglist: {param->InitExprArglistFuncPtr(param);}
  | numEXP 	 	 { */
arglist: numEXP 	 	 { 
  param->InitExprArglistFuncPtr(param);
  param->PushExprArglistFuncPtr(param,$1);
}
| arglist ',' numEXP	 { param->PushExprArglistFuncPtr(param,$3);	 }
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
struct builtin_func
{
  char const *fname;
  double (*fnct) (double);
} const arith_fncts[] =
  {
    {"sin",  sin},
    {"cos",  cos},
    {"atan", atan},
    {"log",   log},
    {"exp",  exp},
    {"sqrt", sqrt},
     {0, 0}
  };

static
struct builtin_ops
{
  char const *oname;
  int const optag;
} const binary_ops[] =
  {
    {"eq",  strEQ},
    {"ne",  strNE},
    {"gt",  strGT},
    {"ge",  strGE},
    {"lt",  strLT},
    {"le",  strLE},
    {"cmp", strCMP},
    {"or",  OR},
    {"and",AND},
    {"not",NOT},
     {0, 0}
  };

#include "calc.inc"

/* Put arithmetic functions in table.  */
static 
void
initsym (void)
{
  int i;
  symrec *ptr;
  for (i = 0; arith_fncts[i].fname != 0; i++)
    {
      ptr = putsym (arith_fncts[i].fname, FNCT);
      ptr->value.fnctptr = arith_fncts[i].fnct;
    }
  for (i = 0; binary_ops[i].oname != 0; i++)
    {
      ptr = putsym (binary_ops[i].oname, binary_ops[i].optag);
      ptr->value.fnctptr = NULL;
    }
}
     
void
expr_init (void)
{
  initsym();
}

void
expr_free(void)
{
  freesym ();
}

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

PSTRING parse_expr (PSTRING expression, struct tmplpro_param* param_arg)
{
  expr=expression;
  curpos=expr.begin;
  expr_retval=(PSTRING) {expression.begin, expression.begin};
  param=param_arg;
  is_expect_quote_like=1;
  yyparse ();
  return expr_retval;
}

void expr_debug(char const *msg1, char const *msg2) {
  tmpl_log(NULL, TMPL_LOG_ERROR, "EXPR:at pos %d: %s %s\n", curpos-expr.begin,msg1,msg2);
}

static
PSTRING fill_symbuf (int is_accepted(char)) {
  register char c=*curpos;
  static char *symbuf = 0;
  static int symbuf_length = 0;
  int i=0;
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
  return (PSTRING) {symbuf, symbuf+i};
}

static int is_alnum_lex (char c)
{
  return (c == '_' || isalnum (c));
}

static int is_not_identifier_ext_end (char c)  
{ 
  return (c != '}');
} 

#define TESTOP(c1,c2,z)  if (c1 == c) { char d=*++curpos; if (c2 != d) return c; else curpos++; return z; }
#define TESTOP3(c1,c2,c3,num2,str3)  if (c1 == c) { char d=*++curpos; if (c2 == d) {curpos++; return num2;} else if (c3 == d) {curpos++; is_expect_quote_like=1; return str3;} else return c; }

static int
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
    strvalue = (PSTRING) {curpos, curpos};
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
  if (isalpha (c) || is_identifier_ext)
    {
      symrec *s;
      PSTRING name;
      if (is_identifier_ext) {
	curpos++; /* jump over { */
	name=fill_symbuf(is_not_identifier_ext_end);
	if (curpos<expr.endnext) curpos++; /* Jump the last } - Emiliano */
      } else {
	name=fill_symbuf(is_alnum_lex);
      }
      s = getsym (name.begin);
      if (s != 0) {
	yylval.tptr = s;
	return s->type;
      } else if ((yylval.extfunc=(param->IsExprUserfncFuncPtr)(param, name))) {
	/* tmpl_log(NULL, TMPL_LOG_ERROR, "lex:detected func %s\n", name.begin); */
	return EXTFUNC;
      } else {
	/* s = putsym (symbuf, VAR); */
	PSTRING varvalue;
	if (param->case_sensitive)
	 varvalue=(param->GetVarFuncPtr)(param, name);
	else
	 varvalue=(param->GetVarFuncPtr)(param, lowercase_pstring(name));
	if (varvalue.begin==NULL) {
	  yylval.numval.val.strval=(PSTRING) {curpos, curpos};
	  if (param->strict) expr_debug("non-initialized variable", name.begin);
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
