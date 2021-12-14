
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:04:24 $
 *           $Source: //pepper/atesse_spice/spice3/CP/RCS/quote.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Various things for quoting words. If this is not ascii, quote and
 * strip are no-ops, so '' and \ quoting won't work. To fix this, sell
 * your IBM machine and buy a vax.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "suffix.h"

/* Strip all the 8th bits from a string (destructively). */

void
cp_wstrip(str)
    char *str;
{
    while (*str) {
        *str = strip(*str);
        str++;
    }
    return;
}

/* Quote all characters in a word. */

void
cp_quoteword(str)
    char *str;
{
    while (*str) {
        *str = quote(*str);
        str++;
    }
    return;
}

/* Print a word (strip the word first). */

void
cp_printword(string, fp)
    char *string;
    FILE *fp;
{
    char *s;

    if (string)
        for (s = string; *s; s++)
            (void) putc((strip(*s)), fp);
    return;
}

/* (Destructively) strip all the words in a wlist. */

void
cp_striplist(wlist)
    wordlist *wlist;
{
    wordlist *wl;

    for (wl = wlist; wl; wl = wl->wl_next)
        cp_wstrip(wl->wl_word);
    return;
}

/* Remove the "" from a string. */

char *
cp_unquote(string)
    char *string;
{
    char *s = copy(string);
    int l;

    if (*s == '"')
        s++;

    l = strlen(s) - 1;
    if (s[l] == '"')
        s[l] = '\0';
    return (s);
}

