/* -*- c -*- 
 * File: calc.c
 * $Id$
 */
#include <string.h>

/* The symbol table: a chain of `struct symrec'. */
struct symrec *sym_table;

static
void freesym (void) {
  while (sym_table) {
    struct symrec *next_sym=sym_table->next;
    free (sym_table);
    sym_table=next_sym;
  }
}

static
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
     
static
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
