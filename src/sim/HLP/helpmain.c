
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:03:04 $
 *           $Source: //pepper/atesse_spice/spice3/HLP/RCS/helpmain.c,v $
 * Copyright (c) 1986 Wayne A. Christopher, U. C. Berkeley CAD Group 
 *   faustus@cad.berkeley.edu, ucbvax!faustus
 * Permission is granted to modify and re-distribute this code in any manner
 * as long as this notice is preserved.  All standard disclaimers apply.
 *
 * The main routine for the help system in stand-alone mode.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "HLPdefs.h"
#include "suffix.h"

#ifdef GI_X
Display *X_display = (Display *) NULL;
#endif

#ifdef GI_X11
Widget toplevel;
#endif

FILE *cp_in, *cp_out, *cp_err;

/* dummy declaration so CP.a doesn't pull in lexical.o and other objects */
bool cp_interactive = false;

char *hlp_filelist[] = {"NUTMEG", "SPICE", ""};

/* ARGSUSED */
void
main(ac, av)
    int ac;
    char **av;
{
    wordlist *wl = NULL;

#ifdef GI_X11
    char *displayname;
    /* grrr, Xtk forced contortions */
    char *argv[2];
    int argc = 2;
    char buf[512];
#endif

    cp_in = stdin;
    cp_out = stdout;
    cp_err = stderr;

#ifdef GI_X11

    if (cp_getvar("display", VT_STRING, buf)) {
      displayname = buf;
    } else if (!(displayname = getenv("DISPLAY"))) {
      fprintf(stderr, "Can't open X display.");
      goto out;
    }

    argv[0] = "nutmeg";
    argv[1] = displayname;
    /* initialize X toolkit */
    toplevel = XtInitialize("nutmeg", "Nutmeg", NULL, 0, &argc, argv);

#endif

out:
    if (ac > 1)
        wl = wl_build(av + 1);
    hlp_main(DEFPATH, wl);

#ifdef GI_X11
    if (hlp_usex) {
	printf("Hit control-C when done.\n");		/* sigh */
	XtMainLoop();
    }
#endif

    exit(EXIT_NORMAL);
}

void
fatal(s)
    char *s;
{
    fprintf(stderr, "fatal error: %s\n", s);
    exit(1);
}

void
cp_printword(s)
    char *s;
{
    printf("%s", s);
    return;
}

/* ARGSUSED */
bool
cp_getvar(n, t, r)
    char *n, *r;
    int t;
{
    return (false);
}

