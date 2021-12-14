
/*
 
    Routines to draw the various sorts of grids -- linear, log, polar.
        (no smiths)
*/

#include "prefix.h"
#include "FTEgraph.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"
#include "FTEparse.h"

/* gtri - delete - wbk - 10/8/90 - duplicates include in CPdefs.h:CPstd.h */
/* #include <math.h> */          /* for modf() */
/* gtri - end - wbk - 10/8/90 */

#include "suffix.h"


#define PI 3.1415926535897932384

static double *lingrid(), *loggrid();
static void polargrid(), smithgrid();

#ifdef notdef
static void arcset(), cliparc();
static void adddeglabel(), addradlabel();
#endif

typedef enum { x_axis, y_axis } Axis;

/* note: scaleunits is static and never changed in this file
    ie, can get rid of it */
static bool scaleunits = true;

void
gr_fixgrid(graph, xdelta, ydelta, xtype, ytype)
    GRAPH *graph;
    double xdelta, ydelta;
    int xtype, ytype;
{
    double *dd;

    if (graph->grid.gridtype == GRID_NONE) {
        graph->grid.gridtype = GRID_LIN;
    }

    SetColor(1);
    SetLinestyle(1);

    if ((graph->datawindow.xmin > graph->datawindow.xmax)
            || (graph->datawindow.ymin > graph->datawindow.ymax)) {
      fprintf(cp_err, 
        "gr_fixgrid: Internal Error: bad limits %lg, %lg, %lg, %lg...\r\n",
        graph->datawindow.xmin, graph->datawindow.xmax, 
        graph->datawindow.ymin, graph->datawindow.xmax);
      return;
    }

    if (graph->grid.gridtype == GRID_POLAR) {
        graph->grid.circular = true;
        polargrid();
        return;
    } else if (graph->grid.gridtype == GRID_SMITH) {
        graph->grid.circular = true;
        smithgrid();
        return;
    }
    graph->grid.circular = false;

    if ((graph->grid.gridtype == GRID_XLOG)
            || (graph->grid.gridtype == GRID_LOGLOG))
        dd = loggrid(graph, graph->datawindow.xmin,
                graph->datawindow.xmax,
                xtype, x_axis);
    else
        dd = lingrid(graph, graph->datawindow.xmin,
                graph->datawindow.xmax,
                xdelta, xtype, x_axis);
    graph->datawindow.xmin = dd[0];
    graph->datawindow.xmax = dd[1];
    if ((graph->grid.gridtype == GRID_YLOG)
            || (graph->grid.gridtype == GRID_LOGLOG))
        dd = loggrid(graph, graph->datawindow.ymin,
                graph->datawindow.ymax,
                ytype, y_axis);
    else
        dd = lingrid(graph, graph->datawindow.ymin,
                graph->datawindow.ymax,
                ydelta, ytype, y_axis);
    graph->datawindow.ymin = dd[0];
    graph->datawindow.ymax = dd[1];

/* do we really need this? */
/*
    SetLinestyle(0);
    DrawLine(graph->viewportxoff, graph->viewportyoff,
            graph->viewport.width + graph->viewportxoff,
            graph->viewportyoff);
    DrawLine(graph->viewportxoff, graph->viewportyoff,
            graph->viewportxoff,
            graph->viewport.height + graph->viewportyoff);
    SetLinestyle(1);
*/

    return;
}

gr_redrawgrid(graph)
GRAPH *graph;
{

    SetColor(1);
    SetLinestyle(1);
    /* draw labels */
    if (graph->grid.xlabel) {
      Text(graph->grid.xlabel,
          graph->absolute.width -
              (strlen(graph->grid.xlabel) + 3) * graph->fontwidth,
          graph->fontheight);
    }
    if (graph->grid.ylabel) {
      Text(graph->grid.ylabel,
          graph->fontwidth,
          graph->absolute.height / 2);
    }

    if ((graph->grid.gridtype == GRID_XLOG)
            || (graph->grid.gridtype == GRID_LOGLOG))
        drawloggrid(graph, graph->grid.xaxis.log.hmt,
                graph->grid.xaxis.log.lmt,
                graph->grid.xaxis.log.decsp,
                graph->grid.xaxis.log.subs,
                graph->grid.xaxis.log.pp, x_axis);
    else
        drawlingrid(graph,
                graph->grid.xaxis.lin.units,
                graph->grid.xaxis.lin.spacing,
                graph->grid.xaxis.lin.numspace,
                graph->grid.xaxis.lin.distance,
                graph->grid.xaxis.lin.lowlimit,
                graph->grid.xaxis.lin.highlimit,
                graph->grid.xaxis.lin.onedec,
                graph->grid.xaxis.lin.mult,
                x_axis);

    if ((graph->grid.gridtype == GRID_YLOG)
            || (graph->grid.gridtype == GRID_LOGLOG))
        drawloggrid(graph, graph->grid.yaxis.log.hmt,
                graph->grid.yaxis.log.lmt,
                graph->grid.yaxis.log.decsp,
                graph->grid.yaxis.log.subs,
                graph->grid.yaxis.log.pp, y_axis);
    else
        drawlingrid(graph,
                graph->grid.yaxis.lin.units,
                graph->grid.yaxis.lin.spacing,
                graph->grid.yaxis.lin.numspace,
                graph->grid.yaxis.lin.distance,
                graph->grid.yaxis.lin.lowlimit,
                graph->grid.yaxis.lin.highlimit,
                graph->grid.yaxis.lin.onedec,
                graph->grid.yaxis.lin.mult,
                y_axis);

}

