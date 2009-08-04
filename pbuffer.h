#ifndef _PBUFFER_H
#define _PBUFFER_H	1

#include<stdlib.h>
#include<pabidecl.h>

typedef struct pbuffer {
  size_t bufsize;
  char*  buffer;
} pbuffer;

TMPLPRO_LOCAL size_t pbuffer_size(pbuffer*);
TMPLPRO_LOCAL char*  pbuffer_init(pbuffer*);
TMPLPRO_LOCAL char*  pbuffer_init_as(pbuffer* pBuffer,size_t size);
TMPLPRO_LOCAL char*  pbuffer_string(pbuffer*);
TMPLPRO_LOCAL char*  pbuffer_resize(pbuffer*, size_t size);
TMPLPRO_LOCAL void   pbuffer_free(pbuffer*);

#endif /* pbuffer.h */

/*
 * Local Variables:
 * mode: c
 * End:
 */
