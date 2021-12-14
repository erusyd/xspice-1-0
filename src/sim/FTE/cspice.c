
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:11:18 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/cspice.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * The main routine for spice 3 in batch mode (small binary version).
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEparse.h"
#include "FTEdata.h"
#include "FTEinp.h"
#include "FTEdebug.h"
#include "IFsim.h"
#include "INPdefs.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#ifdef UNIX
#include <pwd.h>
#endif
#ifdef IBMPC            /* really should be ndef BSD, clear up later */
#include <sys/types.h>
#include <sys/timeb.h>
#endif
#include "suffix.h"

char *errRtn;
char *errMsg;
/* This is left false in the spice front end. */
bool ft_nutmeg = false;
char *cp_program;
#ifdef UNIX
char *ft_rawfile = "rawspice";
#else
char *ft_rawfile = "rawspice.raw";
#endif
bool ft_debug = false;
bool ft_servermode = false;
bool ft_nospiceadd = true;
bool ft_simdb = false;
static char *usage = "Usage: %s [ - ] [ -r rawfile ] [ file ... ]\n";
static started = false;
IFsimulator *ft_sim;

FILE *cp_in, *cp_curin;
FILE *cp_out, *cp_curout;
FILE *cp_err, *cp_curerr;
bool gi_endpause = true;

extern int OUTpBeginPlot(), OUTpData(), OUTwBeginPlot(), OUTwReference();
extern int OUTwData(), OUTwEnd(), OUTendPlot(), OUTbeginDomain();
extern int OUTendDomain(), OUTstopnow(), OUTerror(), OUTattributes();

static IFfrontEnd nutmeginfo = {
    IFnewUid,
    OUTstopnow,
    seconds,
    OUTerror,
    OUTpBeginPlot,
    OUTpData,
    OUTwBeginPlot,
    OUTwReference,
    OUTwData,
    OUTwEnd,
    OUTendPlot,
    OUTbeginDomain,
    OUTendDomain,
    OUTattributes
};


/* The main routine for batch spice. */

#ifndef OPTCHAR
#define OPTCHAR '-'
#endif

#ifdef IBMPC
int _stack = 8000;
struct timeb timebegin;     /* for use w/ ftime */
#endif

void
main(ac, av)
    char **av;
{
    char **tv;
    bool gotone = false;
    bool st = false;
    int tc, i;
    int err;

    /* MFB tends to jump to 0 on errors... This will catch it. */
    if (started) {
        fprintf(cp_err, "Internal Error: jump to zero\n");
        fatal();
    }
    started = true;
    cp_curin = cp_in = stdin;
    cp_curout = cp_out = stdout;
    cp_curerr = cp_err = stderr;

    err = SIMinit(&nutmeginfo,&ft_sim);
    if(err != OK) {
        ft_sperror(err,"SIMinit");
        exit(1);
    }
    cp_program = ft_sim->simulator;
#ifdef UNIX
    srandom(getpid());
#else
    srandom(17);    /* Yeah... */
#endif

    tv = av;
    tc = ac;

    /* Pass 1 -- get options. */
    while (--tc > 0) {
        tv++;
        if (**tv == OPTCHAR)    /* Option argument. */
            switch ((*tv)[1]) {
                case 'r':   /* The rawfile. */
                case 'R':
                    if (tc > 1) {
                        tc--;
                        tv++;
                        ft_rawfile = copy(*tv);
                        **tv = OPTCHAR; /* Hide... */
                    } else {
                        fprintf(cp_err, usage, 
                            cp_program);
                        exit(EXIT_BAD);
                    }
                    break;
                case 'o':   /* Output file. */
                case 'O':
                    if (tc > 1) {
                        tc--;
                        tv++;
                        if (!(freopen(*tv, "w", 
                                stdout))) {
                            perror(*tv);
                            exit(EXIT_BAD);
                        }
                        **tv = OPTCHAR; /* Hide... */
                    } else {
                        fprintf(cp_err, usage, 
                            av[0]);
                        exit(EXIT_BAD);
                    }
                    break;
                default:
                    fprintf(cp_err, usage, cp_program);
                    exit(EXIT_BAD);
            }
    }

    /* Pass 2 -- get the filenames. If we are spice, then this means
     * build a circuit for this file. If this is in server mode, don't
     * process any of these args.
     */

#ifdef IBMPC
    ftime(&timebegin);      /* time we started */
#endif

#ifdef CMS
cp_interactive = false;
#endif
#ifndef CMS
    if (!ft_servermode) {
        FILE *file = NULL, *tp = NULL;
        char *tempfile = NULL, buf[BSIZE], *mktemp();

        for (tv = av + 1, i = 0; *tv; tv++)
            if (**tv != OPTCHAR)
                i++;
        if (i == 1) {
            for (tv = av + 1, i = 0; *tv; tv++)
                if (**tv != OPTCHAR)
                    break;
            file = fopen(*tv, "r");
        } else if (i) {
            tempfile = mktemp("/tmp/spXXXXXX");
            if (!(file = fopen(tempfile, "w+"))) {
                perror(tempfile);
                exit(EXIT_BAD);
            }
            for (tv = av + 1, i = 0; *tv; tv++)
                if (**tv != OPTCHAR) {
                    if (!(tp = fopen(*tv, "r"))) {
                        perror(tempfile);
                        exit(EXIT_BAD);
                    }
                    while ((i = fread(buf, 1, BSIZE, tp))
                            > 0)
                        (void) fwrite(buf, i, 1, file);
                    (void) fclose(tp);
                }
            (void) fseek(file, (long) 0, 0);
        }
        if (file) {
            inp_spsource(file, false, tempfile ? (char *) NULL :
                    *tv);
            gotone = true;
            if (tempfile)
                (void) unlink(tempfile);
        }
    }
#endif

    if (!gotone)
        inp_spsource(stdin, false, (char *) NULL);

    ft_nospiceadd = false;

    /* If we get back here in batch mode then something is
     * wrong, so exit.
     */
    if (st == true)
        exit(EXIT_BAD);
    st = true;
    if (ft_servermode) {
        if (ft_curckt == NULL) {
            fprintf(cp_err, "Error: No circuit loaded!\n");
            exit(EXIT_BAD);
        }
    }

    ft_savedotargs();
    ft_dorun((char *) NULL);
    ft_cktcoms(false);

/*
    ft_savedotargs();
    ft_dorun(ft_rawfile);
    ft_cktcoms(false);
*/

    exit(EXIT_NORMAL);
}

