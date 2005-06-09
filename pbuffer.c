#include "pbuffer.h"

struct pbuffer {
  size_t initcount;
  size_t bufsize;
  char*  buffer;
};

struct pbuffer StrBuffer;

size_t pbuffer_size() {
  return StrBuffer.bufsize;
}
char* pbuffer_init() {
  if (0==StrBuffer.initcount) {
    StrBuffer.bufsize=256;
    StrBuffer.buffer=(char*) malloc (StrBuffer.bufsize * sizeof(char));
  }
  StrBuffer.initcount++;
  return StrBuffer.buffer;
}
char* pbuffer_string() {
  return StrBuffer.buffer;
}
char* pbuffer_resize(size_t size) {
  if (StrBuffer.bufsize< size) {
    StrBuffer.bufsize=2*size; /* aggresive memory allocation to prevent frequent requests*/
    realloc (StrBuffer.buffer,StrBuffer.bufsize * sizeof(char));
  }
  return StrBuffer.buffer;
}
void  pbuffer_free() {
  if (0!=StrBuffer.initcount) StrBuffer.initcount--;
  if (0==StrBuffer.initcount) {
    StrBuffer.bufsize=0;
    free(StrBuffer.buffer);
  }
}

/*
 * Local Variables:
 * mode: c
 * coding: cp1251
 * End:
 */
