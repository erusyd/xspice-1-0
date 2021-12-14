
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:04:28 $
 *           $Source: //pepper/atesse_spice/spice3/CP/RCS/tsh.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * A test shell main routine for use with cshpar.
 * Defined:
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEhelp.h"
#include "suffix.h"

char *cp_program = "tsh";
bool ft_nutmeg;
int disptype = 0;

/* ARGSUSED */
int
main(ac, av)
    char **av;
{
    cp_init();
    cp_ccon(true);
    cp_inp_cur = stdin;
    if (ac == 1) {
        cp_dounixcom = true;
        cp_rehash(getenv("PATH"), true);
    }
    while (cp_evloop((char *) NULL) == 1)
        ;
    exit (0);
    return(0);  /* drat */
}

/* ARGSUSED */
int
cp_usrset(v, i)
    struct variable *v;
    bool i;
{
    return (US_OK);
}

/* ARGSUSED */
void
cp_usrvars(v1, v2)
    struct variable **v1, **v2;
{
    return;
}

double *
ft_numparse(s, whole)
    char **s;
    bool whole;
{
    int i = 0;
    char **x = s;
    static double d;

    while (isdigit(**s))
        i = i * 10 + **s - '0';
    if (**s && whole) {
        s = x;
        return (NULL);
    }
    d = i;
    return (&d);
}

void
fatal()
{
    fprintf(stderr, "Fatal error\n");
    (void) kill(getpid(), 3);
}

void
cp_doquit()
{
    printf("bye\n");
    exit (0);
}

/* ARGSUSED */
struct variable *
cp_enqvar(s)
    char *s;
{
    return (NULL);
}

bool
cp_istrue(wl)
    wordlist *wl;
{
    return (wl->wl_next ? true : false);
}

/* ARGSUSED */
bool
cp_oddcomm(s, wl)
    char *s;
    wordlist *wl;
{
    return (false);
}

/* ARGSUSED */
void
com_foo(wl)
    wordlist *wl;
{
    printf(">> foo <<\n");
    return;
}

void
cp_periodic()
{
    return;
}

struct comm cp_coms[] = {

    { "foo", com_foo, false, false, true,
      { 0, 0, 0, 0 }, E_DEFHMASK, 0, 1, (int (*)()) NULL,
      "nothing" } ,

    { "echo", com_echo, false, false, true,
      { 0, 0, 0, 0 }, E_DEFHMASK, 0, LOTS, (int (*)()) NULL,
      "nothing" } ,

    { "", NULL, false, false, false,
      { 0, 0, 0, 0 }, E_DEFHMASK, 0, LOTS, (int (*)()) NULL,
      NULL }

} ;