/* Now some misc junk that we need to fake... */

bool cp_nocc = true;
bool cp_debug = false;
int cp_maxhistlength;
char cp_chars[128];
bool ft_parsedb = false;

void cp_pushcontrol() {}
void cp_popcontrol() {}
/* ARGSUSED */ char *cp_tildexpand(s) char *s; { return (s); }
/* ARGSUSED */ void cp_addkword(class, word) int class; char *word; {}
struct circ *ft_circuits, *ft_curckt;
/* ARGSUSED */ char *cp_kwswitch(c, t) int c; char *t; { return (NULL); }
/* ARGSUSED */ bool ft_bpcheck(r, i) struct plot *r; int i; { return (true); }
/* ARGSUSED */ void cp_ccon(b) bool b; {}
/* ARGSUSED */ void gr_iplot(pl) struct plot *pl; {}
/* ARGSUSED */ void gr_end_iplot(pl) struct plot *pl; {}
/* ARGSUSED */ void gr_pmsg(text, more) char *text; bool more; {}
/* ARGSUSED */ int cp_evloop(string) char *string; { return (0); }
/* ARGSUSED */ void gr_clean(intr) bool intr; {}
void ft_trquery() {}
/* ARGSUSED */ void ft_newcirc(ckt) struct circ *ckt; {}

void cp_doquit() { exit(0); }
/* ARGSUSED */ void cp_usrvars(v1, v2) struct variable **v1, **v2; { return; }
/* ARGSUSED */ struct variable * cp_enqvar(word) char *word; { return (NULL); }
/* ARGSUSED */ void cp_ccom(w, b, e) wordlist *w; char *b; bool e; { return; }
/* ARGSUSED */ int cp_usrset(v, i) struct variable *v; bool i; {return(US_OK);}

/* This is from breakpoint.c */

struct dbcomm *dbs = NULL;      /* export for iplot */
static int debugnumber = 1;

static void
settrace(wl, what)
    wordlist *wl;
{
    struct dbcomm *d, *td;
    char *s;

    while (wl) {
        s = cp_unquote(wl->wl_word);
        d = alloc(dbcomm);
        d->db_number = debugnumber++;
        if (eq(s, "all")) {
            switch (what) {
                case VF_PRINT:
                    d->db_type = DB_TRACEALL;
                    break;
/*              case VF_PLOT:
                    d->db_type = DB_IPLOTALL;
                    break; */
                case VF_ACCUM:
                    d->db_type = DB_SAVEALL;
                    break;
            }
/*          wrd_chtrace((char *) NULL, true, what); */
        } else {
            switch (what) {
                case VF_PRINT:
                    d->db_type = DB_TRACENODE;
                    break;
/*              case VF_PLOT:
                    d->db_type = DB_IPLOT;
                    break; */
                case VF_ACCUM:
                    d->db_type = DB_SAVE;
                    break;
            }
            d->db_nodename1 = copy(s);
/*          wrd_chtrace(s, true, what); */
        }
        if (dbs) {
            for (td = dbs; td->db_next; td = td->db_next)
                ;
            td->db_next = d;
        } else
            dbs = d;
        wl = wl->wl_next;
    }
    return;
}

/* Save a vector. */

void
com_save(wl)
    wordlist *wl;
{
    settrace(wl, VF_ACCUM);
    return;
}

/* This is from dotcards.c -- we don't want to include the whole file. */

int
ft_getSaves(savesp)
    char ***savesp;
{
    return (0);
}

/* This is from options.c */

/* Extract the .option cards from the deck... */

struct line *
inp_getopts(deck)
    struct line *deck;
{
    struct line *last = NULL, *opts = NULL, *dd, *next = NULL;

    for (dd = deck->li_next; dd; dd = next) {
        next = dd->li_next;
        if (ciprefix(".opt", dd->li_line)) {
            inp_casefix(dd->li_line);
            if (last)
                last->li_next = dd->li_next;
            else
                deck->li_next = dd->li_next;
            dd->li_next = opts;
            opts = dd;
        } else
            last = dd;
    }
    return (opts);
}

