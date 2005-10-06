struct pstack_entry {
  int tag;
  int value;
  int vcontext;
  char* position;
};

static 
void pstack_init();
static 
void pstack_free();
static 
void pstack_push(struct pstack_entry);
static 
struct pstack_entry pstack_pop();
static 
struct pstack_entry pstack_top();
static 
struct pstack_entry* pstack_head();
static 
int pstack_notempty();
static 
int pstack_selftest();

/*
 * Local Variables:
 * mode: c 
 * coding: cp1251
 * End: 
 */
