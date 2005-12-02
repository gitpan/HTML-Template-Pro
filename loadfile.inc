/* -*- c -*- 
 * File: loadfile.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu Sep  8 17:16:48 2005
 *
 * $Id: loadfile.inc,v 1.3 2005/12/02 07:24:50 igor Exp $
 */

#ifdef WIN32

#include <windows.h>
#include <stdio.h>

static 
HANDLE hMapObject;
static 
HANDLE hFile;

static 
PSTRING 
mmap_load_file (const char* filepath) {
  LPCTSTR lpvMem;
  DWORD size_in_bytes;
  PSTRING memarea =(PSTRING) {NULL,NULL};
 
  hFile = CreateFile(TEXT(filepath),    // file to open
                   GENERIC_READ,          // open for reading
                   FILE_SHARE_READ,       // share for reading
                   NULL,                  // default security
                   OPEN_EXISTING,         // existing file only
                   FILE_ATTRIBUTE_NORMAL, // normal file
                   NULL);                 // no attr. template
 
  if (hFile == INVALID_HANDLE_VALUE)
    { 
      printf("Could not open file (error %d)\n", GetLastError());
      return memarea;
    }
  size_in_bytes= GetFileSize(hFile, NULL);
  hMapObject = CreateFileMapping( 
           hFile, 
           NULL,                 // no security attributes
           PAGE_READONLY,       // read/write access
           0,                    // size: high 32-bits
           SHMEMSIZE,            // size: low 32-bits
           NULL);     // name of map object
  if (hMapObject != NULL) {
	// Get a pointer to the file-mapped shared memory.
	lpvMem = (LPTSTR) MapViewOfFile( 
	hMapObject,     // object to map view of
	FILE_MAP_READ, // read/write access
	0,              // high offset:  map from
	0,              // low offset:   beginning
	0);             // default: map entire file
	if (lpvMem == NULL) {
	  CloseHandle(hMapObject);
	}
  }
  if (lpvMem) {
    memarea.begin = (char *) lpvMem;
    memarea.endnext=memarea.begin+size_in_bytes;
  }
  return memarea;
}

static 
int 
mmap_unload_file (PSTRING memarea) {
  UnmapViewOfFile((void *)memarea.begin);
  CloseHandle(hMapObject);
  CloseHandle(hFile);
  return 0;
};

#else /* unix!!! :) */

/* # define NULL 0 */
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>  /* open  */
#include <unistd.h> /* close */

static 
PSTRING 
mmap_load_file (const char* filepath) {
  int fd;
  struct stat st;
  size_t size_in_bytes;
  PSTRING memarea={NULL,NULL};
  fd = open(filepath, O_RDONLY);
  if (fd == -1) return memarea; /* {NULL,NULL} */
  fstat(fd, &st);
  size_in_bytes = st.st_size;
  /* mmap size_in_bytes+1 to avoid crash with empty file */
  memarea.begin = (char *) mmap(0, size_in_bytes+1,  PROT_READ,  MAP_SHARED, fd, 0);
  close(fd);
  memarea.endnext=memarea.begin+size_in_bytes;
  return memarea;
}

static 
int 
mmap_unload_file (PSTRING memarea) {
  /* destroying */
  return munmap((void *)memarea.begin, memarea.endnext-memarea.begin);
}

#endif

