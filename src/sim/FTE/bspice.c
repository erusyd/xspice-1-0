
/* RCS Info: $Revision: 1.4 $ on $Date: 92/02/05 17:21:27 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/bspice.c,v $
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

/* gtri - add - 12/12/90 - wbk - include ipc stuff */
#include "IPCtiein.h"
/* gtri - end - 12/12/90 */

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
bool ft_listprint = true;
bool ft_optsprint = false;
bool ft_nodesprint = false;
bool ft_acctprint = false;
bool ft_nopage = false;
bool ft_simdb = false;
static char *usage = "Usage: %s [ - ] [ -r rawfile ] [ file ... ]\n";
static started = false;
IFsimulator *ft_sim;

FILE *cp_in, *cp_curin;
FILE *cp_out, *cp_curout;
FILE *cp_err, *cp_curerr;
struct plot *plot_list = NULL;
int raw_prec = -1;
bool gi_endpause = true;
int plot_num = 1;

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

/* gtri - add - 12/13/90 - wbk - ipc processing data */

    char    ipc_path[IPC_MAX_PATH_LEN+1];   /* Pathname to use for mailbox */
    char    ipc_log_file[IPC_MAX_PATH_LEN+1];   /* Pathname to use for logfile */
    int     ipc_len;                        /* Length of returned string */

    Ipc_Status_t  ipc_status;       /* Status from IPC calls */

/* gtri - end - 12/13/90 */


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

/* gtri - add - 12/13/90 - wbk - ipc processing */

    /* Check to see if we are using IPC */
    if(strcmp(av[1],"-ipc") == 0) {

        /* Mark that IPC is enabled */
        g_ipc.enabled = IPC_TRUE;

        /* Make sure there are at least two args to the -ipc switch */
        if(ac < 4) {
            printf("\nERROR - Too few arguments to -ipc switch\n");
            printf("\nUsage - atesse_xspice -ipc <mode> <ipc channel>\n");
            exit(1);
        }

        /* Get the args and store them */
        if(strcmp(av[2],"INTERACTIVE") == 0)
            g_ipc.mode = IPC_MODE_INTERACTIVE;
        else if(strcmp(av[2],"BATCH") == 0)
            g_ipc.mode = IPC_MODE_BATCH;
        else {
            printf("\nERROR - Bad mode type as first argument\n");
            printf("\nMust be INTERACTIVE or BATCH\n");
            exit(1);
        }

        if(strlen(av[3]) > IPC_MAX_PATH_LEN) {
            printf("\nERROR - IPC pathname exceeds %d characters\n",IPC_MAX_PATH_LEN);
            exit(1);
        }
        else
            strcpy(ipc_path, av[3]);

        /* Create the IPC transport server */
        ipc_status = ipc_initialize_server(ipc_path, g_ipc.mode, IPC_PROTOCOL_V1);
        if(ipc_status != IPC_STATUS_OK) {
            printf("\nERROR - Problems initializing IPC server\n");
            exit(1);
        }

        /* Reopen the stdout and stderr streams so we can ship stuff written */
        /* by prinf(), etc. over the mailbox */
        if ( ! freopen(IPC_STDERR_FILE_NAME, "w+", stderr)) {
            printf("\nERROR - Problems reopening stderr for w+\n");
            exit(1);
        }
        if ( ! freopen(IPC_STDOUT_FILE_NAME, "w+", stdout)) {
            printf("\nERROR - Problems reopening stdout for w+\n");
            exit(1);
        }

        /* Call inp_spsource() to read and process the deck */
        /* If errors occur, they will be flagged in g_ipc.syntax_error */
        inp_spsource(NULL, false, NULL);

        /* Call ft_dotsaves() to collect the .save cards if any */
        ft_dotsaves();

        /* Call ft_dorun() to run the analysis. */
        ft_dorun("/usr/tmp/rawspice.tmp");

        /* Call ft_cktcoms() to print a listing and accounting information */
        /* Argument "false" says to print full accounting information */
        ft_cktcoms(false);

        /* Send across everything written to stdout and stderr */
        ipc_send_std_files();

        /* Send the final status to the client */
        ipc_send_end();

        /* Close the IPC channel */
        ipc_terminate_server();

        exit(0);
    }
        

