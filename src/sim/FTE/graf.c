
/*
 *  $Header: //pepper/atesse_spice/spice3/FTE/RCS/graf.c,v 1.1 91/04/02 12:11:41 bill Exp $
 *  Copyright 1988 Jeffrey M. Hsu
 *
 *  Most of the gr_ module resides here, in particular, gr_init
 *      and gr_point, expect for the gr_ grid routines.
 *
 */

#include "prefix.h"
#include "CPdefs.h"     /* for VT_ */
#ifndef CMS
#include "FTEconstant.h"
#else  /* CMS */
#include "FTEconst.h"
#endif /* CMS */
#include "FTEdebug.h"       /* for iplot */
#include "FTEdata.h"        /* for struct dvec */
#include "FTEdefs.h"        /* for FTEextern.h and IPOINT{MIN,MAX} */

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
#include "suffix.h"

extern struct dbcomm *dbs;  /* for iplot */

/* note: let's try to get rid of these */
/* global variables */
/* Graphics mode in progress, so signal handlers know to call gr_clean */
/* bool gr_gmode = false; */

/* for legends, set in gr_start, reset in gr_iplot and gr_init */
static int plotno;
static int curcolor = 1;        /* for assigning unique colors */
static int curlst = 0;          /* for assigning line styles */

/* invariant:  currentgraph contains the current graph */

/* These are what gets plotted as points when you specify point plots */
static char pointchars[128];
#define DEFPOINTCHARS   "oxabcdefhgijklmnpqrstuvwyz"

#define XFACTOR 2       /* How much to expand the X scale during iplot. */
#define YFACTOR 1.5     /* How much to expand the Y scale during iplot. */

/*
 *  Start of a new graph.
 *  Fill in the data that gets displayed.
 *  Difference from old gr_init
 *    we don't try to determine the look of the screen from here
 *    leave to lower level routines
 *
 */
/* ARGSUSED */ /* since we don't really use many of them */
gr_init(xlims, ylims, xname, plotname, hcopy, nplots, xdelta, ydelta, gridtype,
                plottype, xlabel, ylabel, xtype, ytype, pname, commandline)
        double *xlims, *ylims;          /* The size of the screen. */
        char *xname, *plotname;         /* What to label things. */
    char *hcopy;                    /* The raster file. */
        int nplots;                     /* How many plots there will be. */
        double xdelta, ydelta;          /* Line increments for the scale. */
        GRIDTYPE gridtype;              /* The grid type ... */
        PLOTTYPE plottype;              /*  and the plot type. */
        char *xlabel, *ylabel;          /* Labels for axes. */
        int xtype, ytype;               /* The types of the data graphed. */
        char *pname;
        char *commandline;              /* For xi_zoomdata()... */
{

    GRAPH *graph;
    int b;

    if (!(graph = NewGraph())) {
      return(false);
    }

    /* 
       The global currentgraph will always be the current graph.
    */
    SetGraphContext(graph->graphid);

    graph->onevalue = (xname ? false : true);

    /* communicate filename to plot 5 driver */
    if (hcopy) {
      graph->devdep = hcopy;
    }

    plotno = 0;

    /* note: should do only once, maybe in gr_init_once */
    if (!cp_getvar("pointchars", VT_STRING, pointchars))
          (void) strcpy(pointchars, DEFPOINTCHARS);

    if (!cp_getvar("ticmarks", VT_NUM, (char *) &graph->ticmarks)) {
      if (cp_getvar("ticmarks", VT_BOOL, (char *) &b))
        graph->ticmarks = 10;
      else
        graph->ticmarks = 0;
    }

    /* set slow flag to stop between each plot and prompt the
        user for a return
       This is used mainly for graphics terminals w/o windows.
    */
/*
    if (incremental)
      slow = false;
    else
      (void) cp_getvar("slowplot", VT_BOOL, (char *) &slow);
*/

    if (!xlims || !ylims) {
      internalerror("gr_init:  no range specified");
      return(false);
    }

    /* indicate some graphics going on */
/*  gr_gmode = true; */

    /* set upper and lower limits */
    graph->datawindow.xmin = xlims[0];
    graph->datawindow.xmax = xlims[1];
    graph->datawindow.ymin = ylims[0];
    graph->datawindow.ymax = ylims[1];

    /* note: have enum here or some better convention */
    if (NewViewport(graph) == 1) {
      /* note: where is the error message generated? */
      /* note: undo mallocs */
      fprintf(cp_err, "Can't open viewport for graphics.\n");
      return(false);
    }

    /* layout decisions */
    /* note: have to do before gr_fixgrid and after NewViewport */
    graph->viewportxoff = graph->fontwidth * 8; /* 8 lines on left */
    graph->viewportyoff = graph->fontheight * 4;    /* 4 on bottom */

    DevClear();

    graph->grid.gridtype = gridtype;
    graph->plottype = plottype;
    graph->grid.xdatatype = xtype;
    graph->grid.ydatatype = ytype;
    graph->grid.xdelta = xdelta;
    graph->grid.ydelta = ydelta;

    if (!graph->onevalue) {
        if (xlabel) {
            graph->grid.xlabel = xlabel;
        } else {
            graph->grid.xlabel = xname;
        }
        if (ylabel) {
            graph->grid.ylabel = ylabel;
        }
    } else {
        if (xlabel) {
            graph->grid.xlabel = xlabel;
        } else {
            graph->grid.xlabel = "real";
        }
        if (ylabel) {
            graph->grid.ylabel = ylabel;
        } else {
            graph->grid.xlabel = "imag";
        }
    }

    gr_resize_internal(graph);
    gr_redrawgrid(graph);

    /* Set up colors and line styles. */
    if (dispdev->numlinestyles == 1)
        curlst = 0; /* Use the same one all the time. */
    else
        curlst = 1;
    if (dispdev->numcolors > 2)
        curcolor = (graph->grid.gridtype == GRID_SMITH) ? 3 : 1;
    else
        curcolor = 1;

    graph->commandline = copy(commandline);
    graph->plotname = copy(pname);
    /* note: what do we do with plotname?  use it as label XXX */

    return(true);

}

