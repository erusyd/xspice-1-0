
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:03:01 $
 *           $Source: //pepper/atesse_spice/spice3/HLP/RCS/help.c,v $
 * Copyright (c) 1986 Wayne A. Christopher, U. C. Berkeley CAD Group 
 *   faustus@cad.berkeley.edu, ucbvax!faustus
 * Permission is granted to modify and re-distribute this code in any manner
 * as long as this notice is preserved.  All standard disclaimers apply.
 *
 * The main entry point for the help system.
 */

#include "prefix.h"
#include "CPstd.h"
#include "HLPdefs.h"
#include "suffix.h"

char *hlp_directory;
extern char *hlp_filelist[];
int hlp_ftablesize = 0;

void
hlp_main(path, wl)
    char *path;
    wordlist *wl;
{
    topic *top;
    fplace *place;

    hlp_directory = path;

    if (wl) {
        while (wl) {
            if (!(place = findglobalsubject(wl->wl_word))) {
                fprintf(stderr, "Error: No such topic: %s\n",
                    wl->wl_word);
                wl = wl->wl_next;
                continue;
            }
            if (!(top = hlp_read(place))) {
                fprintf(stderr, "Error: can't read topic\n");
                wl = wl->wl_next;
                continue;
            }
            hlp_provide(top);
            wl = wl->wl_next;
        }
    } else {
        if (!(place = findglobalsubject("main"))) {
          fprintf(stderr, "Error: no main topic\n");
          return;
        }
        if (!(top = hlp_read(place))) {
            fprintf(stderr, "Error: can't read topic\n");
            return;
        }
        hlp_provide(top);
    }

#ifndef GI_X11
    hlp_free();
#endif

    return;
}

fplace *
findglobalsubject(subject)
char *subject;
{

    fplace *place;
    char **dict;

    place = (fplace *) malloc(sizeof(fplace));

    for (dict = hlp_filelist; **dict; dict++) {
      place->fpos = findsubject(*dict, subject);
      if (place->fpos == -1) continue ;
      place->filename = copy(*dict);
      place->fp = hlp_fopen(*dict);
      return(place);
    }
    free(place);
    return(NULL);

}

/* see if file is on filelist */
bool
hlp_approvedfile(filename)
char *filename;
{
    char **s;

    for (s = hlp_filelist; **s; s++) {
      if (!strcmp(*s, filename)) return(true);
    }
    return(false);
}

/* keep file pointers on top level files so we don't always have to do
    fopen's */
FILE *hlp_fopen(filename)
char *filename;
{
    static struct {
        char filename[BSIZE];
        FILE *fp;
    } hlp_ftable[32];
    int i;
    char buf[BSIZE];

    for (i=0; i < hlp_ftablesize; i++) {
      if (!strcmp(filename, hlp_ftable[i].filename)) {
        return(hlp_ftable[i].fp);
      }
    }

    /* not already in table */
    strcpy(buf, hlp_directory); /* set up pathname */
    strcat(buf, "/");
    strcat(buf, filename);
    strcat(buf, ".txt");
    hlp_pathfix(buf);
    if (!(hlp_ftable[hlp_ftablesize].fp = fopen(buf, "r"))) {
      perror(buf);
      return (NULL);
    }

    strcpy(hlp_ftable[hlp_ftablesize].filename, filename);
    hlp_ftablesize++;

    return(hlp_ftable[hlp_ftablesize - 1].fp);

}

#ifdef UNIX

/* ARGSUSED */
void
hlp_pathfix(buf)
    char *buf;
{
    return;
}

#endif

#ifdef VMS

#ifdef notdef

/* I should probably use shell$to_vms for this...  This has to be sort of
 * tricky, since we might give it something like DEV:[FOO.BAR]/FILE.
 */

void
hlp_pathfix(buf)
    char *buf;
{
    char tmp[BSIZE], *s, *t;
    int i, j;
    bool devgiven = false;

    if (!index(buf, '/'))
        return;
    if (index(buf, '[')) {
        for (s = buf; *s; s++)
            if (*s == '/')
                *s = ' ';
        for (s = t = buf; *s; s++)
            if (*s != ' ')
                *t++ = *s;
        *t = '\0';
        return;
    }
    if (index(buf, ':')) {
        devgiven = true;
        j = 0;
    } else {
        tmp[0] = '[';
        j = 1;
    }
    for (i = 0; buf[i]; i++) {
        if (buf[i] == '/') {
            if (i && ((buf[i - 1] == ':') || (buf[i - 1] == '/')))
                ;
            else if (index(buf + i + 1, '/'))
                tmp[j++] = '.';
            else
                tmp[j++] = ']';
        } else if ((buf[i] == ':') && devgiven) {
            devgiven = false;
            tmp[j++] = ':';
            tmp[j++] = '[';
        } else
            tmp[j++] = buf[i];
    }
    tmp[j] = '\0';
    (void) strcpy(buf, tmp);
    return;
}

#endif

/* This is the corrected version. */

void
hlp_pathfix(buf)
    char *buf;
{
    int i, j, flag;
    char tmp[BSIZE];

    for (i = 0; buf[i]; i++)
        ;
    flag = 0;
    for (j = i + 1; i != -1; i--)
        switch (buf[i]) {
            case '/':
                if ((tmp[j] == '.') || (tmp[j] == ']'))
                    break;
                if (flag)
                    tmp[--j] = '.';
                else {
                    flag = 1;
                    tmp[--j] = ']';
                }
                break;

            case ']':
                if (flag)
                    break;
                else
                    return;

            case ':':
                tmp[j] = '[';
                /* FALLTHRU */

            default:
                tmp[--j] = buf[i];
                break;
        }
    
    (void) strcpy(buf, &tmp[j]);

    return;
}

#endif

#ifdef IBMPC

void
hlp_pathfix(buf)
    char *buf;
{
    while (*buf) {
        if (*buf == '/')
            *buf = '\\';
        buf++;
    }
    return;
}

#endif

