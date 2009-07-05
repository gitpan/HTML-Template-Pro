#ifndef _PBUFFER_H
#define _PBUFFER_H	1

#include<stdlib.h>

typedef struct pbuffer {
  size_t bufsize;
  char*  buffer;
} pbuffer;

size_t pbuffer_size(pbuffer*);
char*  pbuffer_init(pbuffer*);
char*  pbuffer_string(pbuffer*);
char*  pbuffer_resize(pbuffer*, size_t size);
void   pbuffer_free(pbuffer*);

#endif /* pbuffer.h */

/*
 * Local Variables:
 * mode: c
 * End:
 */