/*
 *  Add a point to the curve we're currently drawing.
 *  Should be in between a gr_init() and a gr_end()
 *    expect when iplotting, very bad hack
 *  Differences from old gr_point:
 *    We save points here, instead of in lower levels.
 *    Assume we are in right context
 *  Save points in data space (not screen space).
 *  We pass two points in so we can multiplex plots.
 *
 */
void
gr_point(dv, newx, newy, oldx, oldy, np)
struct dvec *dv;
double newx, newy, oldx, oldy;
int np;
{
    int oldtox, oldtoy;     /* value before clipping */

    char pointc[2];

    int fromx, fromy, tox, toy;
    int ymin, dummy;

    /* If it's a linear plot, ignore first point since we don't want
        to connect with oldx and oldy. */
    if (dv->v_plottype == PLOT_LIN && !np) return;

    DatatoScreen(currentgraph, oldx, oldy, &fromx, &fromy);
    DatatoScreen(currentgraph, newx, newy, &tox, &toy);

/* note: we do not particularly want to clip here */
    oldtox = tox; oldtoy = toy;
    if (!currentgraph->grid.circular) {
      if (clip_line(&fromx, &fromy, &tox, &toy,
        currentgraph->viewportxoff, currentgraph->viewportyoff,
        currentgraph->viewport.width + currentgraph->viewportxoff,
        currentgraph->viewport.height + currentgraph->viewportyoff))
        return;
    } else {
      if (clip_to_circle(&fromx, &fromy, &tox, &toy,
            currentgraph->grid.xaxis.circular.center,
            currentgraph->grid.yaxis.circular.center,
            currentgraph->grid.xaxis.circular.radius))
        return;
    }

    if (currentgraph->plottype != PLOT_POINT) {
      SetLinestyle(dv->v_linestyle);
    } else {
      /* if PLOT_POINT,
           don't want to plot an endpoint which have been clipped */
      if (tox != oldtox || toy != oldtoy)
    return;
    }
    SetColor(dv->v_color);

    switch (currentgraph->plottype) {
      case PLOT_LIN:
        DrawLine(fromx, fromy, tox, toy);
        if ((currentgraph->ticmarks >0) && (np > -1)
                && (np % currentgraph->ticmarks == 0)) {
          /* Draw an 'x' */
          Text("x", (int) (tox - currentgraph->fontwidth / 2),
                (int) (toy - currentgraph->fontheight / 2));
          SaveText(currentgraph, "x",
                (int) (tox - currentgraph->fontwidth / 2),
                (int) (toy - currentgraph->fontheight / 2));
        }
        break;
      case PLOT_COMB:
        DatatoScreen(currentgraph,
                (double) 0, currentgraph->datawindow.ymin,
                &dummy, &ymin);
        DrawLine(tox, ymin, tox, toy);
        break;
      case PLOT_POINT:
        /* Here, gi_linestyle is the character used for the point.  */
        pointc[0] = dv->v_linestyle;
        pointc[1] = '\0';
        Text(pointc, (int) (tox - currentgraph->fontwidth / 2),
            (int) (toy - currentgraph->fontheight / 2));
        /* gr_redraw will redraw this w/o our having to save it */
        /* SaveText(currentgraph, pointc,
            (int) (tox - currentgraph->fontwidth / 2),
            (int) (toy - currentgraph->fontheight / 2)); */
      default:
        break;
    }

}

