
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:11:59 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/nutmeg.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * The main routine for spice 3 and nutmeg.
 */

#include "prefix.h"
#include "IFsim.h"
#include "INPdefs.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "CPdefs.h"
#include "FTEdefs.h"
#ifndef CMS
#include "FTEredirect.h"
#else  /* CMS */
#include "FTEredir.h"
#endif /* CMS */
#include <setjmp.h>
#ifdef UNIX
#include <signal.h>
#include <pwd.h>
#endif
#ifdef MENU
#include "menu.h"
#endif
#ifdef IBMPC			/* really should be ndef BSD, clear up later */
#include <sys/types.h>
#include <sys/timeb.h>
#endif
#include "suffix.h"

char *errRtn;
char *errMsg;
bool ft_nutmeg;
char *cp_program;
#ifdef UNIX
char *ft_rawfile = "rawspice";
#else
char *ft_rawfile = "rawspice.raw";
#endif
bool ft_servermode = false;
bool ft_batchmode = false;
IFsimulator *ft_sim;

#ifdef MENU
bool menumode=false;
int disptype;
#endif
int menuxpos, menuypos;
struct options *exitoption;
struct options *helpoption;

jmp_buf jbuf;
static char *usage = 
"Usage: %s [-] [-b] [-i] [-s] [-n] [-o outfile] [-r rawfile]\n\
\t[-t term] [file ...]\n";
static started = false;

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

#ifndef OPTCHAR
#define OPTCHAR '-'
#endif

#ifdef IBMPC
int _stack = 8000;
extern struct timeb timebegin;		/* for use w/ ftime */
#endif

extern int ft_sigintr(), sigfloat(), sigstop(), sigquit();
extern int sigill(), sigbus(), sigsegv(), sig_sys();

void
main(ac, av)
    char **av;
{
#ifdef UNIX
    struct passwd *pw;
#endif
    char buf[BSIZE], **tv;
    bool gotone = false, readinit = true, rflag = false, ciprefix();
    bool istty = true, iflag = false, qflag = false;
    bool gdata = true;
    int err;
    int tc, i;
    FILE *fp;

extern bool if_isnutmeg;

    /* MFB tends to jump to 0 on errors... This will catch it. */
    if (started) {
        fprintf(cp_err, "main: Internal Error: jump to zero\n");
        fatal();
    }
    started = true;

#ifdef MALLOCTRACE
    mallocTraceInit("malloc.out");
#endif

    cp_in = stdin;
    cp_out = stdout;
    cp_err = stderr;

#ifdef UNIX
    istty = (bool) isatty(fileno(stdin));
#endif

    /* we don't need this anymore because of the IFsimulator structure */
    /*cp_program = av[0];*/

    /* Let's decide if we're nutmeg or not... This has to be done properly
     * as soon as we have the new sim interface working...
     */
    ft_nutmeg = if_isnutmeg;

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

                case '\0':  /* No raw file. */
                gdata = false;
                break;

                case 'b':   /* Batch mode. */
                case 'B':
                ft_batchmode = true;
                break;

                case 's':   /* Server mode. */
                case 'S':
                ft_servermode = true;
                break;

                case 'i':   /* Interactive mode. */
                case 'I':
                iflag = true;
                break;

#ifdef MENU
                case 'm':   /* Menu mode */
                menumode = true;
                break;

                case 'M':
                menumode = true;
                if (tc > 1) {
                    --tc;
                    tv++ ;
                    cp_vset("menufile", VT_STRING, *tv);
                }
                break;
#endif

                case 'q':   /* No command completion. */
                case 'Q':
                qflag = true;
                break;

                case 'n':   /* Don't read .spiceinit. */
                case 'N':
                readinit = false;
                break;

                case 'r':   /* The rawfile. */
                case 'R':
                if (tc > 1) {
                    tc--;
                    tv++;
                    cp_vset("rawfile", VT_STRING,
                            *tv);
                    **tv = OPTCHAR;
                }
                rflag = true;
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
                    **tv = OPTCHAR;
                } else {
                    fprintf(cp_err, usage, 
                        av[0]);
                    exit(EXIT_BAD);
                }
                break;

                case 't':   /* Terminal type. */
                case 'T':
                if (tc > 1) {
                    tc--;
                    tv++;
                    cp_vset("term", VT_STRING, *tv);
                    **tv = OPTCHAR;
                } else {
                    fprintf(cp_err, usage, 
                        cp_program);
                    exit(EXIT_BAD);
                }
                break;

                default:
                fprintf(cp_err, "Error: bad option %s\n", *tv);
                fprintf(cp_err, usage, cp_program);
                exit(EXIT_BAD);
            }
    }

    if ((!iflag && !istty) || ft_servermode)
        ft_batchmode = true;
    if ((iflag && !istty) || qflag)
        cp_nocc = true;
    if (ft_servermode)
        readinit = false;
    if (!istty || ft_batchmode)
        out_moremode = false;

    /* Have to initialize cp now. */
    ft_cpinit();

    /* initialize display */
    DevInit();

#ifdef MENU

    /* initialize menu display */
    disptype = gi_package;
    if (menumode && !ft_batchmode) {
      /* send terminal into graphics mode */
#ifdef GI_MFB
      if (disptype == G_MFB) {
        mi_init();
      }
#endif
#ifdef GI_Xt
      if (disptype == G_X) {
        mu_initx();
      }
#endif
    }

