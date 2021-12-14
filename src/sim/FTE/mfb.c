
/*
    $Header: //pepper/atesse_spice/spice3/FTE/RCS/mfb.c,v 1.1 91/04/02 12:11:53 bill Exp $
    Copyright 1988 Jeffrey M. Hsu

    MFB driver
*/

#include "prefix.h"

#ifdef GI_MFB

/* note:  I would have liked to place this after CPdefs,
    but CPdefs #defines false and true, which are enums in mfb.h. */
#include "mfb.h"

#ifndef CMS
#include "FTEdevice.h"
#else  /* CMS */
#include "FTEdevic.h"
#endif /* CMS */
#include "CPdefs.h"                     /* for VT_ */
#include "FTEgraph.h"           /* for rnd */
#include "suffix.h"

#ifndef M_PI		/* some systems don't have this */
#define M_PI    3.14159265358979323846
#endif

extern char ErrorMessage[];

Mfb_Init()
{

    char buf[BSIZE], dev[BSIZE], *term;
    int err;

    /* Try to identify the terminal type -- check variable,
     * environment, then ask the user.
     */
#ifdef IBMPC
    term = "pc";
#else
    if (cp_getvar("term", VT_STRING, buf)) {
      term = buf;
    } else {
      term = getenv("TERM");
      if (term == NULL) { 
        /* prompt user w/ input() */
        fprintf(cp_out, "Terminal type = ");
        (void) fflush(cp_out);
        (void) fscanf(cp_in, " %s", buf);
        term = copy(buf);
      }
    }
#endif
    if (!cp_getvar("device", VT_STRING, dev))
      dev[0] = '\0';

    (void) MFBOpen(term, (dev[0] ? dev : (char *) NULL), &err);
    if (err != MFBOK) {
      sprintf(ErrorMessage, "MFB error: %s", MFBError(err));
      externalerror(ErrorMessage);
      return (1);
    }

    dispdev->numlinestyles = MFBInfo(MAXLINESTYLES);
    dispdev->numcolors = MFBInfo(MAXCOLORS);
    dispdev->width = MFBInfo(MAXX) + 1;
    dispdev->height = MFBInfo(MAXY) + 1;

#ifdef DEFCOLORS
    if (dispdev->numcolors > 2 && MFBInfo(DEFCOLORS))
      mfbsetcolors();
#endif
    if (dispdev->numlinestyles > 2 && MFBInfo(DEFLINEPATTERN))
      mfbsetlinestyles();

/*  MFBSetALUMode(MFBALUJAM);   /* graphics overstrikes */
    MFBSetTextMode(1);      /* text overstrikes */

    /* get control back */
    if (MFBHalt() != MFBOK) {
      externalerror("Can't restore control of tty.");
      return(1);
    }

    return (0);

}

Mfb_NewViewport(graph)
GRAPH *graph;
{

    if (!MFBInitialize() == MFBOK) {
      externalerror("Can't initialize MFB.");
      return(1);
    }

/* note: for now, do nothing
    later, divide screen into n+1, and redraw existing n plots */

    graph->fontwidth = MFBInfo(FONTWIDTH);
    graph->fontheight = MFBInfo(FONTHEIGHT);

    graph->viewport.height = dispdev->height;
    graph->viewport.width = dispdev->width;

    graph->absolute.xpos = 0;
    graph->absolute.ypos = 0;
    graph->absolute.width = dispdev->width;
    graph->absolute.height = dispdev->height;
    return(0);

}

Mfb_Close()
{

    MFBClose();

}

Mfb_Clear()
{

    MFBSetColor(0);
    MFBFlood();

}

Mfb_DrawLine(x1, y1, x2, y2)
int x1, y1, x2, y2;
{

    MFBLine(x1, y1, x2, y2);

}

Mfb_Arc(x0, y0, radius, theta1, theta2)
int x0, y0, radius;
double theta1, theta2;
{

/* note: figure some better value for nsides as a fn of radius
        and delta theta */
    MFBArc(x0, y0, radius, (int) ((double) M_PI / 180 * theta1),
            (int) ((double) M_PI / 180 * theta2), 20);

}

Mfb_Text(text, x, y)
char *text;
int x, y;
{

    MFBText(text, x, y, 0);

}

Mfb_DefineColor(colorid, red, green, blue)
int colorid;
double red, green, blue;
{

/* note: */
    MFBDefineColor(colorid,
            rnd(red * 1000), rnd(green * 1000), rnd(blue * 1000));

}

Mfb_DefineLinestyle()
{

/* note: */

}

Mfb_SetLinestyle(linestyle)
int linestyle;
{

/* note: do error checking */
    if (MFBSetLineStyle(linestyle) != MFBOK) {
      externalerror("Bad linestyle");
    }

}

Mfb_SetColor(colorid)
int colorid;
{

    if (MFBSetColor(colorid) != MFBOK) {
      externalerror("bad color");
    }

}

Mfb_Update()
{

    MFBUpdate();

}

Mfb_MakeMenu()
{

/* note: */

}

Mfb_MakeDialog()
{

/* note: */

}

/* Set colors properly. Assume that we won't need more than 20 colors. 
 * Also we should try to set the foreground and background colors.
 * These color values are just off of the top of my head...
 * Make the grid color (color 1) blue.
 */

#ifdef DEFCOLORS

static int reds[] = { 999,   0, 999,   0,   0, 999, 999,   0, 999, 500,
              500, 999, 999, 500, 999, 999, 600, 300, 600, 300 } ;
static int grns[] = { 999,   0,   0, 999,   0, 999,   0, 999, 500, 999,
              500, 999, 500, 999, 600, 300, 999, 999, 300, 600 } ;
static int blus[] = { 999,   0,   0,   0, 999,   0, 999, 999, 500, 500,
              999, 500, 999, 999, 300, 600, 300, 600, 999, 999 } ;

static
mfbsetcolors()
{
    int i;
    int red, green, blue;

    MFBDefineColor(1, 0, 0, 999);
    for (i = 2; (i < dispdev->numcolors) && (i < 22); i++) {
        red = reds[i - 2];
        green = grns[i - 2];
        blue = blus[i - 2];
        MFBDefineColor(i, red, green, blue);
    }
    return;
}

#endif

/* Set line styles if necessary. Assume we won't use more than 20 line
 * styles. These line styles also are pretty random. Make sure that
 * line style 0 is solid and line style 1 is dotted.
 */

static int styles[] = { 0333, 0347, 0360, 0071, 0216, 0366, 0155, 0011,
            0123, 0317, 0330, 0145, 0275, 0016, 0315, 0101,
            0222, 0174, 0251, 0376 } ;

static
mfbsetlinestyles()
{
    int i, j;

    MFBDefineLineStyle(0, 0377);    /* Solid. */
    MFBDefineLineStyle(1, 0125);    /* Dotted. */
    for (i = 2; (i < dispdev->numlinestyles) && (i < 22); i++) {
        j = styles[i - 2];
        MFBDefineLineStyle(i, j);
    }
    return;
}

#endif

