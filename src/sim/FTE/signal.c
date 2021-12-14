
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:12:25 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/signal.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * The signal routines for spice 3 and nutmeg.
 */

#include "prefix.h"
#include "IFsim.h"
#ifndef CMS
#include "IFerrmsgs.h"
#else  /* CMS */
#include "IFerrmsg.h"
#endif /* CMS */
#include "CPdefs.h"
#include "FTEdefs.h"
#include <setjmp.h>
#ifdef UNIX
#include <signal.h>
#include <pwd.h>
#endif
#include "suffix.h"

#ifdef UNIX

extern jmp_buf jbuf;

/* The (void) signal handlers... SIGINT is the only one that gets reset (by
 * cshpar) so it is global. They are ifdef BSD because of the sigmask
 * stuff in sigstop. We set the interrupt flag and return if ft_setflag
 * is true.
 */

int
ft_sigintr()
{

    gr_clean();
    if (ft_intrpt)
        fprintf(cp_err, "Interrupt (ouch)\n");
    else {
        fprintf(cp_err, "Interrupt\n");
        ft_intrpt = true;
    }
    if (ft_setflag)
        return;
    cp_interactive = true;
    cp_resetcontrol();
    longjmp(jbuf, 1);
}

/* ARGSUSED */
int
sigfloat(sig, code)
{
    gr_clean();
    fperror("Error", code);
    rewind(cp_out);
    longjmp(jbuf, 1);
}

/* This should give a new prompt if cshpar is waiting for input.  */

#ifdef BSD

#define SIGMASK(s)  (1 << ((s) - 1))

int
sigstop()
{
    int omask;      /* Old (void) signal mask. */

    gr_clean();
    cp_ccon(false);
    omask = sigsetmask(~SIGMASK(SIGTSTP));
    (void) signal(SIGTSTP, SIG_DFL);
    (void) kill(getpid(), SIGTSTP); /* This should stop us */

    (void) signal(SIGTSTP, sigstop);
    (void) sigsetmask(omask);
    if (cp_cwait)
        longjmp(jbuf, 1);
    return;
}

#endif

/* Special (void) signal handlers. */

int
sigill()
{
    fprintf(cp_err, "\ninternal error -- illegal instruction\n");
    fatal();
}

int
sigbus()
{
    fprintf(cp_err, "\ninternal error -- bus error\n");
    fatal();
}

int
sigsegv()
{
    fprintf(cp_err, "\ninternal error -- segmentation violation\n");
    fatal();
}

int
sig_sys()
{
    fprintf(cp_err, 
        "\ninternal error -- bad argument to system call\n");
    fatal();
}

#endif

