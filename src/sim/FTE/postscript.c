
/*
    $Header: //pepper/atesse_spice/spice3/FTE/RCS/postscript.c,v 1.1 91/04/02 12:12:13 bill Exp $
    Copyright (c) 1988 Jeffrey M. Hsu

    Postscript driver
*/

#include "prefix.h"

#include "FTEgraph.h"
#ifndef CMS
#include "FTEdbgraph.h"
#else  /* CMS */
#include "FTEdbgra.h"
#endif /* CMS */
#ifndef CMS
#include "FTEdevice.h"
#else  /* CMS */
#include "FTEdevic.h"
#endif /* CMS */
#include "FTEinput.h"

#include "suffix.h"

static FILE *plotfile;

#define SOLID 0
static char *linestyle[] = {    "[]",           /* solid */
                "[1 2]",        /* dotted */
                "[7 7]",        /* longdashed */
                "[3 3]",        /* shortdashed */
                "[3 5 1 5]" };      /* dotdashed */

typedef struct {
    int lastlinestyle;      /* initial invalid value */
} PSdevdep;

#define DEVDEP(g) (*((PSdevdep *) (g)->devdep))

PS_Init()
{

    dispdev->numlinestyles = 4;
    dispdev->numcolors = 2;

    dispdev->width = 612;       /* (8 1/2) * 72 */
    dispdev->height = 612;

    return(0);

}

/* devdep initially contains name of output file */
PS_NewViewport(graph)
GRAPH *graph;
{

    if (!(plotfile = fopen(graph->devdep, "w"))) {
      perror(graph->devdep);
      graph->devdep = (char *) NULL;
      return(1);
    }

    /* start file off with a % */
    fprintf(plotfile, "%%! nutmeg plot\n");

    if (graph->absolute.width) {
      /* hardcopying from the screen */

      /* scale so fit on 8 1/2 square */
      fprintf(plotfile, "%g %g scale\n",
        (double) dispdev->width / graph->absolute.width,
        (double) dispdev->height / graph->absolute.height);

          /* re-scale linestyles */
      gr_relinestyle(graph);

    } else {

      /* reasonable values, used in gr_ for placement */
      graph->fontwidth = 12;
      graph->fontheight = 24;

      graph->absolute.width = dispdev->width;
      graph->absolute.height = dispdev->height;

    }

    /* set up a reasonable font */
    fprintf(plotfile, "/Helvetica findfont 10 scalefont setfont\n");

    graph->devdep = malloc(sizeof(PSdevdep));
    DEVDEP(graph).lastlinestyle = -1;
    graph->linestyle = -1;

    return(0);

}

PS_Close()
{

    /* in case PS_Close is called as part of an abort,
            w/o having reached PS_NewViewport */
    if (plotfile) {
      if (DEVDEP(currentgraph).lastlinestyle != -1) {
        /* haven't stroked last path */
        fprintf(plotfile, "stroke\n");
      }
      fprintf(plotfile, "showpage\n");
      fclose(plotfile);
    }

}

PS_Clear()
{

    /* do nothing */

}

PS_DrawLine(x1, y1, x2, y2)
int x1, y1, x2, y2;
{

    /* give these initial invalid values */
    static int lastx = -1,lasty = -1;

    /* note: this is not extendible to more than one graph
        => will have to give NewViewport a writeable graph XXX */
    if (DEVDEP(currentgraph).lastlinestyle != currentgraph->linestyle) {
      fprintf(plotfile, "stroke\n");
      fprintf(plotfile, "newpath\n");
    }
    if ((x1 != lastx) || (y1 != lasty)) {
      fprintf(plotfile, "%d %d moveto ", x1, y1);
    }
    fprintf(plotfile, "%d %d lineto\n", x2, y2);

    /* validate cache */
    lastx = x2;
    lasty = y2;
    DEVDEP(currentgraph).lastlinestyle = currentgraph->linestyle;

}

/* ARGSUSED */
PS_Arc(x0, y0, radius, theta1, theta2)
int x0, y0, radius;
double theta1, theta2;
{


}

PS_Text(text, x, y)
char *text;
int x, y;
{

    int savedlstyle;

    /* set linestyle to solid
        or may get funny color text on some plotters */
    savedlstyle = currentgraph->linestyle;
    PS_SetLinestyle(SOLID);

    /* move to (x, y) */
    fprintf(plotfile, "%d %d moveto\n", x, y);

    fprintf(plotfile, "(%s) show\n", text);

    /* restore old linestyle */
    PS_SetLinestyle(savedlstyle);

}

PS_SetLinestyle(linestyleid)
int linestyleid;
{

    /* special case
        get it when PS_Text restores a -1 linestyle */
    if (linestyleid == -1) {
      currentgraph->linestyle = -1;
      return;
    }

    if (linestyleid < 0 || linestyleid > dispdev->numlinestyles) {
      internalerror("bad linestyleid");
      return;
    }

    if (currentgraph->linestyle != linestyleid) {
      if (DEVDEP(currentgraph).lastlinestyle != -1) {
        fprintf(plotfile, "stroke\n");
      }
      fprintf(plotfile, "%s 0 setdash\n", linestyle[linestyleid]);
      currentgraph->linestyle = linestyleid;
    }
    return;

}

/* ARGSUSED */
PS_SetColor(colorid)
{

    /* do nothing */

}

PS_Update()
{

    fflush(plotfile);

}

