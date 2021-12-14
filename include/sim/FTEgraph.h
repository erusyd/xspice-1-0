
/*
    $Header: //pepper/atesse_spice/spice3/include/RCS/FTEgraph.h,v 1.1 91/04/02 11:25:52 bill Exp $
    Copyright (c) 1988 Jeffrey M. Hsu

    This file contains the graph structure.
*/

#ifndef _GRAPH_H_
#define _GRAPH_H_

#ifndef CMS
#include "FTEconstant.h"
#else  /* CMS */
#include "FTEconst.h"
#endif /* CMS */
#include "FTEdata.h"            /* for struct dvec */

/* Device-independent data structure for plots. */

#define NUMCOLORS 20

/* list of data vectors being displayed */
struct dveclist {
    struct dvec *vector;
    struct dveclist *next;
};

typedef struct graph {
    int graphid;
    struct dveclist *plotdata;  /* normalized data */
    char *plotname;         /* name of plot this graph is in */
    int onevalue;           /* boolean variable,
                       true if plotting one value
                       against itself (real vs imaginary) */
    int degree;     /* degree of polynomial interpretation */

    int currentcolor;
    int linestyle;

    struct {
        int height, width;
    } viewport;
    int viewportxoff;   /* x offset of viewport w/in graph */
    int viewportyoff;   /* y offset of viewport w/in graph */

    struct {
        int xpos;   /* x position of graph in screen coord */
        int ypos;   /* y position of graph in screen coord */
        int width;  /* width of window on screen */
        int height; /* height of window on screen */
    } absolute;

    struct {
        double xmin, ymin, xmax, ymax;
        /* cache:  width = xmax - xmin  height = ymax - ymin */
        double width, height;
    } datawindow;

    /* note: this int is device dependent */
    int colors[NUMCOLORS];

    /* cache (datawindow size) / (viewport size) */
    double aspectratiox, aspectratioy;

    int ticmarks;           /* mark every ticmark'th point */
    int fontwidth, fontheight;  /* for use in grid */

    PLOTTYPE plottype;      /* defined in FTEconstant.h */
    struct {
      GRIDTYPE gridtype;        /* defined in FTEconstant.h */
      int circular;         /* TRUE if circular plot area */
      union {
        struct {
        char units[16];     /* unit labels */
        int spacing, numspace, distance, lowlimit, highlimit, mult;
        int onedec;     /* a boolean */
        int hacked;     /* true if hi - lo already hacked up */
        } lin;
        struct {
        int hmt, lmt, decsp, subs, pp;
        } log;
        struct {
        int radius, center;
        } circular;     /* bogus, rework when write polar grids, etc */
      } xaxis, yaxis;
      int xdatatype, ydatatype;
      double xdelta, ydelta; /* if non-zero, user-specified deltas */
      char *xlabel, *ylabel;
    } grid;

    int numbuttons;     /* number of buttons */
    struct {
      int id;
      char *message;
    } *buttons;
    int buttonsxoff;    /* viewportxoff + x size of viewport */
    int buttonsyoff;

    struct {
      int width, height;
      char message[161];        /* two lines of text */
    } messagebox;
    int messagexoff;
    int messageyoff;

    /* characters the user typed on graph */
/* note: think up better names */
    struct _keyed {
      char *text;
      int x, y;
      int colorindex;       /* index into colors array */
      struct _keyed *next;
    } *keyed;

    /* for zoomin */
    char *commandline;

    /* Ick, ick, ick.  Need to have device dependent info here because
        otherwise every device implementation would need to handle
        their own device dependent analog of GRAPH.  I want a
        language with generics.
       Use pointer so we don't have to include device dependent
        header files here.
       Space here is allocated by NewViewport
        and de-allocated by DestroyGraph.
    */
    char *devdep;

} GRAPH;

#define NEWGRAPH (GRAPH *) calloc(1, sizeof(GRAPH))

#define rnd(x)  (int) ((x)+0.5)

#endif  /* notdef _GRAPH_H_ */