/* gtri - end - 12/13/90 */



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
    ft_dotsaves();
    ft_dorun(ft_rawfile);
    ft_cktcoms(false);

    fprintf(stderr, "Warning: .plot, etc cards ignored (rawfile only).\n");

    exit(EXIT_NORMAL);
}

/* Now some misc junk that we need to fake... */

bool cp_nocc = true;
bool cp_debug = false;
int cp_maxhistlength;
char cp_chars[128];
bool ft_parsedb = false;

/* ARGSUSED */ struct dvec *vec_fromplot(w, p) char *w; struct plot *p;
        { return (NULL); }
void vec_new() {}
void plot_new() {}
void plot_setcur() {}
void cp_pushcontrol() {}
void cp_popcontrol() {}
/* ARGSUSED */ char *cp_tildexpand(s) char *s; { return (s); }
/* ARGSUSED */ void cp_addkword(class, word) int class; char *word; {}
struct circ *ft_circuits, *ft_curckt;
/* ARGSUSED */ struct plot *plot_alloc(name) char *name; { return (NULL); }
struct plot *plot_cur = NULL;
/* ARGSUSED */ char *cp_kwswitch(c, t) int c; char *t; { return (NULL); }
/* ARGSUSED */ void plot_docoms(wl) wordlist *wl; {}
/* ARGSUSED */ bool ft_bpcheck(r, i) struct plot *r; int i; { return (true); }
/* ARGSUSED */ void cp_ccon(b) bool b; {}
/* ARGSUSED */ void gr_iplot(pl) struct plot *pl; {}
/* ARGSUSED */ void gr_end_iplot(pl) struct plot *pl; {}
/* ARGSUSED */ void gr_pmsg(text, more) char *text; bool more; {}
/* ARGSUSED */ int cp_evloop(string) char *string; { return (0); }
/* ARGSUSED */ void gr_clean(intr) bool intr; {}
void ft_trquery() {}
void out_init() {}
#ifndef LATTICE
/* PC defines out_printf to printf since vsprintf doesn't exist */
/* ARGSUSED */ /* VARARGS1 */ void out_printf(c) char *c; {}
#endif
/* ARGSUSED */ void out_send(s) char *s; {}
char out_pbuf[1];
/* ARGSUSED */ void ft_newcirc(ckt) struct circ *ckt; {}
/* ARGSUSED */ struct dvec *vec_get(word) char *word; { return (NULL); }

void cp_doquit() { exit(0); }
/* ARGSUSED */ void cp_usrvars(v1, v2) struct variable **v1, **v2; { return; }
/* ARGSUSED */ struct variable * cp_enqvar(word) char *word; { return (NULL); }
/* ARGSUSED */ void cp_ccom(w, b, e) wordlist *w; char *b; bool e; { return; }
/* ARGSUSED */ int cp_usrset(v, i) struct variable *v; bool i; {return (US_OK);}

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

static wordlist *gettoks();

/* Extract all the .save cards... */

void
ft_dotsaves()
{
    wordlist *com, *wl = NULL;
    char *s;

    if (!ft_curckt) /* Shouldn't happen. */
        return;

    for (com = ft_curckt->ci_commands; com; com = com->wl_next) {
        if (ciprefix(".save", com->wl_word)) {
            s = com->wl_word;
            (void) gettok(&s);
            wl = wl_append(wl, gettoks(s));
        }
    }
/*  wrd_saves = wl; */

    com_save(wl);
/* fprintf(stderr, "error, .save card code is broken\n"); */
    return;
}

/* This is also from dotcards.c.  We should probably include the file. */
/* Stripped down version. */

/* Execute the .whatever cards found in the deck, after we are done running.
 * We'll be cheap and use cp_lexer to get the words... This should make us
 * spice-2 compatible.  Is terse is true then there was a rawfile, so don't
 * print lots of junk.
 */