#endif

    /* Pass 2 -- get the filenames. If we are spice, then this means
     * build a circuit for this file. If this is in server mode, don't
     * process any of these args.
     */
    if (setjmp(jbuf) == 1)
        goto bot;
    cp_interactive = false;
    if (!ft_servermode && !ft_nutmeg) {
        FILE *file = NULL, *tp = NULL;
        char *tempfile = NULL, buf[BSIZE], *mktemp();

        for (tv = av + 1, i = 0; *tv; tv++)
            if (**tv != OPTCHAR)
                i++;
        if (i == 1) {
            for (tv = av + 1, i = 0; *tv; tv++)
                if (**tv != OPTCHAR)
                    break;
            if (!(file = fopen(*tv, "r"))) {
                perror(*tv);
                i = 0;
            }
        } else if (i) {
            tempfile = mktemp("/tmp/spXXXXXX");
            if (!(file = fopen(tempfile, "w+"))) {
                perror(tempfile);
                exit(EXIT_BAD);
            }
            for (tv = av + 1, i = 0; *tv; tv++)
                if (**tv != OPTCHAR) {
                    if (!(tp = fopen(*tv, "r"))) {
                        perror(*tv);
                        continue;
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
    } else if (ft_nutmeg && gdata) {
        /* Read in the rawfiles... */
        for (av++; *av; av++)
            if (**av != OPTCHAR) {
                ft_loadfile(*av);
                gotone = true;
            }
        if (!gotone)
            ft_loadfile(ft_rawfile);
    }
    if (!gotone && ft_batchmode && !ft_nutmeg)
        inp_spsource(stdin, false, (char *) NULL);

    /* To catch interrupts during .spiceinit... */
bot:    if (setjmp(jbuf) == 1) {
        fprintf(cp_err, "Warning: error executing .spiceinit.\n");
        if (!ft_batchmode)
            goto evl;
    }

#ifdef UNIX
    /* Set up (void) signal handling... */
    if (!ft_batchmode) {
        (void) signal(SIGINT, ft_sigintr);
        (void) signal(SIGFPE, sigfloat);
#ifdef BSD
        (void) signal(SIGTSTP, sigstop);
#endif
    }
    /* Set up (void) signal handling for fatal errors. */
    (void) signal(SIGILL, sigill);
    (void) signal(SIGBUS, sigbus);
    (void) signal(SIGSEGV, sigsegv);
    (void) signal(SIGSYS, sig_sys);

    if (readinit) {
        /* Try to source either .spiceinit or ~/.spiceinit. */
        if (access(".spiceinit", 0) == 0)
            inp_source(".spiceinit");
        else {
            pw = getpwuid(getuid());
            (void) strcpy(buf, pw->pw_dir);
            (void) strcat(buf, "/.spiceinit");
            if (access(buf, 0) == 0)
                inp_source(buf);
        }
    }
#else
    /* I guess we should just try to source the file "spice.rc" in
     * the current directory...
     */
    if (readinit) {
        if ((fp = fopen("spice.rc", "r")) != NULL) {
            (void) fclose(fp);
            inp_source("spice.rc");
        }
    }
#endif

#ifdef NEWSFILE
    if (!ft_batchmode) {
        fp = fopen(cp_tildexpand(NEWSFILE), "r");
        if (fp) {
            while (fgets(buf, BSIZE, fp))
                fputs(buf, stdout);
            (void) fclose(fp);
        }
    }
#endif

evl:
    if (ft_batchmode) {
        /* If we get back here in batch mode then something is
         * wrong, so exit.
         */
        bool st = false;

        (void) setjmp(jbuf);
        if (st == true)
            exit(EXIT_BAD);
        st = true;
        if (ft_servermode) {
            if (ft_curckt == NULL) {
                fprintf(cp_err, "Error: no circuit loaded!\n");
                exit(EXIT_BAD);
            }
            ft_dorun("");
            exit(EXIT_NORMAL);
        }
        /* If -r is specified, then we don't bother with the dot
         * cards. Otherwise, we use wrd_run, but we are careful
         * not to save too much.
         */
        cp_interactive = false;
        if (rflag) {
            ft_dotsaves();
            ft_dorun(ft_rawfile);
            ft_cktcoms(true);
        } else {
            ft_savedotargs();
            ft_dorun((char *) NULL);
            ft_cktcoms(false);
        }
    } else {
        (void) setjmp(jbuf);
        cp_interactive = true;

#ifdef MENU
        if (menumode) {
          /* where the menu goes */
          menuxpos = gi_fntwidth * 5;
          menuypos = gi_fntheight * 4;
          /* initialize help and exit options */
          helpoption = (struct options *)
                    calloc(1, sizeof(*helpoption));
          exitoption = (struct options *)
                    calloc(1, sizeof(*exitoption));
          helpoption->label = "help";
          exitoption->label = "exit";

          if (initscanner("basic.menu")) {
            exit(1);
          }
          if (parse()) {
            exit(1);
          }
          second();
          stackinit();
          driver();
          (void) endusefulness();
        } else
#endif
          while (cp_evloop((char *) NULL) == 1) ;

    }
    exit(EXIT_NORMAL);
}

#ifdef MENU

endusefulness()
{

#ifdef GI_MFB
    if (disptype == G_MFB) {
      mi_close(NULL);
    }
#endif
    exit (EXIT_NORMAL);

}

#endif
