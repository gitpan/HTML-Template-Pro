%{
#include <math.h>  /* For math functions, cos(), sin(), etc.  */
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */
#include <ctype.h> /* for yylex alnum */
#include "calc.h"  /* Contains definition of `symrec'.  */
#include "procore.h"
  int yylex (void);
  void yyerror (char const *);
  /* expr-specific globals needed by yylex */
  void expr_debug(char const *,char const *);
  PSTRING expr_retval;
  %}
%union {
  int    intval;   /* For returning numbers.  */
  double dblval;   /* For returning numbers.  */
 PSTRING strval;   /* For returning strings.  */
  symrec  *tptr;   /* For returning symbol-table pointers.  */
}
%start line
%token <dblval>  dblNUM        /* Simple double precision number.  */
%token <intval>  intNUM        /* Simple integer number.  */
%token <strval>  strVAL        
/*%token <tptr> VAR FNCT*/   /* Variable and Function.  */
%token <tptr> VAR FNCT   /* Variable and Function.  */
%token <strval>  EXTVAR /* external (perl, etc) variable */
%type  <dblval>  dblEXP
%type  <intval>  intEXP
%type  <strval>  strEXP
     
/*%right '='*/
%left OR
%left AND
%nonassoc strGT strGE strLT strLE strEQ strNE
%nonassoc numGT numGE numLT numLE numEQ numNE '<' '>'
%left '-' '+'
%left '*' '/' '%'
%left  '!' NOT NEG /* negation--unary minus */
%right '^'    /* exponentiation */
%% /* The grammar follows.  */

line: dblEXP		{ expr_retval=double_to_pstring($1);	}
| strVAL		{ expr_retval=$1; 			}
;
/* | error { yyerrok;                  } */

strEXP: strVAL			{ $$ = $1;			}
| EXTVAR			{ $$ = $1;			}
;

intEXP: intNUM			{ $$ = $1;			}
/*| VAR '=' intEXP 		{ $$ = $3; $1->value.var = $3;	} */
/*| FNCT '(' intEXP ')'		{ $$ = (*($1->value.fnctptr))($3); }*/
| intEXP '+' intEXP		{ $$ = $1 + $3;			}
| intEXP '-' intEXP		{ $$ = $1 - $3;			}
| intEXP '*' intEXP		{ $$ = $1 * $3;			}
| intEXP '%' intEXP		{ $$ = $1 % $3;			}
/*| intEXP '/' intEXP		{ $$ = $1 / $3;    }*/
| intEXP '/' intEXP
                 {
                   if ($3)
                     $$ = $1 / $3;
                   else
                     {
                       $$ = 0;
		       expr_debug("division by zero","");
                     }
                 }
| '-' intEXP  %prec NEG	{ $$ = -$2;			}
/*| intEXP '^' intEXP 		{ $$ = pow ($1, $3);		}*/
| intEXP AND intEXP 		{ $$ = $1 && $3;		}
| intEXP OR  intEXP 		{ $$ = $1 || $3;		}
| intEXP numGE intEXP 		{ $$ = $1 >= $3;		}
| intEXP numLE intEXP 		{ $$ = $1 <= $3;		}
| intEXP numNE intEXP 		{ $$ = $1 != $3;		}
| intEXP numEQ intEXP 		{ $$ = $1 == $3;		}
| intEXP '>' intEXP %prec numGT	{ $$ = $1 > $3;			}
| intEXP '<' intEXP %prec numLT	{ $$ = $1 < $3;			}
| '!' intEXP  %prec NOT		{ $$ = !$2;			}
| NOT intEXP			{ $$ = !$2;			}
| '(' intEXP ')'		{ $$ = $2;			}
| strEXP strGE strEXP 		{ $$ = pstring_ge ($1,$3);	}
| strEXP strLE strEXP 		{ $$ = pstring_le ($1,$3);	}
| strEXP strNE strEXP 		{ $$ = pstring_ne ($1,$3);	}
| strEXP strEQ strEXP 		{ $$ = pstring_eq ($1,$3);	}
| strEXP strGT strEXP		{ $$ = pstring_gt ($1,$3);	}
| strEXP strLT strEXP		{ $$ = pstring_lt ($1,$3);	}
;
     
dblEXP: dblNUM			{ $$ = $1;			}
| EXTVAR			{ $$ = atof($1.begin);		}
| VAR				{ $$ = $1->value.var;		}
/*| VAR '=' dblEXP 		{ $$ = $3; $1->value.var = $3;	} */
| FNCT '(' dblEXP ')'		{ $$ = (*($1->value.fnctptr))($3); }
| dblEXP '+' dblEXP		{ $$ = $1 + $3;			}
| dblEXP '-' dblEXP		{ $$ = $1 - $3;			}
| dblEXP '*' dblEXP		{ $$ = $1 * $3;			}
/*| dblEXP '/' dblEXP		{ $$ = $1 / $3;    }*/
| dblEXP '/' dblEXP
                 {
                   if ($3)
                     $$ = $1 / $3;
                   else
                     {
                       $$ = 0;
		       expr_debug("division by zero","");
                     }
                 }
