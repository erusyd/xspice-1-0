
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:04:27 $
 *           $Source: //pepper/atesse_spice/spice3/CP/RCS/std.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Standard utility routines.
 */

#include "prefix.h"
#include "CPstd.h"
#ifdef UNIX
#include <sys/types.h>
#include <signal.h>
#else
#ifdef VMS
#include <sys/types.h>
#include <time.h>
#endif /* VMS */
#endif /* UNIX */
#ifdef BSD
#include <sys/time.h>
#include <sys/resource.h>
#else
#ifndef CMS
#include <sys/types.h>      /* use ftime if not BSD */
#include <sys/timeb.h>
#endif /* CMS */
#endif
#ifdef HP_UX
#include <sys/times.h>      /* use times if HP_UX or system V like */
#endif
#include "suffix.h"

bool
prefix(p, s)
    register char *p, *s;
{
    while (*p && (*p == *s))
        p++, s++;
    if (!*p)
        return (true);
    else
        return (false);
}

/* Create a copy of a string. */

char *
copy(str)
    char *str;
{
    char *p;
    
    p = tmalloc(strlen(str) + 1);
    (void) strcpy(p, str);
    return(p);
}

/* Determine whether sub is a substring of str. */

bool
substring(sub, str)
    register char *str, *sub;
{
    register char *s;

    while (*str) {
        if (*str == *sub) {
            for (s = sub; *s; s++) {
                if (!*str || (*s != *str++))
                    break;
            }
            if (*s == '\0')
                return (true);
        }
        str++;
    }
    return (false);
}

/* Malloc num bytes and initialize to zero. Fatal error if the space can't
 * be malloc'd.   Note that ANSI C may return NULL for a request for 0
 * bytes, so we ask for 1 byte instead.
 */

char *
tmalloc(num)
    int num;
{
    char *s;
#ifdef MALLOCTRACE
    int i;
    static char *mem_alloc();

    if (!num)
        num = 1;

    if (!(s = mem_alloc((unsigned) num, 1, &i))) {
        fprintf(stderr, 
        "malloc: Internal Error: can't allocate %d bytes.\n", num);
        exit(EXIT_BAD);
    }
#else
    if (!(s = malloc((unsigned) num))) {
        fprintf(stderr, 
        "malloc: Internal Error: can't allocate %d bytes.\n", num);
        exit(EXIT_BAD);
    }
#endif
    bzero(s, num);
    return(s);
}

char *
trealloc(str, num)
    char *str;
    int num;
{
    char *s;
#ifdef MALLOCTRACE
    int i;
    static char *mem_alloc();

    if (!num)
        num = 1;

    if (!(s = mem_alloc((unsigned) num, 1, &i))) {
        fprintf(stderr,
        "realloc: Internal Error: can't allocate %d bytes.\n", num);
        exit(EXIT_BAD);
    }
    bcopy(str, s, num); /* Hope this won't cause a mem fault. */
#else
    if (!(s = realloc(str, (unsigned) num))) {
        fprintf(stderr, 
        "realloc: Internal Error: can't allocate %d bytes.\n", num);
        exit(EXIT_BAD);
    }
#endif
    return(s);
}

/* Append one character to a string. Don't check for overflow. */

void
appendc(s, c)
    char *s, c;
{
    while (*s)
        s++;
    *s++ = c;
    *s = '\0';
    return;
}

/* Try to identify an integer that begins a string. Stop when a non-
 * numeric character is reached.
 */

int
scannum(str)
    char *str;
{
    int i = 0;

    while(isdigit(*str))
        i = i * 10 + *(str++) - '0';
    return(i);
}

/* Case insensitive str eq. */

bool
cieq(p, s)
    register char *p, *s;
{
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) !=
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (*s ? false : true);
}

/* Case insensitive prefix. */

bool
ciprefix(p, s)
    register char *p, *s;
{
    while (*p) {
        if ((isupper(*p) ? tolower(*p) : *p) !=
            (isupper(*s) ? tolower(*s) : *s))
            return(false);
        p++;
        s++;
    }
    return (true);
}

#ifdef HP_UX
#define BSD     /* Well, it has the following required BSD'ism. */
#endif

#ifdef BSD

/* Return the date. Return value is static data. */

char *
datestring()
{
    register char *tzn;
    struct tm *tp;
    static char tbuf[40];
    char *ap;
    struct timeval tv;
    struct timezone tz;
    int i;

    (void) gettimeofday(&tv, &tz);
    tp = localtime((long *) &tv.tv_sec);
    ap = asctime(tp);
    tzn = timezone(tz.tz_minuteswest, tp->tm_isdst);
    (void) sprintf(tbuf, "%.20s", ap);
    if (tzn)
        (void) strcat(tbuf, tzn);
    (void) strcat(tbuf, ap + 19);
    i = strlen(tbuf);
    tbuf[i - 1] = '\0';
    return (tbuf);
}

#else

char *
datestring()
{
    return ("today");
}

#endif

#ifdef HP_UX
#undef BSD
#endif

/* return time interval in seconds and milliseconds */

#ifdef IBMPC

struct timeb timebegin;

