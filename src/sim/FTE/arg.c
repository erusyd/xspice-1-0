
/*
    $Header: //pepper/atesse_spice/spice3/FTE/RCS/arg.c,v 1.1 91/04/02 12:10:53 bill Exp $
    Copyright (c) 1987 Jeffrey M. Hsu

    This files contains the routines to evalute arguments to a command
      and prompt the user if necessary.
*/

#include "prefix.h"
#include "FTEinput.h"
#include "CPdefs.h"
#ifndef CMS
#include "FTEextern.h"
#else  /* CMS */
#include "FTEexter.h"
#endif /* CMS */
/*
#include "CPstd.h"
#include "CPstatic.h"
*/
#ifdef MENU
#include "menu.h"
#endif
#include "suffix.h"

#define MAXPROMPT 1024
static char buf[MAXPROMPT];

/* returns a private copy of the string */
/* ARGSUSED */ /* until some code gets added that needs the FILE* */
static char *prompt(fp)
FILE *fp;
{

    char *p;
    RESPONSE response;
    REQUEST request;

#ifdef notdef
    switch (disptype) {
      case G_X:
        input(&response, TEXT);
        if (response.option != OTHER) {
          return ((char *) -1);
        }
        return (response.text);
        break;
      case G_MFB:
      case G_HCOPY:
      case G_TERM:
      case G_NONE:
      default:
        (void) fgets(buf, MAXPROMPT, fp);
        /* get rid of last '\n' */
        p = buf;
        while (*p++ != '\n' && p < buf + MAXPROMPT) ;
        *--p = '\0';
        return ((char *) strcpy(malloc(strlen(buf)+1), buf));
        break;
    }
#endif

/* note: XXX rework this */
    request.option = char_option;
    p = buf;
    do {
      Input(&request, &response);
      *p++ = response.reply.ch;
    } while (response.reply.ch != '\n');
    *--p = '\0';
    return((char *) strcpy(malloc(strlen(buf)+1), buf));

}

countargs(wl)
wordlist *wl;
{

    int number=0;
    wordlist *w;

    for (w = wl; w; w = w->wl_next) {
      number++ ;
    }
    return(number);

}

wordlist *process(wlist)
wordlist *wlist;
{

    wlist = cp_variablesubst(wlist);
    wlist = cp_bquote(wlist);
    wlist = cp_doglob(wlist);
    return (wlist);

}

arg_print(wl, command)
wordlist *wl;
struct comm *command;
{

    common("which variable", wl, command);

}

arg_plot(wl, command)
wordlist *wl;
struct comm *command;
{

    common("which variable", wl, command);

}

arg_load(wl, command)
wordlist *wl;
struct comm *command;
{

#ifdef MENU
    if (menumode) {
      common("which raw file", wl, command);
    } else
#endif
      /* just call com_load */
      (*command->co_func) (wl);

}

arg_let(wl, command)
wordlist *wl;
struct comm *command;
{

    common("which vector", wl, command);

}

arg_set(wl, command)
wordlist *wl;
struct comm *command;
{

    common("which variable", wl, command);

}

arg_display()
{

    /* just return; display does the right thing */

}

/* a common prompt routine */
static common(string, wl, command)
char *string;
struct wordlist *wl;
struct comm *command;
{

    struct wordlist *w;
    char *buf;

    if (!countargs(wl)) {
      outmenuprompt(string);
      if ((buf = prompt(cp_in)) == (char *) -1) /* prompt aborted */
        return;               /* don't execute command */
      /* do something with the wordlist */
      w = alloc(wordlist);
      w->wl_word = buf;
      w->wl_next = NULL;

      w = process(w);
      /* O.K. now call fn */
      (*command->co_func) (w);
    }

}

outmenuprompt(string)
char *string;
{

#ifdef notdef
    char buf[1024];


    if (!menumode) {
      fprintf(cp_out, "%s: ", string);
      fflush(cp_out);
      return;
    }

    switch (disptype) {
      case G_X:
#ifdef GI_Xt
        sprintf(buf, "%s: ", string);
        PromptOut(buf);
#endif
        break;
      case G_MFB:
        sprintf(buf, "%s: ", string);
        gi_text(buf, 0, 0, FALSE, TRUE);
        gi_update();
        break;
      case G_TERM:
      case G_HCOPY:
      case G_NONE:
      default:
        fprintf(cp_out, "%s: ", string);
        fflush(cp_out);
        break;
    }

#endif

      fprintf(cp_out, "%s: ", string);
      fflush(cp_out);
      return;

}