static void
gr_start_internal(dv, copyvec)
struct dvec *dv;
bool copyvec;
{

    struct dveclist *link;
    char *s;

    /* Do something special with poles and zeros.  Poles are 'x's, and
     * zeros are 'o's.
     */
    s = ft_typenames(dv->v_type);
        if (eq(s, "pole")) {
                dv->v_linestyle = 'x';
                return;
        } else if (eq(s, "zero")) {
                dv->v_linestyle = 'o';
                return;
        }

    /* Find a (hopefully) new line style and color. */
        if (currentgraph->plottype == PLOT_POINT) {
                if (pointchars[curlst - 1])
                        curlst++;
                else
                        curlst = 2;
        } else if ((curlst > 0) && (++curlst == dispdev->numlinestyles))
            curlst = 2;
        if ((curcolor > 0) && (++curcolor == dispdev->numcolors))
                curcolor = ((currentgraph->grid.gridtype == GRID_SMITH &&
                (dispdev->numcolors > 3)) ?
                                4 : 2);
    if (currentgraph->plottype == PLOT_POINT)
                dv->v_linestyle = pointchars[curlst - 2];
        else
                dv->v_linestyle = curlst;
        dv->v_color = curcolor;

/* note: XXX */
#ifdef notdef
    /* This is a minor hack -- reset the color... */
    if (dv->v_defcolor)
        ReSetColor(curcolor, dv->v_defcolor);
#endif

    /* save the data so we can refresh */
    link = (struct dveclist *) calloc(1, sizeof(struct dveclist));
    link->next = currentgraph->plotdata;

    if (copyvec) {
      link->vector = vec_copy(dv);
      /* vec_copy doesn't set v_color or v_linestyle */
      link->vector->v_color = dv->v_color;
      link->vector->v_linestyle = dv->v_linestyle;
      link->vector->v_flags |= VF_PERMANENT;
    } else {
      link->vector = dv;
    }

    currentgraph->plotdata = link;

    /* Put the legend entry on the screen. */
    drawlegend(currentgraph, plotno, dv);

    plotno++;

}

/* start one plot of a graph */
void
gr_start(dv)
struct dvec *dv;
{

    gr_start_internal(dv, true);

}

/* make sure the linestyles in this graph don't exceed the number of
    linestyles available in the current display device */
gr_relinestyle(graph)
GRAPH *graph;
{

    struct dveclist *link;

    for (link = graph->plotdata; link; link = link->next) {
      if (graph->plottype == PLOT_POINT) continue;
      if (!(link->vector->v_linestyle < dispdev->numlinestyles)) {
        link->vector->v_linestyle %= dispdev->numlinestyles;
      }
      if (!(link->vector->v_color < dispdev->numcolors)) {
        link->vector->v_color %= dispdev->numcolors;
      }
    }

}

static
drawlegend(graph, plotno, dv)
GRAPH *graph;
int plotno;
struct dvec *dv;
{