timediff(now, begin, sec, msec)
struct timeb *now, *begin;
int *sec, *msec;
{

    *msec = now->millitm - begin->millitm;
    *sec = now->time - begin->time;
    if (*msec < 0) {
      *msec += 1000;
      (*sec)--;
    }
    return;

}

#endif

/* How many seconds have elapsed in running time. */

double
seconds()
{
#ifdef BSD
    struct rusage ruse;

    (void) getrusage(RUSAGE_SELF, &ruse);
    return (ruse.ru_utime.tv_sec + (double) ruse.ru_utime.tv_usec / 1000000.0);
#else
#ifdef IBMPC
    struct timeb timenow;
    int sec, msec;

    ftime(&timenow);
    timediff(&timenow, &timebegin, &sec, &msec);
    return(sec + (double) msec / 1000.0);
#else
#ifdef HP_UX
    struct tms tmsbuf;

    times(&tmsbuf);
    return((double) tmsbuf.tms_utime / HZ);
#else
#ifdef VMS
    return((double)clock()/(double)CLK_TCK);
#else /* unknown */
    /* don't know how to do this in general, so... */
    return(0.0);
#endif
#endif
#endif
#endif
}

/* A few things that may not exist on non-unix systems. */

#ifndef HAS_INDEX

#ifndef index

char *
index(s, c)
    register char *s;
    register char c;
{
    while ((*s != c) && (*s != '\0'))
        s++;
    if (*s == '\0')
        return ((char *) 0);
    else
        return (s);
}

#endif

#ifndef rindex

char *
rindex(s, c)
    register char *s;
    register char c;
{
    register char *t;

    for (t = s; *t != '\0'; t++);
    while ((*t != c) && (t != s))
        t--;
    if (t == s)
        return ((char *) 0);
    else
        return (t);
}

#endif

#endif

#ifndef HAS_BSTRING

#ifndef bcopy

void
bcopy(from, to, num)
    register char *from, *to;
    register int num;
{
    while (num-- > 0)
        *to++ = *from++;
    return;
}

#endif

#ifndef bzero

/* can't declare void here, because we've already used it in this file */
/* and haven't declared it void before the use */
int
bzero(ptr, num)
    register char *ptr;
    register int num;
{
    while (num-- > 0)
        *ptr++ = '\0';
    return (0);
}

#endif

#endif

/* This might not be around... If not then forget about sorting... */

#ifndef HAS_QSORT

#ifndef qsort

qsort() {}

#endif

#endif

char *
gettok(s)
    char **s;
{
    char buf[BSIZE];
    int i = 0;

    while (isspace(**s))
        (*s)++;
    if (!**s)
        return (NULL);
    while (**s && !isspace(**s))
        buf[i++] = *(*s)++;
    buf[i] = '\0';
    while (isspace(**s))
        (*s)++;
    return (copy(buf));
}

/* Print a number in a reasonable form.  This is the sort of thing that
 * %G does, but more appropriate for spice.  Returns static data.
 */

int cp_numdgt = -1;

char *
printnum(num)
    double num;
{
    static char buf[64];
    int n;

    if (cp_numdgt > 1)
        n = cp_numdgt;
    else
        n = 6;
    if (num < 0.0)
        n--;

    (void) sprintf(buf, "%.*le", n, num);

#ifdef notdef
    if ((num < 1.0) && (num > -1.0) && (num != 0.0))
        (void) sprintf(buf, "%.*le", n, num);
    else if (((num < 10.0) && (num > -10.0)) || (num == 0.0))
        /* It is not obvious that this is a good thing... */
        (void) sprintf(buf, "%.*G", num);
    else
        (void) sprintf(buf, "%.*le", n, num);
#endif

    return (buf);
}

#ifdef notdef

/* Some useful things... */

/* Die horribly. */

/* VARARGS1 */
void
fatal(s, args)
        char *s;
{
    fputs("Internal Error: ", stderr);
    _doprnt(s, &args, stderr);      Use vsprintf
    (void) putc('\n', stderr);

    (void) kill(getpid(), SIGIOT);
    /* NOTREACHED */
}

void
setenv(name, value)
    char *name, *value;
{
    int i;
    char **xx, *s;

    s = tmalloc(strlen(name) + 2);
    (void) sprintf(s, "%s=", name);

    /* Copy the old environment... */
    for (i = 0; environ[i]; i++)
        if (prefix(s, environ[i]))
            break;
    if (!environ[i]) {
        xx = (char **) tmalloc((i + 2) * sizeof (char *));
        for (i = 0; environ[i]; i++)
            xx[i] = environ[i];
        xx[i + 1] = NULL;
        environ = xx;
    } else
        xx = environ;
    
    xx[i] = tmalloc(strlen(name) + strlen(value) + 2);
    (void) sprintf(xx[i], "%s=%s", name, value);
    return;
}

char *
getusername()
{
    int i = getuid();
    struct passwd *pw = getpwuid(i);

    return (pw ? pw->pw_name : NULL);
}

char *
gethome()
{
    int i = getuid();
    struct passwd *pw = getpwuid(i);

    return (pw ? pw->pw_dir : "/strange");
}

#endif