/* Plot a linear grid. Returns the new hi and lo limits. */
static double *
lingrid(graph, lo, hi, delta, type, axis)
    GRAPH *graph;
    double lo, hi;
    double delta;
    int type;
    Axis axis;
{
    int mag, hmt, lmt, nsp, spacing, dst;
    double tenpowmag;
    bool onedec = false;
    int margin;
    int max;
    static double dd[2];
    int mult = 1;
    char buf[16], *s;

    if (axis == x_axis) {
      margin = graph->viewportxoff;
      max = graph->viewport.width + graph->viewportxoff;
    } else {
      margin = graph->viewportyoff;
      max = graph->viewport.height + graph->viewportyoff;
    }

    if (delta < 0.0) {
        fprintf(cp_err, "Warning: %cdelta is negative -- ignored\n",
                (axis == x_axis) ? 'x' : 'y');
        delta = 0;
    }

/* note: not idempotent, fix some other way XXX */
    /* This is a hack to fix a problem on the HP 9000 ... */
    if (!((axis == x_axis) ? graph->grid.xaxis.lin.hacked
                  : graph->grid.yaxis.lin.hacked)
            && hi - lo < 1e-6) {
        if (axis == x_axis)
          graph->grid.xaxis.lin.hacked = 1;
        else
          graph->grid.yaxis.lin.hacked = 1;
        hi += 1e-6 * fabs(hi);
        lo -= 1e-6 * fabs(lo);
    }

    /* Express the difference between the high and low values as
     * diff = d * 10^mag. We know diff >= 0.0...  If scaleunits is
     * set then make sure that mag is a multiple of 3.
     */
    mag = floor(mylog10(hi - lo));
    tenpowmag = pow(10.0, (double) mag);

    if (scaleunits) {
        if ((mag % 3) && !((mag - 1) % 3)) {
            mult = 10;
            mag--;
        } else if ((mag % 3) && !((mag + 1) % 3)) {
            onedec = true;
            mag++;
        }
    }

    /* Now we have to round lo down, and hi up... */
    lmt = (int) floor(lo / tenpowmag);
    hmt = (int) ceil(hi / tenpowmag);
    if (fabs(hi - tenpowmag) < 10e-20) {
        /* This is a hack to fix a strange case... */
        hmt = 1;
    }
    dst = hmt - lmt;

    /* This is a strange case. */
    if (dst == 1) {
        dst = 10;
        mag--;
        tenpowmag /= 10;
        hmt *= 10.0;
        lmt *= 10.0;
    }
    dd[0] = lo = lmt * tenpowmag;
    dd[1] = hi = hmt * tenpowmag;

    /* We have to go from lmt to hmt, so think of some useful places
     * to put grid lines. We will have a total of nsp lines, one
     * every spacing pixels, which is every dst / nsp units.
     */
    if (delta == 0.0) {
        for (nsp = 5; nsp < 10; nsp++)
            if (!(dst % nsp))
                break;
        if (nsp == 10)
            for (nsp = 2; nsp < 5; nsp++)
                if (!(dst % nsp))
                    break;
        /* Aaaaaghhh... */
        if ((dst == 11) || (dst == 13) || (dst == 17) || (dst == 19))
            nsp = dst;
        spacing = (max - margin) / nsp;
    } else {
        /* The user told us where to put the grid lines... They will
         * not be equally spaced in this case (i.e, the right edge
         * won't be a line).
         */
        nsp = (hi - lo) / delta;
        spacing = (max - margin) * delta / (hi - lo);
    }

    /* In case we have a relatively small delta... */
    while (nsp > dst) {
        dst *= 10;
        mag--;
        hmt *= 10.0;
        lmt *= 10.0;
    }

    /* Reset the max X coordinate to deal with round-off error. */
    if (nsp && (delta == 0.0)) {
        if (axis == x_axis)
            graph->viewport.width = spacing * nsp;
        else
            graph->viewport.height = spacing * nsp;
    } else if (!nsp) {
        nsp = 1;
    }

    if (scaleunits) {
        /* Figure out what to put here... */
        switch (mag) {
            case -18:   /* atto */
                (void) strcpy(buf, "a");
                break;
            case -15:   /* femto */
                (void) strcpy(buf, "f");
                break;
            case -12:   /* pico */
                (void) strcpy(buf, "p");
                break;
            case -9:    /* nano */
                (void) strcpy(buf, "n");
                break;
            case -6:    /* micro */
                (void) strcpy(buf, "u");
                break;
            case -3:    /* milli */
                (void) strcpy(buf, "m");
                break;
            case 0:     /* -- */
                (void) strcpy(buf, "");
                break;
            case 3:     /* kilo */
                (void) strcpy(buf, "K");
                break;
            case 6:     /* mega */
                (void) strcpy(buf, "M");
                break;
            case 9:     /* giga */
                (void) strcpy(buf, "G");
                break;
            case 12:    /* terra */
                (void) strcpy(buf, "T");
                break;
            case 15:    /* peta */
                (void) strcpy(buf, "P");
                break;
            case 18:    /* exa */
                (void) strcpy(buf, "E");
                break;
            default:
                (void) sprintf(buf, "e%d", mag);
                break;
        }
        if (s = ft_typabbrev(type))
            (void) strcat(buf, s);
        else
            /* Get rid of the prefix... */
            (void) sprintf(buf, "e%d", mag);
    } else
        (void) sprintf(buf, "e%d", mag);

    /* have to save non-intuitive variables left over
        from old algorithms for redraws */
    if (axis == x_axis) {
      graph->grid.xaxis.lin.spacing = spacing;
      graph->grid.xaxis.lin.numspace = nsp;
      graph->grid.xaxis.lin.distance = dst;
      graph->grid.xaxis.lin.lowlimit = lmt;
      graph->grid.xaxis.lin.highlimit = hmt;
      graph->grid.xaxis.lin.onedec = onedec;
      graph->grid.xaxis.lin.mult = mult;
      (void) strcpy(graph->grid.xaxis.lin.units, buf);
    } else {
      graph->grid.yaxis.lin.spacing = spacing;
      graph->grid.yaxis.lin.numspace = nsp;
      graph->grid.yaxis.lin.distance = dst;
      graph->grid.yaxis.lin.lowlimit = lmt;
      graph->grid.yaxis.lin.highlimit = hmt;
      graph->grid.yaxis.lin.onedec = onedec;
      graph->grid.yaxis.lin.mult = mult;
      (void) strcpy(graph->grid.yaxis.lin.units, buf);
    }

    return (dd);
}