    int x, y, i;
    char buf[16];

    x = ((plotno % 2) ? graph->viewportxoff :
            ((graph->viewport.width) / 2));
    y = graph->absolute.height - graph->fontheight
            - ((plotno + 2) / 2) * (graph->fontheight);
    i = y + graph->fontheight / 2 + 1;
    SetColor(dv->v_color);
    if (graph->plottype == PLOT_POINT) {
        (void) sprintf(buf, "%c : ", dv->v_linestyle);
        Text(buf, x + graph->viewport.width / 20
                - 3 * graph->fontwidth, y);
    } else {
        SetLinestyle(dv->v_linestyle);
        DrawLine(x, i, x + graph->viewport.width / 20, i);
    }
    SetColor(1);
    Text(dv->v_name, x + graph->viewport.width / 20
            + graph->fontwidth, y);

}

/* end one plot of a graph */
void
gr_end(dv)
struct dvec *dv;
{
    Update();
#ifdef GI_MFB
    /* hack! */
    if (!strcmp(dispdev->name, "MFB") && dv->v_link2 == NULL) {
      gr_pmsg("Hit return to continue");
      MFBHalt();
    }
#endif
}

/* Print text in the bottom line. */

void
gr_pmsg(text)
    char *text;
{
#ifdef HPUX
    char buf[128];
#endif /* HPUX */

    Update();

    Text(text, currentgraph->absolute.width / 2,
            currentgraph->fontheight);
    Update();
#ifdef HPUX /* This is crazy... Timeouts on read... Ack... */
        while (read(0, buf, 1) < 1)
            ;
#else
        (void) getchar();
#endif
    return;
}

void
gr_clean()
{
    Update();
    return;
}

/* call this routine after viewport size changes */
gr_resize(graph)
GRAPH *graph;
{

    double oldxratio, oldyratio;
    double scalex, scaley;
    struct _keyed *k;

    oldxratio = graph->aspectratiox;
    oldyratio = graph->aspectratioy;
    gr_resize_internal(graph);

    /* X also generates an expose after a resize
    This is handled in X10 by not redrawing on resizes and waiting for
    the expose event to redraw.  In X11, the expose routine tries to
    be clever and only redraws the region specified in an expose
    event, which does not cover the entire region of the plot if the
    resize was from a small window to a larger window.  So in order
    to keep the clever X11 expose event handling, we have the X11
    resize routine pull out expose events for that window, and we
    redraw on resize also.
    */
#ifndef GI_X
    gr_redraw(graph);
#endif

    /* scale keyed text */
    scalex = oldxratio / graph->aspectratiox;
    scaley = oldyratio / graph->aspectratioy;
    for (k = graph->keyed; k; k = k->next) {
      k->x = (k->x - graph->viewportxoff) * scalex + graph->viewportxoff;
      k->y = (k->y - graph->viewportyoff) * scaley + graph->viewportyoff;
    }

}

static gr_resize_internal(graph)
GRAPH *graph;
{

    graph->viewport.width = graph->absolute.width -
            2 * graph->viewportxoff;
    graph->viewport.height = graph->absolute.height -
            2 * graph->viewportyoff;
    
    gr_fixgrid(graph, graph->grid.xdelta, graph->grid.ydelta,
            graph->grid.xdatatype, graph->grid.ydatatype);

    /* cache width and height info to make DatatoScreen go fast */
    /* note: XXX see if this is actually used anywhere */
    graph->datawindow.width = graph->datawindow.xmax -
                    graph->datawindow.xmin;
    graph->datawindow.height = graph->datawindow.ymax -
                    graph->datawindow.ymin;

    /* cache (datawindow size) / (viewport size) */
    graph->aspectratiox = graph->datawindow.width / graph->viewport.width;
    graph->aspectratioy = graph->datawindow.height / graph->viewport.height;

}

