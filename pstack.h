struct pstack_entry {
  int tag;
  int value;
  int vcontext;
  char* position;
};

void pstack_init();
void pstack_free();
void pstack_push(struct pstack_entry);
struct pstack_entry pstack_pop();
struct pstack_entry pstack_top();
struct pstack_entry* pstack_head();
int pstack_notempty();
int pstack_selftest();

/*
 * Local Variables:
 * mode: c 
 * coding: cp1251
 * End: 
 */
