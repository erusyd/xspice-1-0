/* dummy routines for cspice */

#include "FTEdefs.h"
#include "FTEgraph.h"
#include "FTEparse.h"

bool ft_evdb = false;
bool ft_vecdb = false;

#ifdef notdef
struct func func_not;
struct func func_uminus;
#endif

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
	return(false);
}

DestroyGraph(id)
int id;
{
}


GRAPH *CopyGraph(graph)
GRAPH *graph;
{
	return(NULL);
}

#ifdef notdef

/* from parse.c */

static int lasttoken = END, lasttype;
static char *sbuf;

struct pnode *
ft_getpnames(wl, check)
    wordlist *wl;
    bool check;
{
    struct pnode *pn = NULL, *lpn = NULL, *p;
    char *xsbuf;
    char buf[BSIZE], *thisone, *s;

    if (!wl) {
        fprintf(cp_err, "Warning: NULL arithmetic expression\n");
        return (NULL);
    }

    lasttoken = END;
    xsbuf = sbuf = wl_flatten(wl);
    thisone = sbuf;
    while (*sbuf != '\0') {
        if (!(p = parse()))
            return (NULL);

        /* Now snag the name... Much trouble... */
        while (isspace(*thisone))
            thisone++;
        for (s = buf; thisone < sbuf; s++, thisone++)
            *s = *thisone;
        *s = '\0';
        p->pn_name = copy(buf);

        if (pn) {
            lpn->pn_next = p;
            lpn = p;
        } else
            pn = lpn = p;
    }
    if (check)
        if (!checkvalid(pn))
            return (NULL);
    tfree(xsbuf);
    return (pn);
}

/* See if there are any variables around which have length 0 and are
 * not named 'list'. There should really be another flag for this...
 */

static bool
checkvalid(pn)
    struct pnode *pn;
{
    while (pn) {
        if (pn->pn_value) {
            if ((pn->pn_value->v_length == 0) && 
                !eq(pn->pn_value->v_name, "list")) {
                if (eq(pn->pn_value->v_name, "all"))
                    fprintf(cp_err,
                    "Error: %s: no matching vectors.\n",
                        pn->pn_value->v_name);
                else
                    fprintf(cp_err,
                        "Error: %s: no such vector.\n",
                        pn->pn_value->v_name);
                return (false);
            }
        } else if (pn->pn_func || 
                (pn->pn_op && (pn->pn_op->op_arity == 1))) {
            if (!checkvalid(pn->pn_left))
                return (false);
        } else if (pn->pn_op && (pn->pn_op->op_arity == 2)) {
            if (!checkvalid(pn->pn_left))
                return (false);
            if (!checkvalid(pn->pn_right))
                return (false);
        } else
            fprintf(cp_err, 
                "checkvalid: Internal Error: bad node\n");
        pn = pn->pn_next;
    }
    return (true);
}

#endif

gr_redraw(graph)
GRAPH *graph;
{
}

DevSwitch(devname)
char *devname;
{
}

NewViewport(pgraph)
GRAPH *pgraph;
{
}

void
cp_remkword(class, word)
    char *word;
{
}

void
cp_resetcontrol()
{
}

/* Returns the minimum and maximum values of a dvec. Returns a pointer to
 * static data.  If real is true look at the real parts, otherwise the imag
 * parts.
 */

double *
ft_minmax(v, real)
    struct dvec *v;
    bool real;
{
    static double res[2];
    register int i;
    double d;

    res[0] = HUGE;
    res[1] = - HUGE;

    for (i = 0; i < v->v_length; i++) {
        if (isreal(v))
            d = v->v_realdata[i];
        else if (real)
            d = realpart(&v->v_compdata[i]);
        else
            d = imagpart(&v->v_compdata[i]);
        if (d < res[0])
            res[0] = d;
        if (d > res[1])
            res[1] = d;
    }
    return (res);
}

/* Figure out where a point should go, given the limits of the plotting
 * area and the type of scale (log or linear).
 */

int
ft_findpoint(pt, lims, maxp, minp, islog)
    double pt, *lims;
    bool islog;
{
    double tl, th;

    if (pt < lims[0])
        pt = lims[0];
    if (pt > lims[1])
        pt = lims[1];
    if (islog) {
        tl = mylog10(lims[0]);
        th = mylog10(lims[1]);
        return (((mylog10(pt) - tl) / (th - tl)) *
                (maxp - minp) + minp);
    } else {
        return (((pt - lims[0]) / (lims[1] - lims[0])) *
                (maxp - minp) + minp);
    }
}

void
ft_graf(v, xs, nostart)
    struct dvec *v, *xs;
    bool nostart;
{
}
