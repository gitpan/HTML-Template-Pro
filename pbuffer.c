#include "pbuffer.h"

/* reentrant pbuffer functions */

TMPLPRO_LOCAL
size_t pbuffer_size(pbuffer* pBuffer) {
  return pBuffer->bufsize;
}
TMPLPRO_LOCAL
char* pbuffer_init(pbuffer* pBuffer) {
  pBuffer->bufsize=256;
  pBuffer->buffer=(char*) malloc (pBuffer->bufsize * sizeof(char));
  return pBuffer->buffer;
}
TMPLPRO_LOCAL
char* pbuffer_init_as(pbuffer* pBuffer,size_t size) {
  pBuffer->bufsize=size;
  pBuffer->buffer=(char*) malloc (pBuffer->bufsize * sizeof(char));
  return pBuffer->buffer;
}
TMPLPRO_LOCAL
char* pbuffer_string(pbuffer* pBuffer) {
  return pBuffer->buffer;
}
TMPLPRO_LOCAL
char* pbuffer_resize(pbuffer* pBuffer, size_t size) {
  if (pBuffer->bufsize< size) {
    pBuffer->bufsize=PBUFFER_MULTIPLICATOR*size; /* aggresive memory allocation to prevent frequent requests*/
    pBuffer->buffer=(char*) realloc (pBuffer->buffer,pBuffer->bufsize * sizeof(char));
  }
  return pBuffer->buffer;
}

TMPLPRO_LOCAL
void  pbuffer_free(pbuffer* pBuffer) {
  pBuffer->bufsize=0;
  free(pBuffer->buffer);
}

/*
 * Local Variables:
 * mode: c
 * End:
 */
