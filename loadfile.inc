/* -*- c -*- 
 * File: loadfile.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu Sep  8 17:16:48 2005
 *
 * $Id: loadfile.inc,v 1.5 2006/02/04 19:37:39 igor Exp $
 */

#ifdef WIN32
/* 
 * the win32 code of Viacheslav Sheveliov <slavash@aha.ru>
 * 
 */

#include <windows.h>
#include <stdio.h>

static PSTRING mmap_load_file(const char *filepath) {
        PSTRING memarea = { NULL, NULL };
        HANDLE hFile, hMapObject = NULL;

        hFile = CreateFile(
                TEXT(filepath), 
                GENERIC_READ, 
                FILE_SHARE_READ, 
                NULL,                      
                OPEN_EXISTING, 
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 
                NULL
        );

        if (hFile != INVALID_HANDLE_VALUE) {
                hMapObject = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

                if (hMapObject) {
                        // Get a pointer to the file-mapped shared memory.
                        LPCTSTR lpvMem = (LPTSTR) MapViewOfFile(hMapObject, FILE_MAP_READ, 0, 0, 0);

                        if (lpvMem) {       
                                // Everything OK!

                                memarea.begin = (char *) lpvMem;
                                memarea.endnext = memarea.begin + GetFileSize(hFile, NULL);
                                // After MapViewOfFile we don't need file handles no more.
                                // Undocumented, but it works! (In read-only mode?)
                                CloseHandle(hMapObject);
                                CloseHandle(hFile);

                                return memarea;

                        }
                }
        }

        // Something goes wrong

        {
                // Save last error code, before any system call
                DWORD dwLastError = GetLastError();

                // Report error, if file size != 0
                // Mapping of zero-length file cause CreateFileMapping to fail.
                // So skip error messages in this case.
                if (hFile == INVALID_HANDLE_VALUE || GetFileSize(hFile, NULL) != 0)
                        fprintf(stderr, "Could not open file '%s'. (system error#%d)\n", filepath, dwLastError);

        }
        // Close all opened handles
        if (hMapObject) CloseHandle(hMapObject);
        if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);  

        return memarea;
}

// What is better?
/* static int mmap_unload_file(PSTRING memarea) { return UnmapViewOfFile((LPCVOID) memarea.begin) ? 0 : -1; }; */
// or
#define mmap_unload_file(map) (UnmapViewOfFile((LPCVOID) map.begin) ? 0 : -1)

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