static
drawlingrid(graph, units, spacing, nsp, dst, lmt, hmt, onedec, mult, axis)
GRAPH *graph;
char units[16];
bool onedec;
int hmt, lmt, nsp, spacing, dst, mult;
Axis axis;
{

    int i, j;
    char buf[16];

    /* i counts how many pixels we have drawn, and j counts which unit
     * we are at.
     */
    for (i = 0, j = lmt; j <= hmt; i += spacing, j += dst / nsp) {
        if (j == 0)
            SetLinestyle(0);
        if (graph->grid.gridtype != GRID_NONE) {
/* note: get rid of this parameter and draw both axes at once */
            if (axis == x_axis)
                DrawLine(graph->viewportxoff + i,
                  graph->viewportyoff, graph->viewportxoff + i,
                  graph->viewport.height + graph->viewportyoff);
            else
                DrawLine(graph->viewportxoff,
                  graph->viewportyoff + i,
                  graph->viewport.width + graph->viewportxoff,
                  graph->viewportyoff + i);
        }
        if (j == 0)
            SetLinestyle(1);
        if (onedec)
            (void) sprintf(buf, "%.1lf", (double) j / 10);
        else
            (void) sprintf(buf, "%d", j * mult);
        if (axis == x_axis)
            Text(buf, graph->viewportxoff + i -
                    strlen(buf) / 2 * graph->fontwidth,
                    (int) (graph->fontheight * 2.5));
        else
            Text(buf, graph->viewportxoff -
                    graph->fontwidth * (strlen(buf) + 1),
                    graph->viewportyoff + i -
                    graph->fontheight / 2);
        /* This is to make sure things work when delta > hi - lo. */
        if (nsp == 1)
            j++;
    }
    if (axis == x_axis)
        Text(units,
            graph->absolute.width -
                (strlen(units) + 10) * graph->fontwidth,
            graph->fontheight);
    else
        Text(units, graph->fontwidth,
            (int) (graph->absolute.height * 0.75));
    Update();

}

/* Plot a log grid.  Note that we pay no attention to x- and y-delta here. */
/* ARGSUSED */
static double *
loggrid(graph, lo, hi, type, axis)
    GRAPH *graph;
    double lo, hi;
    int type;
    Axis axis;
{
    static double dd[2];
    int margin;
    int max;
    int decs, subs, pp, decsp, lmt, hmt;

    if (axis == x_axis) {
      margin = graph->viewportxoff;
      max = graph->viewport.width + graph->viewportxoff;
    } else {
      margin = graph->viewportyoff;
      max = graph->viewport.height + graph->viewportyoff;
    }

    /* How many orders of magnitude... We are already guaranteed that hi
     * and lo are positive. We want to have something like 8 grid lines
     * total, so if there are few orders of magnitude put in intermediate
     * lines...
     */
    lmt = floor(mylog10(lo));
    hmt = ceil(mylog10(hi));

    dd[0] = pow(10.0, (double) lmt);
    dd[1] = pow(10.0, (double) hmt);

    decs = hmt - lmt;
    subs = 8 / decs;
    if (decs > 10)
        pp = decs / 8 + 1;
    else
        pp = 1;
    
    decsp = (max - margin) * pp / decs;

    if (axis == x_axis) {
      graph->grid.xaxis.log.hmt = hmt;
      graph->grid.xaxis.log.lmt = lmt;
      graph->grid.xaxis.log.decsp = decsp;
      graph->grid.xaxis.log.subs = subs;
      graph->grid.xaxis.log.pp = pp;
    } else {
      graph->grid.yaxis.log.hmt = hmt;
      graph->grid.yaxis.log.lmt = lmt;
      graph->grid.yaxis.log.decsp = decsp;
      graph->grid.yaxis.log.subs = subs;
      graph->grid.yaxis.log.pp = pp;
    }

    return (dd);

}