/* redraw everything in struct graph */
gr_redraw(graph)
GRAPH *graph;
{

    struct dveclist *link;

    /* establish current graph so default graphic calls will work right */
    PushGraphContext(graph);

    DevClear();

    /* redraw grid */
    gr_redrawgrid(graph);

    for (link=graph->plotdata, plotno = 0; link;
            link = link->next, plotno++) {
      /* redraw legend */
      drawlegend(graph, plotno, link->vector);

      /* replot data
        if onevalue, pass it a NULL scale
        otherwise, if vec has its own scale, pass that
            else pass vec's plot's scale
      */
      ft_graf(link->vector,
          graph->onevalue ? (struct dvec *) NULL :
                    (link->vector->v_scale ?
                    link->vector->v_scale :
                    link->vector->v_plot->pl_scale),
          true);
    }

    gr_restoretext(graph);

    PopGraphContext();

}

gr_restoretext(graph)
GRAPH *graph;
{

    struct _keyed *k;

    /* restore text */
    for (k=graph->keyed; k; k = k->next) {
      SetColor(k->colorindex);
      Text(k->text, k->x, k->y);
    }

}

/* Do some incremental plotting. 3 cases -- first, if length < IPOINTMIN, don't
 * do anything. Second, if length = IPOINTMIN, plot what we have so far. Third,
 * if length > IPOINTMIN, plot the last points and resize if needed.
 * Note we don't check for pole / zero because they are of length 1...
 */

/* note: there is a problem with multiple iplots that use the same vector,
    namely, that vector has the same color throughout.  This is another
    reason why we need to pull color and linestyle out of dvec XXX */

static
iplot(pl)
    struct plot *pl;
{
    int len = pl->pl_scale->v_length;
    struct dvec *v, *xs = pl->pl_scale;
    double *lims, dy;
    double start, stop, step;
    register int j;
    bool changed = false;
    int yt;
    char *yl = NULL;
    double xlims[2], ylims[2];
    int inited = 0;

    for (j = 0, v = pl->pl_dvecs; v; v = v->v_next)
        if (v->v_flags & VF_PLOT)
            j++;
    if (!j)
        return(0);
    if (ft_grdb)
        fprintf(cp_err, "Entering iplot, len = %d...\n\r", len);

