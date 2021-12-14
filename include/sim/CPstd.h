
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 11:25:32 $
 *         $Source: //pepper/atesse_spice/spice3/include/RCS/CPstd.h,v $
 * Copyright (c) 1986 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Standard definitions. This file serves as the header file for std.c and
 * wlist.c
 */

#ifndef _STD_H_
#define _STD_H_

#ifndef FILE
#include <stdio.h>
#endif
#ifndef isalpha
#include <ctype.h>
#endif
#ifndef HUGE
#include <math.h>
#endif
#include <string.h>

typedef int bool;

#define false 0
#define true 1

/* Doubly linked lists of words. */

struct wordlist {
    char *wl_word;
    struct wordlist *wl_next;
    struct wordlist *wl_prev;
} ;

typedef struct wordlist wordlist;

/* Complex numbers. */

struct _complex {   /* IBM portability... */
    double cx_real;
    double cx_imag;
} ;

typedef struct _complex complex;

#define realpart(cval)  ((struct _complex *) (cval))->cx_real
#define imagpart(cval)  ((struct _complex *) (cval))->cx_imag

/* Externs defined in std.c */

extern char *tmalloc();
extern char *trealloc();
extern char *copy();
extern char *gettok();
extern char *datestring();
extern char *getusername();
extern char *gethome();
extern char *tildexpand();
extern char *printnum();
extern int cp_numdgt;
#ifndef CMS
extern void fatal();
#endif
extern void setenv();
extern void appendc();
extern int scannum();
extern double seconds();
extern bool prefix();
extern bool ciprefix();
extern bool cieq();
extern bool substring();
extern void cp_printword();

/* Externs from wlist.c ... */

extern char **wl_mkvec();
extern char *wl_flatten();
extern int wl_length();
extern void wl_free();
extern void wl_print();
extern void wl_sort();
extern wordlist *wl_append();
extern wordlist *wl_build();
extern wordlist *wl_copy();
extern wordlist *wl_range();
extern wordlist *wl_nthelem();
extern wordlist *wl_reverse();
extern wordlist *wl_splice();

/* Externs from libc */

extern char *getenv();
extern int errno;
extern char *sys_errlist[];
extern char *calloc();
extern char *mktemp();
extern char *getenv();
extern char *malloc();
extern char *realloc();
extern char *getwd();
extern long random();
extern int atoi();
extern int bzero();
extern char *rindex();
extern char *index();
extern int kill();
extern int getpid();
extern int qsort();
extern int rand();

extern void exit();
extern void bcopy();
#ifdef BSD
#ifndef clearerr
extern void clearerr();
#endif /* clearerr */
#endif /* BSD */
#ifndef HAS_PERROR
/* lattice and ibm cc declares this in stdio.h, and differently.... */
extern void perror();
#endif
extern void srandom();

#ifdef BSD
extern char *sbrk();
extern char *timezone();
extern char *asctime();
extern struct tm *localtime();
#endif /*BSD*/

/* Should use BSIZE instead of BUFSIZ... */

#define BSIZE      512

/* Some standard macros. */

#define eq(a,b)  (!strcmp((a), (b)))
#define isalphanum(c)   (isalpha(c) || isdigit(c))
#define alloc(strname)  ((struct strname *) tmalloc(sizeof(struct strname)))
#ifdef MALLOCTRACE
#define tfree(ptr)  { if (ptr) mem_free((char *) ptr); ptr = 0; }
#else /*MALLOCTRACE*/
#define tfree(ptr)  { if (ptr) free((char *) ptr); ptr = 0; }
extern void free();
#endif /*MALLOCTRACE*/
#define hexnum(c) ((((c) >= '0') && ((c) <= '9')) ? ((c) - '0') : ((((c) >= \
        'a') && ((c) <= 'f')) ? ((c) - 'a' + 10) : ((((c) >= 'A') && \
        ((c) <= 'F')) ? ((c) - 'A' + 10) : 0)))

#ifndef BSD
#define random rand
#define srandom srand
#endif /*BSD*/

#ifdef VMS

#define EXIT_NORMAL 1
#define EXIT_BAD    0

#else /* VMS */

#define EXIT_NORMAL 0
#define EXIT_BAD    1

#endif /*VMS*/

#endif /*not _STD_H_*/

