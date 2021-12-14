
/* RCS Info: $Revision: 1.2 $ on $Date: 92/07/29 19:48:08 $
 *           $Source: //carob/xspice/src/sim/FTE/RCS/misccoms.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Assorted commands.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"
#include "FTEhelp.h"
#include "version.h"
#ifdef UNIX
#include <pwd.h>
#endif
#ifdef BSD
#include <sys/time.h>
#include <sys/wait.h>
#endif
#include "HLPdefs.h"
#include "suffix.h"

static void byemesg();

#ifndef IBMPC
#define HELPMSG \
"For a complete description of %s read the %s manual.\n\
There are short introductions to both Spice 3 and Nutmeg available, in\n\
addition to the SPICE User's Guide and the manual pages.  If you don't\n\
have these documents ask your system manager for them.  Below is a short\n\
list of useful commands -- for a list of all commands type \"help all\",\n\
and for a short description of \"command\", type \"help command\".\n\n"
#endif

#define SHORTHELPMSG \
"For a complete description of %s read the %s manual.\n\n"

void
com_help(wl)
    wordlist *wl;
{
    struct comm *c;
    struct comm *ccc[512];  /* Should be enough. */
    int numcoms, i;
    bool allflag = false;

    if (wl && eq(wl->wl_word, "all")) {
        allflag = true;
        wl = NULL;  /* Probably the right thing to do... */
    }

    /* We want to use more mode whether "moremode" is set or not. */
    out_moremode = true;
    out_init();
    out_moremode = false;
    if (wl == NULL) {
#ifdef IBMPC
        out_printf(SHORTHELPMSG,
            cp_program, cp_program);
#else
        out_printf((allflag ? SHORTHELPMSG : HELPMSG),
            cp_program, cp_program);
#endif

        /* Sort the commands... */
        for (numcoms = 0; cp_coms[numcoms].co_func != NULL; numcoms++)
            ccc[numcoms] = &cp_coms[numcoms];
        qsort((char *) ccc, numcoms, sizeof (struct comm *), hcomp);

        for (i = 0; i < numcoms; i++) {
            if ((ccc[i]->co_spiceonly && ft_nutmeg) || 
                    (ccc[i]->co_help == NULL) || 
                    (!allflag && !ccc[i]->co_major))
                continue;
            out_printf("%s ", ccc[i]->co_comname);
            out_printf(ccc[i]->co_help, cp_program);
            out_send("\n");
        }
    } else {
        while (wl != NULL) {
            for (c = &cp_coms[0]; c->co_func != NULL; c++)
                if (eq(wl->wl_word, c->co_comname)) {
                    out_printf("%s ", c->co_comname);
                    out_printf(c->co_help, cp_program);
                    if (c->co_spiceonly && ft_nutmeg)
                        out_send(
                        " (Not available in nutmeg)");
                    out_send("\n");
                    break;
                }
            if (c->co_func == NULL) {
                /* See if this is aliased. */
                struct alias *al;

                for (al = cp_aliases; al; al = al->al_next)
                    if (eq(al->al_name, wl->wl_word))
                        break;
                if (al == NULL)
                    fprintf(cp_out, 
                        "Sorry, no help for %s.\n", 
                        wl->wl_word);
                else {
                    out_printf("%s is aliased to ",
                        wl->wl_word);
                    /* Minor badness here... */
                    wl_print(al->al_text, cp_out);
                    out_send("\n");
                }
            }
            wl = wl->wl_next;
        }
    }
    out_send("\n");
    return;
}

/* dumb C compiler can't handle strings longer than 256 characters */
#define AHELPMESG1 \
"For a complete description of %s read the %s manual.\n\
There are short introductions to both Spice 3 and Nutmeg available, in\n\
addition to the SPICE User's Guide and the manual pages.  If you don't\n"
#define AHELPMESG2 \
"have these documents ask your system manager for them.  Below is a short\n\
list of useful commands -- for a list of all commands type \"help all\",\n\
and for a short description of \"command\", type \"help command\".\n"
#define AHELPMESG3 \
"There is also an on-line tutorial available by typing \"tutorial\"\n\
or \"tutorial subject\"\n\n"