    if (len < IPOINTMIN) {
        /* Nothing yet... */
        return(0);
    } else if (len == IPOINTMIN) {
        /* Draw the grid for the first time, and plot everything. */
        lims = ft_minmax(xs, true);
        xlims[0] = lims[0];
        xlims[1] = lims[1];
        ylims[0] = HUGE;
        ylims[1] = - HUGE;
        for (v = pl->pl_dvecs; v; v = v->v_next)
            if (v->v_flags & VF_PLOT) {
                lims = ft_minmax(v, true);
                if (lims[0] < ylims[0])
                  ylims[0] = lims[0];
                if (lims[1] > ylims[1])
                  ylims[1] = lims[1];
                if (!yl)
                  yl = v->v_name;
            }
        if (ft_grdb)
            fprintf(cp_err, 
              "iplot: after 5, xlims = %G, %G, ylims = %G, %G\n\r", 
              xlims[0],
              xlims[1],
              ylims[0],
              ylims[1]);
        for (yt = pl->pl_dvecs->v_type, v = pl->pl_dvecs->v_next; v;
                v = v->v_next)
            if ((v->v_flags & VF_PLOT) && (v->v_type != yt)) {
                yt = 0;
                break;
            }
/*
        (void) gr_init((double *) NULL, (double *) NULL, xs->v_name, 
            pl->pl_title, (char *) NULL, j, xdelta, ydelta,
            GRID_LIN, plottype, xs->v_name, yl, xs->v_type, yt,
            commandline, plotname);
*/
/* note: have command options for iplot to specify xdelta, etc.
    So don't need static variables hack.
    Assume default values for now.
*/
        (void) gr_init(xlims, ylims, xs->v_name, 
            pl->pl_title, (char *) NULL, j, 0.0, 0.0,
            GRID_LIN, PLOT_LIN, xs->v_name, yl, xs->v_type, yt,
            "", "iplot");
        for (v = pl->pl_dvecs; v; v = v->v_next)
            if (v->v_flags & VF_PLOT) {
              gr_start_internal(v, false);
              ft_graf(v, xs, true);
            }
        inited = 1;
    } else {
        /* First see if we have to make the screen bigger... */
        dy = (isreal(xs) ? xs->v_realdata[len - 1] :
                realpart(&xs->v_compdata[len - 1]));
        if (ft_grdb)
            fprintf(cp_err, "x = %G\n\r", dy);
        if (!if_tranparams(ft_curckt, &start, &stop, &step) ||
                !ciprefix("tran", pl->pl_typename)) {
            stop = HUGE;
            start = - HUGE;
        }
        while (dy < currentgraph->datawindow.xmin) {
            changed = true;
            if (ft_grdb)
              fprintf(cp_err, "resize: xlo %G -> %G\n\r",
                  currentgraph->datawindow.xmin,
                  currentgraph->datawindow.xmin -
                    (currentgraph->datawindow.xmax -
                    currentgraph->datawindow.xmin)
                * XFACTOR);
            currentgraph->datawindow.xmin -=
              (currentgraph->datawindow.xmax -
              currentgraph->datawindow.xmin)
              * XFACTOR;
            if (currentgraph->datawindow.xmin < start) {
                currentgraph->datawindow.xmin = start;
                break;
            }
        }
        if (currentgraph->datawindow.xmax <
                currentgraph->datawindow.xmin)
            currentgraph->datawindow.xmax =
                    currentgraph->datawindow.xmin;
        while (dy > currentgraph->datawindow.xmax) {
            changed = true;
            if (ft_grdb)
                fprintf(cp_err, "resize: xhi %G -> %G\n\r",
                  currentgraph->datawindow.xmax,
                  currentgraph->datawindow.xmax +
                    (currentgraph->datawindow.xmax - 
                    currentgraph->datawindow.xmin) * XFACTOR);
            currentgraph->datawindow.xmax +=
                    (currentgraph->datawindow.xmax -
                    currentgraph->datawindow.xmin) *
                    XFACTOR;
            if (currentgraph->datawindow.xmax > stop) {
                currentgraph->datawindow.xmax = stop;
                break;
            }
        }
        for (v = pl->pl_dvecs; v; v = v->v_next) {
            if (!(v->v_flags & VF_PLOT))
                continue;
            dy = (isreal(v) ? v->v_realdata[len - 1] :
                    realpart(&v->v_compdata[len - 1]));
            if (ft_grdb)
                fprintf(cp_err, "y = %G\n\r", dy);
            while (dy < currentgraph->datawindow.ymin) {
                changed = true;
                if (ft_grdb)
                  fprintf(cp_err, "resize: ylo %G -> %G\n\r",
                    currentgraph->datawindow.ymin,
                    currentgraph->datawindow.ymin -
                    (currentgraph->datawindow.ymax - 
                    currentgraph->datawindow.ymin) * YFACTOR);
                currentgraph->datawindow.ymin -=
                  (currentgraph->datawindow.ymax -
                  currentgraph->datawindow.ymin) * YFACTOR;
            }
            if (currentgraph->datawindow.ymax <
                    currentgraph->datawindow.ymin)
                currentgraph->datawindow.ymax =
                        currentgraph->datawindow.ymin;
            while (dy > currentgraph->datawindow.ymax) {
                changed = true;
                if (ft_grdb)
                  fprintf(cp_err, "resize: yhi %G -> %G\n\r",
                    currentgraph->datawindow.ymax,
                    currentgraph->datawindow.ymax +
                      (currentgraph->datawindow.ymax -
                      currentgraph->datawindow.ymin) * YFACTOR);
                currentgraph->datawindow.ymax +=
                  (currentgraph->datawindow.ymax -
                  currentgraph->datawindow.ymin) * YFACTOR;
            }
        }
        if (changed) {
            /* Redraw everything. */
#ifdef notdef
            gr_pmsg("Resizing screen...", false);
            DevClear();
/*
            gr_fixgrid(currentgraph, xdelta, ydelta,
                    xtype, ytype);
*/
/*
            currentgraph->grid.xdatatype = xtype;
            currentgraph->grid.ydatatype = ytype;
            currentgraph->grid.xdelta = xdelta;
            currentgraph->grid.ydelta = ydelta;
*/
            gr_resize_internal(currentgraph);
            gr_redrawgrid(currentgraph);

            /* Now replot everything. */
            plotno = 0;
            curlst = 1;
            curcolor = 1;
            for (v = plot_cur->pl_dvecs; v; v = v->v_next)
                if (v->v_flags & VF_PLOT)
                    ft_graf(v, xs, true);
#endif
            gr_resize_internal(currentgraph);
            gr_redraw(currentgraph);
        } else {
            /* Just connect the last two points. This won't
             * be done with curve interpolation, so it might
             * look funny.
             */
            for (v = pl->pl_dvecs; v; v = v->v_next)
                if (v->v_flags & VF_PLOT) {
                    gr_point(v, 
                    (isreal(xs) ? xs->v_realdata[len - 1] :
                    realpart(&xs->v_compdata[len - 1])),
                    (isreal(v) ? v->v_realdata[len - 1] :
                    realpart(&v->v_compdata[len - 1])),
                    (isreal(xs) ? xs->v_realdata[len - 2] :
                    realpart(&xs->v_compdata[len - 2])),
                    (isreal(v) ? v->v_realdata[len - 2] :
                    realpart(&v->v_compdata[len - 2])),
                    len - 1);
                }
        }
    }
    Update();
    return(inited);
}

