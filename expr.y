%{
#include <math.h>  /* For math functions, cos(), sin(), etc.  */
#include <stdio.h> /* for printf */
#include <stdlib.h> /* for malloc */
#include <ctype.h> /* for yylex alnum */
#include "calc.h"  /* Contains definition of `symrec'.  */
#include "procore.h"
#include "exprtool.h"
  int yylex (void);
  void yyerror (char const *);
  /* expr-specific globals needed by yylex */
  PSTRING expr_retval;
  struct tmplpro_param* param;
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
%nonassoc strGT strGE strLT strLE strEQ strNE
%nonassoc numGT numGE numLT numLE numEQ numNE '<' '>'
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
| numEXP strGE numEXP 		{ DO_TXTOP($$,pstring_ge,$1,$3);}
| numEXP strLE numEXP 		{ DO_TXTOP($$,pstring_le,$1,$3);}
| numEXP strNE numEXP 		{ DO_TXTOP($$,pstring_ne,$1,$3);}
| numEXP strEQ numEXP 		{ DO_TXTOP($$,pstring_eq,$1,$3);}
| numEXP strGT numEXP		{ DO_TXTOP($$,pstring_gt,$1,$3);}
| numEXP strLT numEXP		{ DO_TXTOP($$,pstring_lt,$1,$3);}
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
      yylval.numval=exp_read_number (&curpos, expr.endnext);
      return NUM;
    }
  /* Char starts an identifier => read the name.       */
  if (isalpha (c))
    {
      symrec *s;
      PSTRING name=fill_symbuf(is_alnum_lex);
      s = getsym (name.begin);
      if (s != 0) {
	yylval.tptr = s;
	return s->type;
      } else if ((yylval.extfunc=(param->IsExprUserfncFuncPtr)(param, name))) {
	// fprintf(stderr, "lex:detected func %s\n", name.begin);
	return EXTFUNC;
      } else {
	// s = putsym (symbuf, VAR);
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
	// fprintf(stderr, "lex:decoded number %s\n", name.begin);
	return NUM;
      }
    }
  /* Char starts a quote => read a string */
  if ('\''==c || '"'==c) {
    PSTRING strvalue;
    char terminal_quote=c;
    curpos++;
    strvalue = (PSTRING) {curpos, curpos};
    while (curpos<expr.endnext && ((c = *curpos) != terminal_quote)) curpos++;
    strvalue.endnext=curpos;
    if (curpos<expr.endnext && ((c = *curpos) == terminal_quote)) curpos++;
    yylval.numval.val.strval=strvalue;
    yylval.numval.type=EXPRPSTR;
    return NUM;
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