| '-' dblEXP  %prec NEG	{ $$ = -$2;			}
| dblEXP '^' dblEXP 		{ $$ = pow ($1, $3);		}
| dblEXP AND dblEXP 		{ $$ = $1 && $3;		}
| dblEXP OR  dblEXP 		{ $$ = $1 || $3;		}
| dblEXP numGE dblEXP 		{ $$ = $1 >= $3;		}
| dblEXP numLE dblEXP 		{ $$ = $1 <= $3;		}
| dblEXP numNE dblEXP 		{ $$ = $1 != $3;		}
| dblEXP numEQ dblEXP 		{ $$ = $1 == $3;		}
| dblEXP '>' dblEXP %prec numGT	{ $$ = $1 > $3;			}
| dblEXP '<' dblEXP %prec numLT	{ $$ = $1 < $3;			}
| '!' dblEXP  %prec NOT		{ $$ = !$2;			}
| NOT dblEXP			{ $$ = !$2;			}
| '(' dblEXP ')'		{ $$ = $2;			}
| intEXP			{ $$ = $1;			}
;

/* End of grammar.  */
%%

/* Called by yyparse on error.  */
void
yyerror (char const *s)
{
  expr_debug("not a valid expression:", s);
}
      
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
    {"or",  OR},
    {"and",AND},
    {"not",NOT},
     {0, 0}
  };

/* Put arithmetic functions in table.  */
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

PSTRING expr;
char* curpos;
struct tmplpro_param* param;

PSTRING parse_expr (PSTRING expression, struct tmplpro_param* param_arg)
{
  expr=expression;
  curpos=expr.begin;
  expr_retval=(PSTRING) {expression.begin, expression.begin};
  param=param_arg;
  yyparse ();
  return expr_retval;
}

void expr_debug(char const *msg1, char const *msg2) {
  fprintf(stderr, "EXPR:at pos %d: %s %s\n", curpos-expr.begin,msg1,msg2);
}

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

int is_float_lex (char c)
{
  return (c == '.' || isdigit (c));
}

int is_alnum_lex (char c)
{
  return (c == '_' || isalnum (c));
}

#define TESTOP(x,y,z)  if (x == c) { char d=*++curpos; if (y != d) return c; else curpos++; return z; }

int
yylex (void)
{
  register char c;
  /* Ignore white space, get first nonwhite character.  */
  while (curpos<expr.endnext && ((c = *curpos) == ' ' || c == '\t')) curpos++;
  if (curpos>=expr.endnext) return 0;

  /* Char starts a number => parse the number.         */
  if (c == '.' || isdigit (c))
    {
      yylval.dblval=atof(fill_symbuf(is_float_lex).begin);
      return dblNUM;
    }
  /* Char starts an identifier => read the name.       */
  if (isalpha (c))
    {
      symrec *s;
      PSTRING name=fill_symbuf(is_alnum_lex);
      s = getsym (name.begin);
      if (s == 0) {
	// s = putsym (symbuf, VAR);
	PSTRING varvalue;
	if (param->case_sensitive)
	 varvalue=(param->GetVarFuncPtr)(param, name);
	else
	 varvalue=(param->GetVarFuncPtr)(param, lowercase_pstring(name));

	if (varvalue.begin==NULL) {
	  yylval.strval=(PSTRING) {curpos, curpos};
	  if (param->strict) expr_debug("non-initialized variable", name.begin);
	} else yylval.strval=varvalue;
	return EXTVAR;
      } else {
	yylval.tptr = s;
	return s->type;
      }
    }
  /* Char starts a quote => read a string */
  if ('\''==c || '"'==c) {
    char terminal_quote=c;
    curpos++;
    PSTRING strvalue = {curpos, curpos};
    while (curpos<expr.endnext && ((c = *curpos) != terminal_quote)) curpos++;
    strvalue.endnext=curpos;
    if (curpos<expr.endnext && ((c = *curpos) == terminal_quote)) curpos++;
    yylval.strval = strvalue;
    return strVAL;
  }

  TESTOP('=','=',numEQ)
  TESTOP('>','=',numGE)
  TESTOP('<','=',numLE)
  TESTOP('!','=',numNE)
  TESTOP('&','&',AND)
  TESTOP('|','|',OR)

  /* Any other character is a token by itself. */
  curpos++;
  return c;
}