void
com_ahelp(wl)
wordlist *wl;
{

    int i, n;
    /* assert: number of commands must be less than 512 */
    struct comm *cc[512];
    int env = 0;
    struct comm *com;
    int level;
    char slevel[256];

    if (wl) {
      com_help(wl);
      return;
    }

    out_init();

    /* determine environment */
    if (plot_list->pl_next) {   /* plots load */
      env |= E_HASPLOTS;
    } else {
      env |= E_NOPLOTS;
    }

/* note: XXX */

#ifdef notdef
#ifdef GI_X
    if (screens) {          /* have graphs on screen */
      env |= E_HASGRAPHS;
    }
#endif
    if (menumode) {
      env |= E_MENUMODE;
    }
#endif

    /* determine level */
    if (cp_getvar("level", VT_STRING, slevel)) {
      switch (*slevel) {
        case 'b':   level = 1;
            break;
        case 'i':   level = 2;
            break;
        case 'a':   level = 4;
            break;
        default:    level = 1;
            break;
      }
    } else {
      level = 1;
    }

    /* dumb C compiler can't handle strings longer than 256 characters */
    out_printf(AHELPMESG1, cp_program, cp_program);
    out_printf(AHELPMESG2, cp_program, cp_program);
    out_printf(AHELPMESG3, cp_program, cp_program);

    /* sort the commands */
    for (n = 0; cp_coms[n].co_func != (void (*)()) NULL; n++) {
      cc[n] = &cp_coms[n];
    }
    qsort((char *) cc, n, sizeof(struct comm *), hcomp);

    /* filter the commands */
    for (i=0; i< n; i++) {
      com = cc[i];
      if ((com->co_env < (level << 13)) && (!(com->co_env & 4095) ||
        (env & com->co_env))) {
        if ((com->co_spiceonly && ft_nutmeg) ||
        (com->co_help == (char *) NULL)) {
          continue;
        }
        out_printf("%s ", com->co_comname);
        out_printf(com->co_help, cp_program);
        out_send("\n");
      }
    }

    out_send("\n");

    return;

}

#ifdef HELPPATH

void
com_ghelp(wl)
    wordlist *wl;
{
    char *path = HELPPATH, buf[BSIZE];
    int i;

    if (cp_getvar("helppath", VT_STRING, buf))
        path = copy(buf);
    if (!(path = cp_tildexpand(path))) {
        fprintf(cp_err, "Error: can't find help dir %s\n", HELPPATH);
        fprintf(cp_err, "Defaulting to old help...\n\n");
        com_help(wl);
        return;
    }
    if (cp_getvar("helpregfont", VT_STRING, buf))
        hlp_regfontname = copy(buf);
    if (cp_getvar("helpboldfont", VT_STRING, buf))
        hlp_boldfontname = copy(buf);
    if (cp_getvar("helpitalicfont", VT_STRING, buf))
        hlp_italicfontname = copy(buf);
    if (cp_getvar("helptitlefont", VT_STRING, buf))
        hlp_titlefontname = copy(buf);
    if (cp_getvar("helpbuttonfont", VT_STRING, buf))
        hlp_buttonfontname = copy(buf);
    if (cp_getvar("helpinitxpos", VT_NUM, (char *) &i))
        hlp_initxpos = i;
    if (cp_getvar("helpinitypos", VT_NUM, (char *) &i))
        hlp_initypos = i;
    if (cp_getvar("helpbuttonstyle", VT_STRING, buf)) {
        if (cieq(buf, "left"))
            hlp_buttonstyle = BS_LEFT;
        else if (cieq(buf, "center"))
            hlp_buttonstyle = BS_CENTER;
        else if (cieq(buf, "unif"))
            hlp_buttonstyle = BS_UNIF;
        else
            fprintf(cp_err, "Warning: no such button style %s\n",
                    buf);
    }
    if (cp_getvar("width", VT_NUM, (char *) &i))
        hlp_width = i;
    if (cp_getvar("display", VT_STRING, buf))
        hlp_displayname = copy(buf);
    else if (cp_getvar("device", VT_STRING, buf))
        hlp_displayname = copy(buf);
    else
        hlp_displayname = NULL;
    hlp_main(path, wl);
    return;
}

#else

void
com_ghelp(wl)
    wordlist *wl;
{
    com_help(wl);
    return;
}

#endif

int
hcomp(c1, c2)
    struct comm **c1, **c2;
{
    return (strcmp((*c1)->co_comname, (*c2)->co_comname));
}

