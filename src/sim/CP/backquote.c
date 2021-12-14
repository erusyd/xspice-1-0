
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:04:08 $
 *           $Source: //pepper/atesse_spice/spice3/CP/RCS/backquote.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Do backquote substitution on a word list. 
 */

#include "prefix.h"
#include "CPdefs.h"
#include "suffix.h"

static wordlist *backeval();

char cp_back = '`';

wordlist *
cp_bquote(wlist)
    wordlist *wlist;
{
    wordlist *wl, *nwl;
    char *s, *t, buf[BSIZE], wbuf[BSIZE];
    int i;

    for (wl = wlist; wl; wl = wl->wl_next) {
        t = wl->wl_word;
        if (!t)
            continue;
        i = 0;
loop:       s = index(t, cp_back);
        if (s == NULL)
            continue;
        while (t < s)
            wbuf[i++] = *t++;
        wbuf[i] = '\0';
        (void) strcpy(buf, ++s);
        s = buf;
        t++;
        while (*s && (*s != cp_back)) {
            t++;    /* Get s and t past the next backquote. */
            s++;
        }
        /* What the heck, let "echo `foo" work... */
        *s = '\0';
        t++;    /* Get past the second ` */
        if (!(nwl = backeval(buf))) {
            wlist->wl_word = NULL;
            return (wlist);
        }
        if (i) {
            (void) strcpy(buf, wbuf);
            (void) strcat(buf, nwl->wl_word);
            tfree(nwl->wl_word);
            nwl->wl_word = copy(buf);
        }
        wl = wl_splice(wl, nwl);
        (void) strcpy(buf, wl->wl_word);
        i = strlen(buf);
        (void) strcat(buf, t);
        tfree(wl->wl_word);
        wl->wl_word = copy(buf);
        t = &wl->wl_word[i];
        s = wl->wl_word;
        for (i = 0; s < t; s++)
            wbuf[i++] = *s;
        goto loop;
    }
    return (wlist);
}

/* Do a popen with the string, and then reset the file pointers so that
 * we can use the first pass of the parser on the output.
 */

static wordlist *
backeval(string)
    char *string;
{
#ifdef UNIX
    FILE *proc, *old;
    wordlist *wl;
    bool intv;

    proc = popen(string, "r");
    if (proc == NULL) {
        fprintf(cp_err, "Error: can't evaluate %s.\n", string);
        return (NULL);
    }
    old = cp_inp_cur;
    cp_inp_cur = proc;
    intv = cp_interactive;
    cp_interactive = false;
    cp_bqflag = true;
    wl = cp_lexer((char *) NULL);
    cp_bqflag = false;
    cp_inp_cur = old;
    cp_interactive = intv;
    (void) pclose(proc);
    return (wl);
#else
    wordlist *wl = alloc(wordlist);

    wl->wl_word = copy(string);
    return (wl);
#endif
}

