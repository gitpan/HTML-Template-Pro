/* 
 * File: tmplpro.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Sat Apr 15 21:15:24 2006
 *
 * $Id$
 */



#ifndef _PABIDECL_H
#define _PABIDECL_H	1

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(HTMLTMPLPRO_STATIC)
#   define CALL __stdcall
#   ifdef htmltmplpro_EXPORTS
#       define HTMLTMPLPRO_API __declspec(dllexport)
#   else
#       define HTMLTMPLPRO_API __declspec(dllimport)
#   endif
#else
#   define CALL
#   define HTMLTMPLPRO_API
#endif

#endif /* pabidecl.h */
