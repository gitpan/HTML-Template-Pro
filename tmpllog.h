/* -*- c -*- 
 * File: log.h
 * Author: Igor Vlasenko <vlasenko@imath.kiev.ua>
 * Created: Thu Sep  1 17:17:37 2005
 *
 * $Id: tmpllog.h,v 1.3 2005/09/30 11:00:37 igor Exp $
 */

/* based on FFmpeg av_log API */

#include <stdarg.h>

#define TMPL_LOG_QUIET -1
#define TMPL_LOG_ERROR 0
#define TMPL_LOG_INFO 1
#define TMPL_LOG_DEBUG 2
#define TMPL_LOG_DEBUG2 3

#ifdef __GNUC__
extern void tmpl_log(void*, int level, const char *fmt, ...) __attribute__ ((__format__ (__printf__, 3, 4)));
#else
extern void tmpl_log(void*, int level, const char *fmt, ...);
#endif

extern void tmpl_vlog(void*, int level, const char *fmt, va_list);
extern  int tmpl_log_get_level(void);
extern void tmpl_log_set_level(int);
extern void tmpl_log_set_callback(void (*)(void*, int, const char*, va_list));