/* ARGSUSED */
void
com_quit(wl)
    wordlist *wl;
{
    struct circ *cc;
    struct plot *pl;
    int ncc = 0, npl = 0;
    char buf[64];
    bool noask;

    (void) cp_getvar("noaskquit", VT_BOOL, (char *) &noask);
    gr_clean();
    cp_ccon(false);

/* gtri - begin - Eliminate query about wanting to quit or not */
#if 0
/* gtri - end - Eliminate query about wanting to quit or not */
    
    /* Make sure the guy really wants to quit. */
    if (!ft_nutmeg && !noask) {
        for (cc = ft_circuits; cc; cc = cc->ci_next)
            if (cc->ci_inprogress)
                ncc++;
        for (pl = plot_list; pl; pl = pl->pl_next)
            if (!pl->pl_written && pl->pl_dvecs)
                npl++;
        if (ncc || npl) {
            fprintf(cp_out, "Warning: ");
            if (ncc) {
                fprintf(cp_out, 
            "the following simulation%s still in progress:\n",
                        (ncc > 1) ? "s are" : " is");
                for (cc = ft_circuits; cc; cc = cc->ci_next)
                    if (cc->ci_inprogress)
                        fprintf(cp_out, "\t%s\n",
                                cc->ci_name);
            }
            if (npl) {
                if (ncc)
                    fprintf(cp_out, "and ");
                fprintf(cp_out, 
                "the following plot%s been saved:\n",
                    (npl > 1) ? "s haven't" : " hasn't");
                for (pl = plot_list; pl; pl = pl->pl_next)
                    if (!pl->pl_written && pl->pl_dvecs)
                        fprintf(cp_out, "%s\t%s, %s\n",
                                pl->pl_typename,
                                pl->pl_title,
                                pl->pl_name);
            }
            fprintf(cp_out, 
                "\nAre you sure you want to quit (yes)? ");
            (void) fflush(cp_out);
#ifdef HPUX
foo:
#endif
            if (!fgets(buf, BSIZE, stdin)) {
                clearerr(stdin);
#ifdef HPUX
                goto foo;
#else
                *buf = 'y';
#endif
            }
            if ((*buf == 'y') || (*buf == 'Y') || (*buf == '\n'))
                byemesg();
            else {
                fprintf(cp_out, "I didn't think so...\n");
                return;
            }
        } else
            byemesg();
    } else

/* gtri - begin - Eliminate query about wanting to quit or not */
#endif
/* gtri - end - Eliminate query about wanting to quit or not */

        byemesg();

    exit(EXIT_NORMAL);
    /* NOTREACHED */
}

#ifdef BUGADDR

#ifdef UNIX

/* ARGSUSED */
void
com_bug(wl)
    wordlist *wl;
{
    char buf[BSIZE];

    fprintf(cp_out, "Enter a description of the bug. End with ^D.\n");
    fprintf(cp_out, 
        "(This works only if you can mail directly to %s.)\n",
        BUGADDR);

    (void) sprintf(buf, "mail -s \"%s Bug Report\" %s", SPICE_VERSION,
            BUGADDR);
    (void) system(buf);
    fprintf(cp_out, "Bug report sent.  Thank you.\n");
    return;
}

#else

#ifdef VMS

/* ARGSUSED */
void
com_bug(wl)
    wordlist *wl;
{
    char buf[BSIZE];

    fprintf(cp_out, "Sending bug report to %s.\n", BUGADDR);
    (void) sprintf(buf, "mail/subject=\"%s Bug Report\" sys$command %s",
            SPICE_VERSION, BUGADDR);
    system(buf);
    fprintf(cp_out, "Bug report sent.  Thank you.\n");
    return;
}

#else

/* ARGSUSED */
void
com_bug(wl)
    wordlist *wl;
{
    fprintf(cp_out, "Send mail to the address %s-bugs@ucbcad.ARPA\n",
        ft_nutmeg ? "nutmeg" : "spice3");
    return;
}

#endif

#endif

#else

/* ARGSUSED */
void
com_bug(wl)
    wordlist *wl;
{
    fprintf(cp_out, "Send mail to the address %s-bugs@ucbcad.ARPA\n",
        ft_nutmeg ? "nutmeg" : "spice3");
    return;
}

#endif

void
com_version(wl)
    wordlist *wl;
{
    char *s;

    if (!wl)
        fprintf(cp_out, "Version: %s\n", SPICE_VERSION);
    else {
        s = wl_flatten(wl);
        if (!eq(SPICE_VERSION, s)) {
            fprintf(stderr,
        "Note: rawfile is version %s (current version is %s)\n",
                    wl->wl_word, SPICE_VERSION);
        }
        tfree(s);
    }
    return;
}

/* Oh well, I'd better not put in the "fun" code here... */

static void
byemesg()
{
    printf("So long.\n");
    return;
}

