#ifndef _PMISCDEF_H
#define _PMISCDEF_H	1

/* snprintf MS VC++ support;
 * thanks to Viacheslav Sheveliov <slavash@aha.ru>
 */
#ifdef _MSC_VER
#  define snprintf _snprintf
#endif

/* printf ptrdiff_t format modifier */
#if __STDC_VERSION__ >= 199901L
# define TO_PTRDIFF_T(X) (X)
# define MOD_TD "%td"
#elif defined _MSC_VER
# define TO_PTRDIFF_T(X) (X)
# define MOD_TD "%Id"
#else
# define TO_PTRDIFF_T(X) ((long) (X))
# define MOD_TD "%ld"
#endif

#endif /* pmiscdef.h */

/*
 * Local Variables:
 * mode: c
 * End:
 */
