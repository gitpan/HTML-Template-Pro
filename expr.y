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
  double expr_retval;
  %}
%union {
  double    val;   /* For returning numbers.  */
  symrec  *tptr;   /* For returning symbol-table pointers.  */
}
%token <val>  NUM        /* Simple double precision number.  */
/*%token <tptr> VAR FNCT*/   /* Variable and Function.  */
%token <tptr> VAR FNCT   /* Variable and Function.  */
%token <val> EXTVAR /* external (perl, etc) variable */
%type  <val>  exp
     
%right '='
%left OR
%left AND
%left GT GE LT LE EQ NE
%left '-' '+'
%left '*' '/'
%left NEG NOT /* negation--unary minus */
%right '^'    /* exponentiation */
%% /* The grammar follows.  */

line: exp  {  expr_retval=$1; }
;
/* | error { yyerrok;                  } */
     
exp: NUM             { $$ = $1;				}
| EXTVAR             { $$ = $1;				}
| VAR                { $$ = $1->value.var;		}
/*| VAR '=' exp        { $$ = $3; $1->value.var = $3;     } */
| FNCT '(' exp ')'   { $$ = (*($1->value.fnctptr))($3); }
| exp '+' exp        { $$ = $1 + $3;			}
| exp '-' exp        { $$ = $1 - $3;			}
| exp '*' exp        { $$ = $1 * $3;			}
/*| exp '/' exp        { $$ = $1 / $3;    }*/
| exp '/' exp
                 {
                   if ($3)
                     $$ = $1 / $3;
                   else
                     {
                       $$ = 0;
		       expr_debug("division by zero","");
                     }
                 }
| '-' exp  %prec NEG	{ $$ = -$2;			}
| exp '^' exp 		{ $$ = pow ($1, $3);		}
| exp AND exp 		{ $$ = $1 && $3;		}
| exp OR  exp 		{ $$ = $1 || $3;		}
| exp GE  exp 		{ $$ = $1 >= $3;		}
| exp LE  exp 		{ $$ = $1 <= $3;		}
| exp NE  exp 		{ $$ = $1 != $3;		}
| exp EQ  exp 		{ $$ = $1 == $3;		}
| exp '>' exp %prec GT	{ $$ = $1 > $3;			}
| exp '<' exp %prec LT	{ $$ = $1 < $3;			}
| '!' exp  %prec NOT	{ $$ = !$2;			}
| NOT exp		{ $$ = !$2;			}
| '(' exp ')'		{ $$ = $2;			}
;
/* End of grammar.  */
%%

/* Called by yyparse on error.  */
void
yyerror (char const *s)
{
  expr_debug("not a valid expression", s);
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
    {"ln",   log},
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
    {"eq",  EQ},
    {"ne",  NE},
    {"gt",  GT},
    {"ge",  GE},
    {"lt",  LT},
    {"le",  LE},
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

double parse_expr (PSTRING expression, struct tmplpro_param* param_arg)
{
  expr=expression;
  curpos=expr.begin;
  expr_retval=0.0;
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
      yylval.val=atof(fill_symbuf(is_float_lex).begin);
      return NUM;
    }
  /* Char starts an identifier => read the name.       */
  if (isalpha (c))
    {
      symrec *s;
      PSTRING name=fill_symbuf(is_alnum_lex);
      s = getsym (name.begin);
      if (s == 0) {
	// s = putsym (symbuf, VAR);
	PSTRING varvalue =(param->GetVarFuncPtr)(param, name);
	if (varvalue.begin==NULL) {
	  yylval.val=0;
	  if (param->strict) expr_debug("non-initialized variable", name.begin);
	} else yylval.val = atof(varvalue.begin);
	return EXTVAR;
      } else {
	yylval.tptr = s;
	return s->type;
      }
    }

  TESTOP('=','=',EQ)
  TESTOP('>','=',GE)
  TESTOP('<','=',LE)
  TESTOP('!','=',NE)
  TESTOP('&','&',AND)
  TESTOP('|','|',OR)

  /* Any other character is a token by itself. */
  curpos++;
  return c;
}

