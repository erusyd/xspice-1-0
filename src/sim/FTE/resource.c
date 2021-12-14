
/* RCS Info: $Revision: 1.2 $ on $Date: 91/12/19 16:58:04 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/resource.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Resource-related routines.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#ifdef BSD
#include <sys/time.h>
#include <sys/resource.h>
#else
#ifndef CMS
#include <sys/types.h>
#include <sys/timeb.h>
#endif /* CMS */
#endif

/* gtri - add - 12/12/90 - wbk - include ipc stuff */
#include "IPCtiein.h"
/* gtri - end - 12/12/90 */

#include "suffix.h"

extern edata;

static void printres();

void
com_rusage(wl)
    wordlist *wl;
{
    /* Fill in the SPICE accounting structure... */

    if (wl && (eq(wl->wl_word, "everything") || eq(wl->wl_word, "all"))) {
        printres((char *) NULL);
    } else if (wl) {
        for (; wl; wl = wl->wl_next) {
            printres(cp_unquote(wl->wl_word));
            if (wl->wl_next)
                (void) putc('\n', cp_out);
        }
    } else {
        printres("totaltime");
        (void) putc('\n', cp_out);
        printres("space");
        (void) putc('\n', cp_out);
        printres("elapsed");
    }
    return;
}

/* Find out if the user is approaching his maximum data size. */

#ifdef BSD

void
ft_ckspace()
{
#ifndef LINT
    struct rlimit rld;
    char *hi;

    getrlimit(RLIMIT_DATA, &rld);
    hi = sbrk(0);
    if (rld.rlim_cur == RLIM_INFINITY)  /* Wow. */
        return;
    if ((int) (hi - (char *) &edata) > rld.rlim_max * 0.9) {
        fprintf(cp_err, "Warning - approaching max data size: ");
        fprintf(cp_err, "cur size = %d, hard limit = %d.\n", 
                (int) hi, (int) rld.rlim_max);
    } else if ((int) (hi - (char *) &edata) > rld.rlim_cur * 0.9) {
        fprintf(cp_err, "Warning - approaching max data size: ");
        fprintf(cp_err, "cur size = %d, soft limit = %d.\n", 
                (int) hi, (int) rld.rlim_cur);
    } 
#endif
    return;
}

#else

void
ft_ckspace() {}

#endif

/* Print out one piece of resource usage information. */

static void
printres(name)
    char *name;
{
    bool yy = false;
    static long lastsec = -1, lastusec = -1;
    struct variable *v;
#ifdef IBMPC
    struct timeb timenow;
    extern struct timeb timebegin;
    int sec, msec;
#endif
    /* gtri - add - 12/12/90 - wbk - a temp for testing purposes  */
    double ipc_test;
    /* gtri - end - 12/12/90 - wbk - */

    if (!name || eq(name, "totaltime")) {
#ifdef BSD
        struct rusage ruse;

        (void) getrusage(RUSAGE_SELF, &ruse);
        fprintf(cp_out, "Total run time: %lu.%03lu seconds.\n",
            ruse.ru_utime.tv_sec, ruse.ru_utime.tv_usec / 1000);
        lastsec = ruse.ru_utime.tv_sec;
        lastusec = ruse.ru_utime.tv_usec;

        /* gtri - add - 12/12/90 - wbk - record cpu time used for ipc */
        g_ipc.cpu_time = lastsec;
        ipc_test = lastsec;
        g_ipc.cpu_time = (double) lastusec;
        g_ipc.cpu_time /= 1.0e6;
        g_ipc.cpu_time += (double) lastsec;
        /* gtri - end - 12/12/90 */

#endif
#ifdef IBMPC
    ftime(&timenow);
    timediff(&timenow, &timebegin, &sec, &msec);
    fprintf(cp_out, "Total run time: %u.%03u seconds.\n", sec, msec);
#endif
        yy = true;
    } 
    if (!name || eq(name, "space")) {
#ifdef BSD
#ifndef LINT
        struct rlimit rld;
        char *hi;

        getrlimit(RLIMIT_DATA, &rld);
        hi = sbrk(0);
        fprintf(cp_out, "Current data size = %d,\n",
        (int) (hi - (char *) &edata));
        fprintf(cp_out, "Data limits: hard = %d, soft = %d.\n",
            rld.rlim_max, rld.rlim_cur);
#endif
        yy = true;
#endif
    } 
    if (!name || eq(name, "faults")) {
#ifdef BSD
        struct rusage ruse;

        (void) getrusage(RUSAGE_SELF, &ruse);
        fprintf(cp_out, 
        "%lu page faults, %lu vol + %lu invol = %lu context switches.\n",
                ruse.ru_majflt, ruse.ru_nvcsw, ruse.ru_nivcsw, 
                ruse.ru_nvcsw + ruse.ru_nivcsw);
#endif
        yy = true;
    } 
    if (!name || eq(name, "elapsed")) {
#ifdef BSD
        struct rusage ruse;

        (void) getrusage(RUSAGE_SELF, &ruse);
        if (lastsec >= 0) {
            if (ruse.ru_utime.tv_usec < lastusec) {
                ruse.ru_utime.tv_sec--;
                ruse.ru_utime.tv_usec += 1000000;
            }

            fprintf(cp_out,
                "Time since last call: %lu.%03lu seconds.\n",
                ruse.ru_utime.tv_sec - lastsec, 
                (ruse.ru_utime.tv_usec - lastusec) / 1000);
        }
        lastsec = ruse.ru_utime.tv_sec;
        lastusec = ruse.ru_utime.tv_usec;
#endif
        yy = true;
    } 

    /* Now get all the spice resource stuff. */
    if (ft_curckt && ft_curckt->ci_ckt) {
        v = if_getstat(ft_curckt->ci_ckt, name);
        if (name && v) {
            fprintf(cp_out, "%s = ", v->va_name);
            wl_print(cp_varwl(v), cp_out);
            (void) putc('\n', cp_out);
            yy = true;
        } else if (v) {
            (void) putc('\n', cp_out);
            while (v) {
                fprintf(cp_out, "%s = ", v->va_name);
                wl_print(cp_varwl(v), cp_out);
                (void) putc('\n', cp_out);
                v = v->va_next;
            }
            yy = true;
        }
    }

    if (!yy) {
        fprintf(cp_err, "Error: no rusage information on %s,\n", name);
        fprintf(cp_err, "\tor no active circuit available\n");
        
    }
    return;
}

