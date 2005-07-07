/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     dblNUM = 258,
     intNUM = 259,
     strVAL = 260,
     VAR = 261,
     FNCT = 262,
     EXTVAR = 263,
     OR = 264,
     AND = 265,
     strNE = 266,
     strEQ = 267,
     strLE = 268,
     strLT = 269,
     strGE = 270,
     strGT = 271,
     numNE = 272,
     numEQ = 273,
     numLE = 274,
     numLT = 275,
     numGE = 276,
     numGT = 277,
     NEG = 278,
     NOT = 279
   };
#endif
#define dblNUM 258
#define intNUM 259
#define strVAL 260
#define VAR 261
#define FNCT 262
#define EXTVAR 263
#define OR 264
#define AND 265
#define strNE 266
#define strEQ 267
#define strLE 268
#define strLT 269
#define strGE 270
#define strGT 271
#define numNE 272
#define numEQ 273
#define numLE 274
#define numLT 275
#define numGE 276
#define numGT 277
#define NEG 278
#define NOT 279




/* Copy the first part of user declarations.  */
#line 1 "expr.y"

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
  

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 14 "expr.y"
typedef union YYSTYPE {
  int    intval;   /* For returning numbers.  */
  double dblval;   /* For returning numbers.  */
 PSTRING strval;   /* For returning strings.  */
  symrec  *tptr;   /* For returning symbol-table pointers.  */
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 144 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 156 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  25
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   303

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  36
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  5
/* YYNRULES -- Number of rules. */
#define YYNRULES  51
/* YYNRULES -- Number of states. */
#define YYNSTATES  103

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   279

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,     2,     2,    29,     2,     2,
      34,    35,    27,    26,     2,    25,     2,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      17,     2,    18,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    33,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    19,    20,    21,    22,    23,    24,    31,    32
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    17,    21,
      25,    29,    33,    36,    40,    44,    48,    52,    56,    60,
      64,    68,    71,    74,    78,    82,    86,    90,    94,    98,
     102,   104,   106,   108,   113,   117,   121,   125,   129,   132,
     136,   140,   144,   148,   152,   156,   160,   164,   168,   171,
     174,   178
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      37,     0,    -1,    40,    -1,     5,    -1,     5,    -1,     8,
      -1,     4,    -1,    39,    26,    39,    -1,    39,    25,    39,
      -1,    39,    27,    39,    -1,    39,    29,    39,    -1,    39,
      28,    39,    -1,    25,    39,    -1,    39,    10,    39,    -1,
      39,     9,    39,    -1,    39,    23,    39,    -1,    39,    21,
      39,    -1,    39,    19,    39,    -1,    39,    20,    39,    -1,
      39,    18,    39,    -1,    39,    17,    39,    -1,    30,    39,
      -1,    32,    39,    -1,    34,    39,    35,    -1,    38,    15,
      38,    -1,    38,    13,    38,    -1,    38,    11,    38,    -1,
      38,    12,    38,    -1,    38,    16,    38,    -1,    38,    14,
      38,    -1,     3,    -1,     8,    -1,     6,    -1,     7,    34,
      40,    35,    -1,    40,    26,    40,    -1,    40,    25,    40,
      -1,    40,    27,    40,    -1,    40,    28,    40,    -1,    25,
      40,    -1,    40,    33,    40,    -1,    40,    10,    40,    -1,
      40,     9,    40,    -1,    40,    23,    40,    -1,    40,    21,
      40,    -1,    40,    19,    40,    -1,    40,    20,    40,    -1,
      40,    18,    40,    -1,    40,    17,    40,    -1,    30,    40,
      -1,    32,    40,    -1,    34,    40,    35,    -1,    39,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,    42,    42,    43,    47,    48,    51,    54,    55,    56,
      57,    59,    69,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      90,    91,    92,    94,    95,    96,    97,    99,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "dblNUM", "intNUM", "strVAL", "VAR", 
  "FNCT", "EXTVAR", "OR", "AND", "strNE", "strEQ", "strLE", "strLT", 
  "strGE", "strGT", "'<'", "'>'", "numNE", "numEQ", "numLE", "numLT", 
  "numGE", "numGT", "'-'", "'+'", "'*'", "'/'", "'%'", "'!'", "NEG", 
  "NOT", "'^'", "'('", "')'", "$accept", "line", "strEXP", "intEXP", 
  "dblEXP", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,    60,    62,   272,
     273,   274,   275,   276,   277,    45,    43,    42,    47,    37,
      33,   278,   279,    94,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    36,    37,    37,    38,    38,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    40,    40,
      40,    40
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     4,     3,     3,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,    30,     6,     4,    32,     0,    31,     0,     0,     0,
       0,     0,     0,    51,     2,     0,     4,    12,    38,    21,
      48,    22,    49,    51,     0,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    23,
      50,     5,    26,    27,    25,    29,    24,    28,     0,     0,
       0,     0,    14,    13,    20,    19,    17,    18,    16,    15,
       8,     7,     9,    11,    10,    41,    40,    47,    46,    44,
      45,    43,    42,    35,    34,    36,    37,    39,    33,    12,
      21,    22,     0
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,    11,    12,    13,    14
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -20
static const short yypact[] =
{
      52,   -20,   -20,    11,   -20,   -10,     4,    62,    62,    62,
      62,    38,   287,   194,   155,    62,   -20,   -20,     6,   -20,
       6,   -20,     6,    94,   115,   -20,     5,     5,     5,     5,
       5,     5,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,   135,   -20,
     -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,    68,    68,
      68,    68,   208,   255,   268,   268,   268,   268,   268,   268,
      -6,    -6,   -20,   -20,   -20,   174,   221,   238,   238,   238,
     238,   238,   238,   -19,   -19,     6,     6,     6,   -20,   -20,
     -20,   -20,    94
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -20,   -20,    79,    -7,    -3
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -6
static const yysigned_char yytable[] =
{
      17,    19,    21,    23,    18,    20,    22,    24,    55,    56,
      16,    -3,    58,    61,    57,    -5,    -5,    -5,    -5,    -5,
      -5,    42,    43,    44,    15,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    25,    57,
       0,     0,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,     1,     2,     3,     4,     5,
       6,    99,   100,   101,   102,     1,     2,    16,     4,     5,
       6,     0,     2,    16,     0,     0,    61,     7,     0,     0,
       0,     0,     8,     0,     9,     0,    10,     7,     0,     0,
       0,     0,     8,    68,     9,     0,    10,     0,    69,     0,
      70,     0,    71,    32,    33,    62,    63,    64,    65,    66,
      67,    34,    35,    36,    37,    38,     0,    39,     0,    40,
      41,    42,    43,    44,    45,    46,     0,     0,     0,    59,
       0,     0,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,    56,    45,    46,     0,     0,    57,     0,
      60,     0,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,    56,    45,    46,     0,     0,    57,     0,
      98,     0,    47,    48,    49,    50,    51,     0,    52,     0,
      53,    54,    55,    56,    46,     0,     0,     0,    57,     0,
       0,    47,    48,    49,    50,    51,     0,    52,     0,    53,
      54,    55,    56,    32,    33,     0,     0,    57,     0,     0,
       0,    34,    35,    36,    37,    38,     0,    39,    33,    40,
      41,    42,    43,    44,     0,    34,    35,    36,    37,    38,
       0,    39,     0,    40,    41,    42,    43,    44,    47,    48,
      49,    50,    51,     0,    52,     0,    53,    54,    55,    56,
       0,     0,     0,     0,    57,    -6,    -6,    -6,    -6,    -6,
       0,    -6,     0,    53,    54,    55,    56,     0,     0,     0,
       0,    57,    34,    35,    36,    37,    38,     0,    39,     0,
      40,    41,    42,    43,    44,    -6,    -6,    -6,    -6,    -6,
       0,    -6,     0,    40,    41,    42,    43,    44,    26,    27,
      28,    29,    30,    31
};

static const yysigned_char yycheck[] =
{
       7,     8,     9,    10,     7,     8,     9,    10,    27,    28,
       5,     0,    15,     8,    33,    11,    12,    13,    14,    15,
      16,    27,    28,    29,    34,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,     0,    33,
      -1,    -1,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,     3,     4,     5,     6,     7,
       8,    68,    69,    70,    71,     3,     4,     5,     6,     7,
       8,    -1,     4,     5,    -1,    -1,     8,    25,    -1,    -1,
      -1,    -1,    30,    -1,    32,    -1,    34,    25,    -1,    -1,
      -1,    -1,    30,    25,    32,    -1,    34,    -1,    30,    -1,
      32,    -1,    34,     9,    10,    26,    27,    28,    29,    30,
      31,    17,    18,    19,    20,    21,    -1,    23,    -1,    25,
      26,    27,    28,    29,     9,    10,    -1,    -1,    -1,    35,
      -1,    -1,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    28,     9,    10,    -1,    -1,    33,    -1,
      35,    -1,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    28,     9,    10,    -1,    -1,    33,    -1,
      35,    -1,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    28,    10,    -1,    -1,    -1,    33,    -1,
      -1,    17,    18,    19,    20,    21,    -1,    23,    -1,    25,
      26,    27,    28,     9,    10,    -1,    -1,    33,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    -1,    23,    10,    25,
      26,    27,    28,    29,    -1,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    28,    29,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    28,
      -1,    -1,    -1,    -1,    33,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    28,    -1,    -1,    -1,
      -1,    33,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    28,    29,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    28,    29,    11,    12,
      13,    14,    15,    16
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,    25,    30,    32,
      34,    37,    38,    39,    40,    34,     5,    39,    40,    39,
      40,    39,    40,    39,    40,     0,    11,    12,    13,    14,
      15,    16,     9,    10,    17,    18,    19,    20,    21,    23,
      25,    26,    27,    28,    29,     9,    10,    17,    18,    19,
      20,    21,    23,    25,    26,    27,    28,    33,    40,    35,
      35,     8,    38,    38,    38,    38,    38,    38,    25,    30,
      32,    34,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    40,    40,    40,    40,    40,
      40,    40,    40,    40,    40,    40,    40,    40,    35,    39,
      39,    39,    39
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 42 "expr.y"
    { expr_retval=double_to_pstring(yyvsp[0].dblval);	}
    break;

  case 3:
#line 43 "expr.y"
    { expr_retval=yyvsp[0].strval; 			}
    break;

  case 4:
#line 47 "expr.y"
    { yyval.strval = yyvsp[0].strval;			}
    break;

  case 5:
#line 48 "expr.y"
    { yyval.strval = yyvsp[0].strval;			}
    break;

  case 6:
#line 51 "expr.y"
    { yyval.intval = yyvsp[0].intval;			}
    break;

  case 7:
#line 54 "expr.y"
    { yyval.intval = yyvsp[-2].intval + yyvsp[0].intval;			}
    break;

  case 8:
#line 55 "expr.y"
    { yyval.intval = yyvsp[-2].intval - yyvsp[0].intval;			}
    break;

  case 9:
#line 56 "expr.y"
    { yyval.intval = yyvsp[-2].intval * yyvsp[0].intval;			}
    break;

  case 10:
#line 57 "expr.y"
    { yyval.intval = yyvsp[-2].intval % yyvsp[0].intval;			}
    break;

  case 11:
#line 60 "expr.y"
    {
                   if (yyvsp[0].intval)
                     yyval.intval = yyvsp[-2].intval / yyvsp[0].intval;
                   else
                     {
                       yyval.intval = 0;
		       expr_debug("division by zero","");
                     }
                 }
    break;

  case 12:
#line 69 "expr.y"
    { yyval.intval = -yyvsp[0].intval;			}
    break;

  case 13:
#line 71 "expr.y"
    { yyval.intval = yyvsp[-2].intval && yyvsp[0].intval;		}
    break;

  case 14:
#line 72 "expr.y"
    { yyval.intval = yyvsp[-2].intval || yyvsp[0].intval;		}
    break;

  case 15:
#line 73 "expr.y"
    { yyval.intval = yyvsp[-2].intval >= yyvsp[0].intval;		}
    break;

  case 16:
#line 74 "expr.y"
    { yyval.intval = yyvsp[-2].intval <= yyvsp[0].intval;		}
    break;

  case 17:
#line 75 "expr.y"
    { yyval.intval = yyvsp[-2].intval != yyvsp[0].intval;		}
    break;

  case 18:
#line 76 "expr.y"
    { yyval.intval = yyvsp[-2].intval == yyvsp[0].intval;		}
    break;

  case 19:
#line 77 "expr.y"
    { yyval.intval = yyvsp[-2].intval > yyvsp[0].intval;			}
    break;

  case 20:
#line 78 "expr.y"
    { yyval.intval = yyvsp[-2].intval < yyvsp[0].intval;			}
    break;

  case 21:
#line 79 "expr.y"
    { yyval.intval = !yyvsp[0].intval;			}
    break;

  case 22:
#line 80 "expr.y"
    { yyval.intval = !yyvsp[0].intval;			}
    break;

  case 23:
#line 81 "expr.y"
    { yyval.intval = yyvsp[-1].intval;			}
    break;

  case 24:
#line 82 "expr.y"
    { yyval.intval = pstring_ge (yyvsp[-2].strval,yyvsp[0].strval);	}
    break;

  case 25:
#line 83 "expr.y"
    { yyval.intval = pstring_le (yyvsp[-2].strval,yyvsp[0].strval);	}
    break;

  case 26:
#line 84 "expr.y"
    { yyval.intval = pstring_ne (yyvsp[-2].strval,yyvsp[0].strval);	}
    break;

  case 27:
#line 85 "expr.y"
    { yyval.intval = pstring_eq (yyvsp[-2].strval,yyvsp[0].strval);	}
    break;

  case 28:
#line 86 "expr.y"
    { yyval.intval = pstring_gt (yyvsp[-2].strval,yyvsp[0].strval);	}
    break;

  case 29:
#line 87 "expr.y"
    { yyval.intval = pstring_lt (yyvsp[-2].strval,yyvsp[0].strval);	}
    break;

  case 30:
#line 90 "expr.y"
    { yyval.dblval = yyvsp[0].dblval;			}
    break;

  case 31:
#line 91 "expr.y"
    { yyval.dblval = atof(yyvsp[0].strval.begin);		}
    break;

  case 32:
#line 92 "expr.y"
    { yyval.dblval = yyvsp[0].tptr->value.var;		}
    break;

  case 33:
#line 94 "expr.y"
    { yyval.dblval = (*(yyvsp[-3].tptr->value.fnctptr))(yyvsp[-1].dblval); }
    break;

  case 34:
#line 95 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval + yyvsp[0].dblval;			}
    break;

  case 35:
#line 96 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval - yyvsp[0].dblval;			}
    break;

  case 36:
#line 97 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval * yyvsp[0].dblval;			}
    break;

  case 37:
#line 100 "expr.y"
    {
                   if (yyvsp[0].dblval)
                     yyval.dblval = yyvsp[-2].dblval / yyvsp[0].dblval;
                   else
                     {
                       yyval.dblval = 0;
		       expr_debug("division by zero","");
                     }
                 }
    break;

  case 38:
#line 109 "expr.y"
    { yyval.dblval = -yyvsp[0].dblval;			}
    break;

  case 39:
#line 110 "expr.y"
    { yyval.dblval = pow (yyvsp[-2].dblval, yyvsp[0].dblval);		}
    break;

  case 40:
#line 111 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval && yyvsp[0].dblval;		}
    break;

  case 41:
#line 112 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval || yyvsp[0].dblval;		}
    break;

  case 42:
#line 113 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval >= yyvsp[0].dblval;		}
    break;

  case 43:
#line 114 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval <= yyvsp[0].dblval;		}
    break;

  case 44:
#line 115 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval != yyvsp[0].dblval;		}
    break;

  case 45:
#line 116 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval == yyvsp[0].dblval;		}
    break;

  case 46:
#line 117 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval > yyvsp[0].dblval;			}
    break;

  case 47:
#line 118 "expr.y"
    { yyval.dblval = yyvsp[-2].dblval < yyvsp[0].dblval;			}
    break;

  case 48:
#line 119 "expr.y"
    { yyval.dblval = !yyvsp[0].dblval;			}
    break;

  case 49:
#line 120 "expr.y"
    { yyval.dblval = !yyvsp[0].dblval;			}
    break;

  case 50:
#line 121 "expr.y"
    { yyval.dblval = yyvsp[-1].dblval;			}
    break;

  case 51:
#line 122 "expr.y"
    { yyval.dblval = yyvsp[0].intval;			}
    break;


    }

/* Line 999 of yacc.c.  */
#line 1419 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 126 "expr.y"


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