static
drawloggrid(graph, hmt, lmt, decsp, subs, pp, axis)
GRAPH *graph;
int hmt, lmt, decsp, subs, pp;
Axis axis;
{

    int i, j, k, l, m;
    char buf[16];

    /* Now plot every pp'th decade line, with subs lines between them. */
    for (i = 0, j = lmt; j <= hmt; i += decsp, j += pp) {
        /* Draw the decade line... */
        if (graph->grid.gridtype != GRID_NONE) {
            if (axis == x_axis)
                DrawLine(graph->viewportxoff + i,
                    graph->viewportyoff,
                    graph->viewportxoff + i,
                    graph->viewport.height
                      +graph->viewportyoff);
            else
                DrawLine(graph->viewportxoff,
                    graph->viewportyoff + i, 
                    graph->viewport.width
                      + graph->viewportxoff,
                    graph->viewportyoff + i);
        }
        (void) sprintf(buf, "e%d", j);
        if (axis == x_axis)
            Text(buf, graph->viewportxoff + i - strlen(buf) / 2,
                    (int) (graph->fontheight * 2.5));
        else
            Text(buf, graph->viewportxoff - graph->fontwidth *
                    (strlen(buf) + 1),
                    graph->viewportyoff + i -
                    graph->fontheight / 2);

        if (j == hmt)
            break;

        /* Now draw the other lines... */
        for (k = 1; k <= subs; k++) {
            l = ceil((double) k * 10 / subs);
            if (l == 10)
                break;
            m = decsp * log10((double ) l) + i;
            if (graph->grid.gridtype != GRID_NONE) {
                if (axis == x_axis)
                    DrawLine(graph->viewportxoff + m,
                        graph->viewportyoff,
                        graph->viewportxoff + m,
                        graph->viewport.height
                          + graph->viewportyoff);
                else
                    DrawLine(graph->viewportxoff,
                        graph->viewportyoff + m,
                        graph->viewport.width
                          + graph->viewportxoff,
                        graph->viewportyoff + m);
            }
        }
    }
    Update();

}

/* Polar grids... */

