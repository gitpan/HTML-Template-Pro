#include <string.h>

/* Function type.  */
typedef double (*func_t) (double);
     
/* memory is allocated at compile time. it is also thread safe */
struct symrec_const
{
  char *name;  /* name of symbol */
  int type;    /* type of symbol: either VAR or FNCT */
  double var;      /* value of a VAR */
  func_t fnctptr;  /* value of a FNCT */
};

typedef struct symrec_const symrec_const;

/*
  Local Variables:
  mode: c
  End:
*/