void
ft_cktcoms(terse)
    bool terse;
{
    wordlist *coms, *command;
    char *plottype, *s;
    struct dvec *v;
    static wordlist twl = { "col", NULL, NULL } ;
    int i;

    if (!ft_curckt)
        return;
    if (!ft_curckt->ci_commands)
    goto nocmds;
    coms = ft_curckt->ci_commands;
    cp_interactive = false;

nocmds:
    /* Circuit name... */
    fprintf(cp_out, "Circuit: %s\nDate: %s\n\n", ft_curckt->ci_name,
            datestring());
    fprintf(cp_out, "\n");

/* gtri - modify - 12/24/90 - wbk - if IPC enabled, do a listing */

    if(g_ipc.enabled) {
        /* Print listings of input deck */
        printf("\n\n****************** INPUT DECK **********************\n\n");
        inp_list(stdout, ft_curckt->ci_origdeck, ft_curckt->ci_options,
                LS_DECK);
        printf("\n\n***************** EXPANDED DECK ********************\n\n");
        inp_list(stdout, ft_curckt->ci_deck, ft_curckt->ci_options,
                LS_LOGICAL);
        printf("\n\n****************************************************\n\n");
    }
    else {  /* original code */

      /* Listing if necessary... */
      if (ft_listprint)
        inp_list(cp_out, ft_curckt->ci_deck, ft_curckt->ci_options,
                LS_DECK);
    }

/* gtri - end - 12/24/90 */

    /* And finally the accounting info. */
    if (ft_acctprint) {
        static wordlist ww = { "everything", NULL, NULL } ;
        com_rusage(&ww);
    } else
        com_rusage((wordlist *) NULL);

    (void) putc('\n', cp_out);
    return;

bad:    fprintf(cp_err, "Internal Error: ft_cktcoms: bad commands\n");
    return;
}

int
ft_getSaves(savesp)
    char ***savesp;
{
    return (0);
}

static wordlist *
gettoks(s)
    char *s;
{
    char *t, *r, buf[64];
    wordlist *wl = NULL, *end = NULL;
    bool iflag;

    while (t = gettok(&s)) {
        if (*t == '(' /* ) */) {
            /* This is a (upper, lower) thing -- ignore. */
            continue;
        } else if (!index(t, '(' /*)*/ )) {
            if (end) {
                end->wl_next = alloc(wordlist);
                end->wl_next->wl_prev = end;
                end = end->wl_next;
            } else
                wl = end = alloc(wordlist);
            end->wl_word = copy(t);
        } else if (!index(t, ',')) {
            iflag = ((*t == 'i') || (*t == 'I')) ? true : false;
            while (*t != '(' /*)*/)
                t++;
            t++;
            for (r = t; *r && *r != /*(*/ ')'; r++)
                ;
            *r = '\0';
            if (end) {
                end->wl_next = alloc(wordlist);
                end->wl_next->wl_prev = end;
                end = end->wl_next;
            } else
                wl = end = alloc(wordlist);
            if (iflag) {
                (void) sprintf(buf, "%s#branch", t);
                t = buf;
            }
            end->wl_word = copy(t);
        } else {
            /* The painful case... */
            while (*t != '(' /*)*/)
                t++;
            t++;
            for (r = t; *r && *r != ','; r++)
                ;
            *r = '\0';
            if (end) {
                end->wl_next = alloc(wordlist);
                end->wl_next->wl_prev = end;
                end = end->wl_next;
            } else
                wl = end = alloc(wordlist);
            end->wl_word = copy(t);
            t = r + 1;
            for (r = t; *r && *r != /*(*/ ')'; r++)
                ;
            *r = '\0';
            if (end) {
                end->wl_next = alloc(wordlist);
                end->wl_next->wl_prev = end;
                end = end->wl_next;
            } else
                wl = end = alloc(wordlist);
            end->wl_word = copy(t);
        }
    }
    return (wl);
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

