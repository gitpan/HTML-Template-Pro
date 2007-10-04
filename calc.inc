/* -*- c -*- 
 * File: calc.c
 * $Id$
 */

static
const symrec_const *
getsym (const symrec_const symrec_array[], char const *sym_name)
{
  const symrec_const* ptr;
  for (ptr=symrec_array; ptr->name!=NULL; ptr++) 
    if (strcmp (ptr->name,sym_name) == 0)
      return ptr;
  return 0;
}
