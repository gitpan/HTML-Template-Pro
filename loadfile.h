/* -*- c -*- 
 * File: loadfile.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu Sep  8 17:16:48 2005
 *
 * $Id: loadfile.h,v 1.1 2005/09/08 17:31:29 igor Exp $
 */

PSTRING mmap_load_file (const char* filepath);
int mmap_unload_file (PSTRING memarea);