static void
set(plot, db, unset)
struct plot *plot;
struct dbcomm *db;
bool unset;
{

    struct dvec *v;
    struct dbcomm *dc;

    if (db->db_type == DB_IPLOTALL) {
      for (v = plot->pl_dvecs; v; v = v->v_next) {
        if (unset)
          v->v_flags &= ~VF_PLOT;
        else
          v->v_flags |= VF_PLOT;
      }
      return;
    }
    for (dc = db; dc; dc = dc->db_also) {
       v = vec_fromplot(dc->db_nodename1, plot);
        if (!v || v->v_plot != plot) {
          if (!eq(dc->db_nodename1, "0")) {
        fprintf(cp_err, "Warning: node %s non-existent in %s.\n",
            dc->db_nodename1, plot->pl_name);
        /* note: XXX remove it from dbs, so won't get further errors */
          }
          continue;
        }
        if (unset)
          v->v_flags &= ~VF_PLOT;
        else
          v->v_flags |= VF_PLOT;
    }
    return;
}

void
gr_iplot(plot)
struct plot *plot;
{

    struct dbcomm *db;
    int dontpop;        /* So we don't pop w/o push. */

    for (db = dbs; db; db = db->db_next) {
      if (db->db_type == DB_IPLOT || db->db_type == DB_IPLOTALL) {

        if (db->db_graphid) PushGraphContext(FindGraph(db->db_graphid));

        set(plot, db, false);

        dontpop = 0;
        if (iplot(plot)) {
          /* graph just assigned */
          db->db_graphid = currentgraph->graphid;
          dontpop = 1;  /* Boy, now I really have to rewrite iplot. */
        }

        set(plot, db, true);

        if (!dontpop && db->db_graphid) PopGraphContext();

      }
    }

}

/*
 *  This gets called after iplotting is done.  We clear out the db_graphid
 *  fields.  Copy the dvecs, which we referenced by reference, so
 *  DestroyGraph gets to free its own copy.  Note:  This is a clear
 *  case for separating the linestyle and color fields from dvec.
 */

void
gr_end_iplot()
{

    struct dbcomm *db;
    GRAPH *graph;
    struct dveclist *link;
    struct dvec *dv;

    for (db = dbs; db; db = db->db_next) {
      if (db->db_type == DB_IPLOT || db->db_type == DB_IPLOTALL) {
        if (db->db_graphid) {

          /* get private copy of dvecs */
          graph = FindGraph(db->db_graphid);

          link = graph->plotdata;

          while (link) {
            dv = link->vector;
            link->vector = vec_copy(dv);
            /* vec_copy doesn't set v_color or v_linestyle */
            link->vector->v_color = dv->v_color;
            link->vector->v_linestyle = dv->v_linestyle;
            link->vector->v_flags |= VF_PERMANENT;
        link = link->next;
          }

          db->db_graphid = 0;
        } else {
          /* warn that this wasn't plotted */
          fprintf(cp_err, "Warning: iplot %d was not executed.\n",
          db->db_number);
        }
      }
    }

    return;
}
