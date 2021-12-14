
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:04:23 $
 *           $Source: //pepper/atesse_spice/spice3/CP/RCS/output.c,v $
 * Copyright (c) 1986 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Routines to handle "more"d output.  There are some serious system
 * dependencies in here, and it isn't clear that versions of this stuff
 * can be written for every possible machine...
 */

#ifdef STANDALONE
#define DIRECT          /* no redirecting of output */
#endif

#include "prefix.h"
#include "CPdefs.h"
#ifndef CMS
#include "FTEredirect.h"
#else  /* CMS */
#include "FTEredir.h"
#endif /* CMS */
#ifdef BSD
#include <sys/ioctl.h>
#endif
#ifdef MENU
#ifdef GI_X
#include <X/Xlib.h>
#endif
#endif
#ifndef NOVARARGS
#include <varargs.h>
#endif
#include "suffix.h"

#ifndef STANDALONE
static bool menumode;
#endif

#define DEF_SCRHEIGHT   24
#define DEF_SCRWIDTH    80

bool out_moremode = true;
bool out_isatty = true;

static int xsize, ysize;
static int xpos, ypos;
static bool noprint, nopause;

/* out_printf doesn't handle double arguments correctly, so we
    sprintf into this buf and call out_send w/ it */
char out_pbuf[BSIZE];

/* Start output... */

void
out_init()
{
#ifdef TIOCGWINSZ
    struct winsize ws;
#endif
    char tbuf[4096], *s;
    bool moremode;

    noprint = nopause = false;

    if (cp_getvar("nomoremode", VT_BOOL, (char *) &moremode))
        out_moremode = false;
    else
    out_moremode = true;
    if (!out_moremode || !cp_interactive)
        out_isatty = false;

    if (!out_isatty)
        return;

    xsize = ysize = 0;

    /* Figure out the screen size.  We try, in order, TIOCGSIZE,
     * tgetent(), and cp_getvar(height).  Default is 24 x 80.
     */

#ifdef MENU
#ifndef STANDALONE
    if (menumode) {     /* hack! */
        xsize = 80;
        ysize = 24;
    }
#endif
#endif

#ifdef TIOCGWINSZ
    if (!xsize || !ysize) {
        (void) ioctl(fileno(stdout), TIOCGWINSZ, (char *) &ws);
        xsize = ws.ws_col;
        ysize = ws.ws_row;
    }
#endif

#ifdef UNIX
    if (!xsize || !ysize) {
        if ((s = getenv("TERM")) && (tgetent(tbuf, s) != -1)) {
            xsize = tgetnum("co");
            ysize = tgetnum("li");
            if ((xsize <= 0) || (ysize <= 0))
                xsize = ysize = 0;
        }
    }
#endif

    if (!xsize || !ysize) {
        (void) cp_getvar("height", VT_NUM, (char *) &ysize);
        (void) cp_getvar("width", VT_NUM, (char *) &xsize);
    }
    if (!xsize || !ysize) {
        xsize = DEF_SCRWIDTH;
        ysize = DEF_SCRHEIGHT;
    }
    ysize -= 2; /* Fudge room... */
    xpos = ypos = 0;

    return;
}

/* Putc may not be buffered (sp?), so we do it ourselves. */

static char staticbuf[BUFSIZ];
struct {
    int count;
    char *ptr;
} ourbuf = { BUFSIZ, staticbuf };

/* send buffer out */
void
outbufputc()
{

    if (ourbuf.count != BUFSIZ) {
      fputs(staticbuf, cp_out);
      bzero(staticbuf, BUFSIZ-ourbuf.count);
      ourbuf.count = BUFSIZ;
      ourbuf.ptr = staticbuf;
    }

}

#define bufputc(c)  ( --ourbuf.count >= 0 ? ((int) \
    (*ourbuf.ptr++ = (unsigned)(c))) : fbufputc((unsigned) (c)))

static int
fbufputc(c)
unsigned char c;
{

    ourbuf.count = 0;
    outbufputc();
    ourbuf.count = BUFSIZ;
    ourbuf.ptr = staticbuf;
    bufputc(c);

}

/* prompt for a return */
void
promptreturn()
{
    char buf[16];

#ifdef MENU
#ifndef STANDALONE
#ifdef GI_X
#ifdef GI_Xt

    XEvent ev;

    if (menumode) {
      mu_out("\n\t--- click any button to continue --- ");
/*    input(&reponse, CLICK); */
      do {          /* hack! */
        XNextEvent(&ev);
      } while (ev.type != ButtonReleased);
      return;
    }
#endif
#endif
#endif
#endif

moe:
    fprintf(cp_out,
        "\n\t-- hit return for more, ? for help -- ");
    if (!fgets(buf, 16, cp_in)) {
        clearerr(cp_in);
        *buf = 'q';
    }
    switch (*buf) {
        case '\n':
            break;
        case 'q':
            noprint = true;
            break;
        case 'c':
            nopause = true;
            break;
        case ' ':
            break;
        case '?':
            fprintf(cp_out,
"\nPossible responses:\n\
\t<cr>   : Print another screenful\n\
\tq <cr> : Discard the rest of the output\n\
\tc <cr> : Continuously print the rest of the output\n\
\t? <cr> : Print this help message\n");
            goto moe;
        default:
            fprintf(cp_out, "Character %d is no good\n", *buf);
            goto moe;
    }

}

/* Print a string to the output.  If this would cause the screen to scroll,
 * print "more".
 */

void
out_send(string)
    char *string;
{

    if (noprint)
        return;
    if (!out_isatty || nopause) {
        fputs(string, cp_out);
        return;
    }
    while (*string) {
        switch (*string) {
            case '\n':
                xpos = 0;
                ypos++;
                break;
            case '\f':
                ypos = ysize;
                xpos = 0;
                break;
            case '\t':
                xpos = xpos / 8 + 1;
                xpos *= 8;
                break;
            default:
                xpos++;
                break;
        }
        while (xpos >= xsize) {
            xpos -= xsize;
            ypos++;
        }
        if (ypos >= ysize) {
            outbufputc();       /* out goes buffer */
            promptreturn();
            (void) fflush(cp_out);
            ypos = xpos = 0;
        }
        bufputc(*string);   /* we need to buffer these */
        string++;
    }
    (void) outbufputc();
    return;
}

/* Printf some stuff using more mode. */

#define MAXLEN 4096

/* VARARGS1 */
void
out_printf(fmt, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10)
    char *fmt, *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *s10;
{
    char buf[MAXLEN];

    sprintf(buf, fmt, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10);

    out_send(buf);
    return;
}

