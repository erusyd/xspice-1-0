#define _SUFFIX_H_

/* New fix to nail the NULL typecast problem */
#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

#define index strchr
#define rindex strrchr
#define has_qsort

#ifdef FABS
#undef FABS
#endif /*FABS*/

#define FABS fabs

#ifdef max
#undef max
#endif /*max*/

#ifdef min
#undef min
#endif /*min*/