static void
polargrid()
{

#ifdef notdef
    double mx, my, rad, tenpowmag, theta;
    int hmt, lmt, mag, i, step;
    int relcx, relcy, relrad, dist, degs;
    int x1, y1, x2, y2;
    double minrad, maxrad, pixperunit, xx, yy;
    bool centered = false;

    /* Make sure that our area is square. */
    if (graph->viewport.width > graph->viewport.height) {
        graph->viewport.width =  graph->viewport.height;
    } else {
        graph->viewport.height = graph->viewport.width;
    }

    graph->grid.xaxis.circular.center = (graph->viewport.width
                        + graph->viewportxoff) / 2;
    graph->grid.yaxis.circular.center = (graph->viewport.height
                        + graph->viewportyoff) / 2;
    graph->grid.xaxis.circular.radius = graph->viewport.width / 2;

    /* Figure out the minimum and maximum radii we're dealing with. */
    minrad = HUGE;
    maxrad = - HUGE;
    rad = sqrt(graph->datawindow.xmin * graph->datawindow.xmin
            + graph->datawindow.ymin * graph->datawindow.ymin);
    if (rad > maxrad)
        maxrad = rad;
    if (rad < minrad)
        minrad = rad;
    rad = sqrt(graph->datawindow.xmin * graph->datawindow.xmin
            + graph->datawindow.ymax * graph->datawindow.ymax);
    if (rad > maxrad)
        maxrad = rad;
    if (rad < minrad)
        minrad = rad;
    rad = sqrt(graph->datawindow.xmax * graph->datawindow.xmax
            + graph->datawindow.ymin * graph->datawindow.ymin);
    if (rad > maxrad)
        maxrad = rad;
    if (rad < minrad)
        minrad = rad;
    rad = sqrt(graph->datawindow.xmax * graph->datawindow.xmax
            + graph->datawindow.ymax * graph->datawindow.ymax);
    if (rad > maxrad)
        maxrad = rad;
    if (rad < minrad)
        minrad = rad;
    
    if (maxrad == 0.0) {
        fprintf(cp_err, "Error: 0 radius in polargrid\n");
        return;
    }
    if ((graph->datawindow.xmin < 0) && (graph->datawindow.ymin < 0) && 
            (graph->datawindow.xmax > 0) && (graph->datawindow.ymax > 0))
        minrad = 0;
    if ((graph->datawindow.xmin == - graph->datawindow.xmax)
            && (graph->datawindow.ymin == -graph->datawindow.ymax)
            && (graph->datawindow.xmin == graph->datawindow.ymin))
        centered = true;

    mag = floor(mylog10(maxrad));
    tenpowmag = pow(10.0, (double) mag);
    hmt = maxrad / tenpowmag;
    lmt = minrad / tenpowmag;
    if (hmt * tenpowmag < maxrad)
        hmt++;
    if (lmt * tenpowmag > minrad)
        lmt--;
    maxrad = hmt * tenpowmag;
    minrad = lmt * tenpowmag;

    xx = graph->datawindow.xmin + graph->datawindow.xmax;
    yy = graph->datawindow.ymin + graph->datawindow.ymax;
    graph->datawindow.xmin = xx - maxrad;
    graph->datawindow.xmax = xx + maxrad;
    graph->datawindow.ymin = yy - maxrad;
    graph->datawindow.ymax = yy + maxrad;

    if (ft_grdb)
        printf("polar: maxrad = %g, center = (%g, %g)\n",
                maxrad, xx, yy);

    if ((minrad == 0) && ((hmt - lmt) > 5)) {
        if (!((hmt - lmt) % 2))
            step = 2;
        else if (!((hmt - lmt) % 3))
            step = 3;
        else 
            step = 1;
    } else
        step = 1;
    
    pixperunit = graph->grid.xaxis.circular.radius / (maxrad - minrad);

    relcx = - (graph->datawindow.xmin + graph->datawindow.xmax) / 2
            * pixperunit;
    relcy = - (graph->datawindow.ymin + graph->datawindow.ymax) / 2
            * pixperunit;

    /* The distance from the center of the plotting area to the center of
     * the logical area.
     */
    dist = sqrt((double) (relcx * relcx + relcy * relcy));

drawpolargrid(graph)
GRAPH *graph;
{
    double mx, my, rad, tenpowmag, theta;
    int hmt, lmt, mag, i, step;
    int relcx, relcy, relrad, dist, degs;
    int x1, y1, x2, y2;
    double minrad, maxrad, pixperunit, xx, yy;
    char buf[64];

    Setlinestyle(0);
    Arc(graph->grid.xaxis.circular.center,
            graph->grid.yaxis.circular.center,
            graph->grid.xaxis.circular.radius,
            0, 0);
    Setlinestyle(1);

    /* Now draw the circles. */
    for (i = lmt; (relrad = i * tenpowmag * pixperunit) <= 
            dist + graph->grid.xaxis.circular.radius; i += step) {
        cliparc(graph->grid.xaxis.circular.center + relcx,
                graph->grid.yaxis.circular.center + relcy,
                relrad, 0.0, 0.0,
                graph->grid.xaxis.circular.center,
                graph->grid.yaxis.circular.center,
                graph->grid.xaxis.circular.radius);
        /* Toss on the label... */
        if (relcx || relcy)
            theta = atan2((double) relcy, (double) relcx);
        else
            theta = PI;
        if (i && (relrad > dist - graph->grid.xaxis.circular.radius))
            addradlabel(i, theta,
                (int) (graph->grid.xaxis.circular.center -
                    (relrad - dist) * cos(theta)),
                (int) (graph->grid.yaxis.circular.center
                    - (relrad - dist) * sin(theta)));
    }

    /* Now draw the spokes.  We have two possible cases -- first, the
     * origin may be inside the area -- in this case draw 12 spokes.
     * Otherwise, draw several spokes at convenient places.
     */
    if ((graph->datawindow.xmin <= 0.0)
            && (graph->datawindow.xmax >= 0.0)
            && (graph->datawindow.ymin <= 0.0)
            && (graph->datawindow.ymax >= 0.0)) {
        for (i = 0; i < 12; i++) {
            x1 = graph->grid.xaxis.circular.center + relcx;
            y1 = graph->grid.yaxis.circular.center + relcy;
            x2 = x1 + graph->grid.xaxis.circular.radius * 2
                    * cos(i * PI / 6);
            y2 = y1 + graph->grid.xaxis.circular.radius * 2
                    * sin(i * PI / 6);
            if (!clip_to_circle(&x1, &y1, &x2, &y2,
                    graph->grid.xaxis.circular.center,
                    graph->grid.yaxis.circular.center,
                    graph->grid.xaxis.circular.radius))
                Drawline(x1, y1, x2, y2); {
                /* Add a label here... */
                adddeglabel(i * 30, x2, y2, x1, y1);
            }
        }
    } else {
        /* Figure out the angle that we have to fill up... */
        theta = 2 * asin((double) graph->grid.xaxis.circular.radius
                / dist);
        theta = theta * 180 / PI;   /* Convert to degrees. */
        
        /* See if we should put lines at 30, 15, 5, or 1 degree 
         * increments.
         */
        if (theta / 30 > 3)
            degs = 30;
        else if (theta / 15 > 3)
            degs = 15;
        else if (theta / 5 > 3)
            degs = 5;
        else
            degs = 1;

        /* We'll be cheap... */
        for (i = 0; i < 360; i+= degs) {
            x1 = graph->grid.xaxis.circular.center + relcx;
            y1 = graph->grid.yaxis.circular.center + relcy;
            x2 = x1 + dist * 2 * cos(i * PI / 180);
            y2 = y1 + dist * 2 * sin(i * PI / 180);
            if (!clip_to_circle(&x1, &y1, &x2, &y2,
                    graph->grid.xaxis.circular.center,
                    graph->grid.yaxis.circular.center,
                    graph->grid.xaxis.circular.radius)) {
                Drawline(x1, y1, x2, y2);
                /* Put on the label... */
                adddeglabel(i, x2, y2, x1, y1);
            }
        }
    }

    (void) sprintf(buf, "e%d", mag);
    Text(buf, graph->grid.xaxis.circular.center
              + graph->grid.xaxis.circular.radius,
            graph->grid.yaxis.circular.center
              - graph->grid.xaxis.radius);
    Update();
    return;
#endif
}

/* Put a degree label on the screen, with 'deg' as the label, near point (x, y)
 * such that the perpendicular to (lx, ly) and (x, y) doesn't overwrite the
 * label... Sort of...  If the distance between the center and the point is
 * too small, don't put the label on...
 */

#ifdef notdef

#define LOFF    5
#define MINDIST 10

static void
adddeglabel(deg, x, y, lx, ly)
    int deg, x, y, lx, ly;
{
    char buf[8];
    int d, w, h;
    double angle;

    if (sqrt((double) (x - lx) * (x - lx) + (y - ly) * (y - ly)) < MINDIST)
        return;
    (void) sprintf(buf, "%d", deg);
    w = graph->fontwidth * (strlen(buf) + 1);
    h = graph->fontheight * 1.5;
    angle = atan2((double) (y - ly), (double) (x - lx));
    d = fabs(cos(angle)) * w / 2 + fabs(sin(angle)) * h / 2 + LOFF;
    x = x + d * cos(angle) - w / 2;
    y = y + d * sin(angle) - h / 2;
    Text(buf, x, y);
    Text("o", x + strlen(buf) * graph->fontwidth,
            y + graph->fontheight / 2, 0);
    return;
}

/* This is kind of wierd. If dist = 0, then this is the normal case, where
 * the labels should go along the positive X-axis.  Otherwise, to make
 * sure that all circles drawn have labels, put the label near the circle
 * along the line from the logical center to the physical center.
 */

void
addradlabel(lab, theta, x, y)
    double theta;
    int lab, x, y;
{
    char buf[32];

    (void) sprintf(buf, "%d", lab);
    if (theta == PI) {
        y = y - graph->fontheight - 2;
        x = x - graph->fontwidth * strlen(buf) - 3;
    }
    Text(buf, x, y);
    return;
}

#endif

/* ... and smith charts. */

static void
smithgrid()
{
#ifdef notdef
    double mx, my, rad, tenpowmag, d, minrad, maxrad;
    double pixperunit;
    int relcx, relcy;
    int mt, mag, i, j, k, sp, xoff, yoff, zheight;
    int basemag;
    char buf[64], plab[32], nlab[32];
    bool centered = false;

    gi_resetcolor(2, "Coral");
    gi_resetcolor(3, "MediumAquamarine");
    gi_setlinestyle(0);

    gi_maxx -= 2 * gi_fntwidth;
    gi_maxy -= 2 * gi_fntheight;

    /* Make sure that our area is square, etc. */
    if ((gi_maxx - gr_xmargin) > (gi_maxy - gr_ymargin)) {
        i = (gi_maxx - gr_xmargin - gi_maxy + gr_ymargin) / 2;
        gi_maxx -= i;
        gr_xmargin += i;
    } else {
        i = (gi_maxy - gr_ymargin - gi_maxx + gr_xmargin) / 2;
        gi_maxy -= i;
        gr_ymargin += i;
    }

    gr_xcenter = (gi_maxx + gr_xmargin) / 2;
    gr_ycenter = (gi_maxy + gr_ymargin) / 2;
    gr_radius = (gi_maxx - gr_xmargin) / 2;

    /* We don't handle cases where the range isn't centered about the
     * X-axis.  This is something that has to be fixed...
     */
    if (fabs(graph->datawindow.ymin) > fabs(graph->datawindow.ymax))
        graph->datawindow.ymax = - graph->datawindow.ymin;
    else
        graph->datawindow.ymin = - graph->datawindow.ymax;
    
    /* What a hassle. */
    graph->datawindow.xmin *= sqrt(2.0);
    graph->datawindow.xmax *= sqrt(2.0);
    graph->datawindow.ymin *= sqrt(2.0);
    graph->datawindow.ymax *= sqrt(2.0);

    /* We have to make sure that the range is square... */
    mx = graph->datawindow.xmax - graph->datawindow.xmin;
    my = graph->datawindow.ymax - graph->datawindow.ymin;
    if (mx > my) {
        graph->datawindow.ymin -= (mx - my) / 2;
        graph->datawindow.ymax += (mx - my) / 2;
    } else if (mx < my) {
        graph->datawindow.xmin -= (my - mx) / 2;
        graph->datawindow.xmax += (my - mx) / 2;
    }
    if ((graph->datawindow.xmin == - graph->datawindow.xmax) && 
            (graph->datawindow.ymin == -
            graph->datawindow.ymax) && (graph->datawindow.xmin == 
            graph->datawindow.ymin))
        centered = true;

    /* Figure out the minimum and maximum radii we're dealing with. */
    mx = (graph->datawindow.xmin + graph->datawindow.xmax) / 2;
    my = (graph->datawindow.ymin + graph->datawindow.ymax) / 2;
    d = sqrt(mx * mx + my * my);
    maxrad = d + (graph->datawindow.xmax - graph->datawindow.xmin) / 2;
    minrad = d - (graph->datawindow.xmax - graph->datawindow.xmin) / 2;

    mag = floor(mylog10(maxrad));
    tenpowmag = pow(10.0, (double) mag);

    pixperunit = gr_radius * 2 / (graph->datawindow.xmax - 
            graph->datawindow.xmin);

    relcx = - (graph->datawindow.xmin + graph->datawindow.xmax) / 2 * 
            pixperunit;
    relcy = - (graph->datawindow.ymin + graph->datawindow.ymax) / 2 * 
            pixperunit;

    xoff = - pixperunit * (graph->datawindow.xmin + graph->datawindow.xmax) / 2;
    yoff = - pixperunit * (graph->datawindow.ymin + graph->datawindow.ymax) / 2;

    /* Sweep the range from 10e-20 to 10e20.  If any arcs fall into the
     * picture, plot the arc set.
     */
    for (mag = -20; mag < 20; mag++) {
        i = gr_radius * pow(10.0, (double) mag) / maxrad;
        if (i > 10) {
            j = 1;
            break;
        } else if (i > 5) {
            j = 2;
            break;
        } else if (i > 2) {
            j = 5;
            break;
        }
    }
    k = 0;

    /* Now plot all the arc sets.  Go as high as 5 times the radius that
     * will fit on the screen.  The base magnitude is one more than 
     * the least magnitude that will fit...
     */
    if (i > 20)
        basemag = mag;
    else
        basemag = mag + 1;
    while (mag < 20) {
        /*
        i = j * gr_radius * pow(10.0, (double) mag) / (maxrad * 2);
        */
        i = j * pow(10.0, (double) mag) * pixperunit / 2;
        if (i / 5 > gr_radius + ((xoff > 0) ? xoff : - xoff))
            break;
        (void) sprintf(plab, "%lg", j * pow(10.0, 
                (double) (mag - basemag)));
        (void) sprintf(nlab, "-%lg", j * pow(10.0,
                (double) (mag - basemag)));
        arcset(i, k, gr_radius, gr_xcenter, gr_ycenter, xoff, yoff,
                plab, nlab);
        if (i * 2.5 < gr_radius + ((xoff > 0) ? xoff : - xoff))
            k = i;
        if (j == 5) {
            j = 1;
            mag++;
        } else if (j == 2)
            j = 5;
        else if (j == 1)
            j = 2;
    }
    if (mag == 20) {
        fprintf(cp_err, "smithgrid: Internal Error: screwed up\n");
        return;
    }

    gi_arc(gr_xcenter, gr_ycenter, gr_radius, 0.0, 0.0);
    if ((xoff > - gr_radius) && (xoff < gr_radius)) {
        zheight = gr_radius * sin(acos((double) xoff / gr_radius));
        if (zheight < 0)
            zheight = - zheight;
        gi_drawline(gr_xcenter + xoff, gr_ycenter - zheight,
                gr_xcenter + xoff, gr_ycenter + zheight);
    }
    if ((yoff > - gr_radius) && (yoff < gr_radius)) {
        zheight = gr_radius * cos(asin((double) yoff / gr_radius));
        if (zheight < 0)
            zheight = - zheight;
        gi_drawline(gr_xcenter - zheight, gr_ycenter + yoff,
                gr_xcenter + zheight, gr_ycenter + yoff);
    }

    (void) sprintf(buf, "e%d", basemag);
    gi_text(buf, gr_xcenter + gr_radius, gr_ycenter - gr_radius, 0, false);

#ifdef notdef
    gi_text("0", gr_xcenter + gr_radius + gi_fntwidth, gr_ycenter - 
            gi_fntheight / 2, 0, false);
    gi_text("o", gr_xcenter + gr_radius + gi_fntwidth * 2, gr_ycenter, 0, 
            false);
    gi_text("90", gr_xcenter - gi_fntwidth, gr_ycenter + gr_radius + 
            gi_fntheight / 2, 0, false);
    gi_text("o", gr_xcenter + gi_fntwidth, gr_ycenter + gr_radius + 
            gi_fntheight, 0, false);
    gi_text("180", gr_xcenter - gr_radius - gi_fntwidth * 5, gr_ycenter 
            - gi_fntheight / 2, 0, false);
    gi_text("o", gr_xcenter - gr_radius - gi_fntwidth * 2, gr_ycenter, 0, 
            false);
    gi_text("-90", gr_xcenter - gi_fntwidth * 2, gr_ycenter - gr_radius -
            2 * gi_fntheight, 0, false);
    gi_text("o", gr_xcenter + gi_fntwidth, gr_ycenter - gr_radius -
            gi_fntheight - gi_fntheight / 2, 0, false);
#endif
    gi_update();
    return;
#endif
}

/* Draw one arc set.  The arcs should have radius rad. The outermost circle is
 * described by (centx, centy) and maxrad, and the distance from the right side
 * of the bounding circle to the logical center of the other circles in pixels
 * is xoffset (positive brings the negative plane into the picture).
 * plab and nlab are the labels to put on the positive and negative X-arcs,
 * respectively...  If the X-axis isn't on the screen, then we have to be
 * clever...
 */

#ifdef notdef
static void
arcset(rad, prevrad, maxrad, centx, centy, xoffset, yoffset, plab, nlab)
    int rad, prevrad, maxrad, centx, centy, xoffset, yoffset;
    char *plab, *nlab;
{
    double angle = atan((double) prevrad / rad);
    int x;

    /* Let's be lazy and just draw everything -- we won't get called too
     * much and the circles get clipped anyway...
     */
    gi_setcolor(2);
    cliparc(centx + xoffset - rad, centy + yoffset, rad, 2 * angle,
            2 * PI - 2 * angle, centx, centy, maxrad);

    cliparc(centx + xoffset + rad, centy + yoffset, rad, PI + 2 * 
            angle, PI - 2 * angle, centx, centy, maxrad);

    /* Draw the upper and lower circles.  */
    gi_setcolor(3);
    cliparc(centx + xoffset, centy + yoffset + rad, rad, PI * 1.5 + 2 *
            angle, PI * 1.5 - 2 * angle, centx, centy, maxrad);
    cliparc(centx + xoffset, centy + yoffset - rad, rad, PI / 2 + 2 *
            angle, PI / 2 - 2 * angle, centx, centy, maxrad);
    
    /* Now toss the labels on... */
    gi_setcolor(1);

    x = centx + xoffset - 2 * rad - gi_fntwidth * strlen(plab) - 2;
    if ((x > gr_xmargin) && (x < gi_maxx))
        gi_text(plab, x, centy - gi_fntheight - 1, false, false);
    x = centx + xoffset + 2 * rad - gi_fntwidth * strlen(nlab) - 2;
    if ((x > gr_xmargin) && (x < gi_maxx))
        gi_text(nlab, x, centy - gi_fntheight - 1, false, false);

    return;
}

/* This routine draws an arc and clips it to a circle.  It's hard to figure
 * out how it works without looking at the piece of scratch paaper I have
 * in front of me, so let's hope it doesn't break...
 */

static void
cliparc(cx, cy, rad, start, end, clipx, clipy, cliprad)
    int cx, cy, rad, clipx, clipy, cliprad;
    double start, end;
{
    int x, y, tx, ty, dist;
    double alpha, theta, phi, a1, a2, d, l;
    bool in;

    x = cx - clipx;
    y = cy - clipy;
    dist = sqrt((double) (x * x + y * y));

    if (!rad || !cliprad)
        return;
    if (dist + rad < cliprad) {
        /* The arc is entirely in the boundary. */
        gi_arc(cx, cy, rad, start, end);
        return;
    } else if ((dist - rad >= cliprad) || (rad - dist >= cliprad)) {
        /* The arc is outside of the boundary. */
        return;
    }
    /* Now let's figure out the angles at which the arc crosses the
     * circle. We know dist != 0.
     */
    if (x)
        phi = atan((double) y / x);
    else if (y > 0)
        phi = PI * 1.5;
    else
        phi = PI / 2;
    if (cx > clipx)
        theta = PI + phi;
    else
        theta = phi;

    alpha = acos((double) (dist * dist + rad * rad - cliprad * cliprad) /
            (2 * dist * rad));

    a1 = theta + alpha;
    a2 = theta - alpha;
    while (a1 < 0)
        a1 += PI * 2;
    while (a2 < 0)
        a2 += PI * 2;
    while (a1 >= PI * 2)
        a1 -= PI * 2;
    while (a2 >= PI * 2)
        a2 -= PI * 2;

    tx = cos(start) * rad + x;
    ty = sin(start) * rad + y;
    d = sqrt((double) tx * tx + ty * ty);
    in = (d > cliprad) ? false : true;

    /* Now begin with start.  If the point is in, draw to either end, a1,
     * or a2, whichever comes first.
     */
    d = PI * 3;
    if ((end < d) && (end > start))
        d = end;
    if ((a1 < d) && (a1 > start))
        d = a1;
    if ((a2 < d) && (a2 > start))
        d = a2;
    if (d == PI * 3) {
        d = end;
        if (a1 < d)
            d = a1;
        if (a2 < d)
            d = a2;
    }

    if (in)
        gi_arc(cx, cy, rad, start, d);
    if (d == end)
        return;
    if (a1 != a2)
        in = in ? false : true;

    /* Now go from here to the next point. */
    l = d;
    d = PI * 3;
    if ((end < d) && (end > l))
        d = end;
    if ((a1 < d) && (a1 > l))
        d = a1;
    if ((a2 < d) && (a2 > l))
        d = a2;
    if (d == PI * 3) {
        d = end;
        if (a1 < d)
            d = a1;
        if (a2 < d)
            d = a2;
    }

    if (in)
        gi_arc(cx, cy, rad, l, d);
    if (d == end)
        return;
    in = in ? false : true;
    
    /* And from here to the end. */
    if (in)
        gi_arc(cx, cy, rad, d, end);
    return;
}
#endif

#ifdef notdef

/* These routines are wrong on a lot of systems. */

double
floor(d)
    double d;
{
    double fp, ip;

    if (d >= 0.0) {
        fp = modf(d, &ip);
        return (ip);
    } else {
        /* This is the case that lattice C screws up... */
        fp = modf(-d, &ip);
        if (fp != 0.0)
            return (-ip - 1);
        else
            return (-ip);
    }
}

double
ceil(d)
    double d;
{
    return (- floor(-d));
}

#endif
