/* -*- c -*- 
 * File: calc.c
 * $Id: calc.c,v 1.1 2005/06/09 08:46:42 igor Exp $
 */
#include <string.h>
#include <stdlib.h>
#include "calc.h"

/* The symbol table: a chain of `struct symrec'. */
struct symrec *sym_table;

void freesym (void) {
  while (sym_table) {
    struct symrec *next_sym=sym_table->next;
    free (sym_table);
    sym_table=next_sym;
  }
}

symrec *
putsym (char const *sym_name, int sym_type)
{
  symrec *ptr;
  ptr = (symrec *) malloc (sizeof (symrec));
  ptr->name = (char *) malloc (strlen (sym_name) + 1);
  strcpy (ptr->name,sym_name);
  ptr->type = sym_type;
  ptr->value.var = 0; /* Set value to 0 even if fctn.  */
  ptr->next = (struct symrec *)sym_table;
  sym_table = ptr;
  return ptr;
}
     
symrec *
getsym (char const *sym_name)
{
  symrec *ptr;
  /*
  symrec rec;
  */
  for (ptr = sym_table; ptr != (symrec *) 0;
       ptr = (symrec *)ptr->next)
    if (strcmp (ptr->name,sym_name) == 0)
      return ptr;
  /*
    printf ("Enter %s value:",sym_name);
    scanf ("%lf", &rec.value);
  */
  return 0;
}
