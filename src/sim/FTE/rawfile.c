
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:12:15 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/rawfile.c,v $
 * Copyright (c) 1986 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Read and write the ascii and binary rawfile formats.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"
#include "suffix.h"

static void fixdims();

int raw_prec = -1;  /* How many sigfigs to use, default 15 (max).  */

#define DEFPREC 15

/* Write a raw file.  We write everything in the plot pointed to. */

void
raw_write(name, pl, app, binary)
    char *name;
    struct plot *pl;
    bool app, binary;
{
    FILE *fp;
    bool realflag = true;
    int length = pl->pl_scale->v_length;
    int nvars, i, prec;
    struct dvec *v, *lv;
    wordlist *wl;
    struct variable *vv;
    double dd;

    if (raw_prec != -1)
        prec = raw_prec;
    else
        prec = DEFPREC;
    if (!(fp = fopen(name, app ? "a" : "w"))) {
        perror(name);
        return;
    }
    for (nvars = 0, v = pl->pl_dvecs; v; v = v->v_next) {
        if (iscomplex(v))
            realflag = false;
        nvars++;
    }

    fprintf(fp, "Title: %s\n", pl->pl_title);
    fprintf(fp, "Date: %s\n", pl->pl_date);
    fprintf(fp, "Plotname: %s\n", pl->pl_name);
    fprintf(fp, "Flags: %s\n", realflag ? "real" : "complex");
    fprintf(fp, "No. Variables: %d\n", nvars);
    fprintf(fp, "No. Points: %d\n", length);

    for (wl = pl->pl_commands; wl; wl = wl->wl_next)
        fprintf(fp, "Command: %s\n", wl->wl_word);

    for (vv = pl->pl_env; vv; vv = vv->va_next) {
        wl = cp_varwl(vv);
        if (vv->va_type == VT_BOOL) {
            fprintf(fp, "Option: %s\n", vv->va_name);
        } else {
            fprintf(fp, "Option: %s = ", vv->va_name);
            if (vv->va_type == VT_LIST)
                fprintf(fp, "( ");
            wl_print(wl, fp);
            if (vv->va_type == VT_LIST)
                fprintf(fp, " )");
            (void) putc('\n', fp);
        }
    }

    /* Before we write the stuff out, make sure that the scale is the first
     * in the list.
     */
    for (lv = NULL, v = pl->pl_dvecs; v != pl->pl_scale; v = v->v_next)
        lv = v;
    if (lv) {
        lv->v_next = v->v_next;
        v->v_next = pl->pl_dvecs;
        pl->pl_dvecs = v;
    }

    fprintf(fp, "Variables:");
    for (i = 0, v = pl->pl_dvecs; v; v = v->v_next) {
        fprintf(fp, " %d %s %s", i++, v->v_name,
                ft_typenames(v->v_type));
        if (v->v_flags & VF_MINGIVEN)
            fprintf(fp, " min=%e", v->v_minsignal);
        if (v->v_flags & VF_MAXGIVEN)
            fprintf(fp, " max=%e", v->v_maxsignal);
        if (v->v_defcolor)
            fprintf(fp, " color=%s", v->v_defcolor);
        if (v->v_gridtype)
            fprintf(fp, " grid=%d", v->v_gridtype);
        if (v->v_plottype)
            fprintf(fp, " plot=%d", v->v_gridtype);
        if (v->v_numdims > 1) {
            fprintf(fp, " dims=");
            for (i = 0; i < v->v_numdims; i++)
                fprintf(fp, "%d%s", v->v_dims[i], (i <
                        v->v_numdims - 1) ? "," : "");
        }
        (void) putc('\n', fp);
    }

    if (binary) {
        fprintf(fp, "Binary:\n");
        for (i = 0; i < length; i++) {
            for (v = pl->pl_dvecs; v; v = v->v_next) {
                if (realflag) {
                    dd = (isreal(v) ? v->v_realdata[i] :
                        realpart(&v->v_compdata[i]));
                    (void) fwrite((char *) &dd, sizeof
                            (double), 1, fp);
                } else if (isreal(v)) {
                    dd = v->v_realdata[i];
                    (void) fwrite((char *) &dd, sizeof
                            (double), 1, fp);
                    dd = 0.0;
                    (void) fwrite((char *) &dd, sizeof
                            (double), 1, fp);
                } else {
                    dd = realpart(&v->v_compdata[i]);
                    (void) fwrite((char *) &dd, sizeof
                            (double), 1, fp);
                    dd = imagpart(&v->v_compdata[i]);
                    (void) fwrite((char *) &dd, sizeof
                            (double), 1, fp);
                }
            }
        }
    } else {
        fprintf(fp, "Values:\n");
        for (i = 0; i < length; i++) {
            fprintf(fp, " %d", i);
            for (v = pl->pl_dvecs; v; v = v->v_next) {
                if (realflag)
                    fprintf(fp, "\t%.*e\n", prec,
                        isreal(v) ? v->v_realdata[i] :
                        realpart(&v->v_compdata[i]));
                else if (isreal(v))
                    fprintf(fp, "\t%.*e,0.0\n", prec,
                        v->v_realdata[i]);
                else
                    fprintf(fp, "\t%.*e,%.*e\n", prec,
                        realpart(&v->v_compdata[i]),
                        prec,
                        imagpart(&v->v_compdata[i]));
            }
            (void) putc('\n', fp);
        }
    }
    (void) fclose(fp);
    return;
}

/* Read a raw file.  Returns a list of plot structures.  This routine should be
 * very flexible about what it expects to see in the rawfile.  Really all we
 * require is that there be one variables and one values section per plot
 * and that the variables precede the values.
 */

#define skip(s) while (*(s) && !isspace(*(s)))(s)++; while (isspace(*(s)))(s)++
#define nonl(s) r = (s); while (*r && (*r != '\n')) r++; *r = '\0'

struct plot *
raw_read(name)
    char *name;
{
    char *title = "default title";
    char *date = datestring();
    struct plot *plots = NULL, *curpl = NULL;
    char buf[BSIZE], buf2[BSIZE], *s, *t, *r;
    int flags, nvars, npoints, i, j;
    struct dvec *v, *nv;
    struct variable *vv;
    wordlist *wl, *nwl;
    FILE *fp, *lastin, *lastout, *lasterr;

    if (!(fp = fopen(name, "r"))) {
        perror(name);
        return (NULL);
    }

    /* Since we call cp_evloop() from here, we have to do this junk. */
    lastin = cp_curin;
    lastout = cp_curout;
    lasterr = cp_curerr;
    cp_curin = cp_in;
    cp_curout = cp_out;
    cp_curerr = cp_err;

    cp_pushcontrol();

    while (fgets(buf, BSIZE, fp)) {
        /* Figure out what this line is... */
        if (ciprefix("title:", buf)) {
            s = buf;
            skip(s);
            nonl(s);
            title = copy(s);
        } else if (ciprefix("date:", buf)) {
            s = buf;
            skip(s);
            nonl(s);
            date = copy(s);
        } else if (ciprefix("plotname:", buf)) {
            s = buf;
            skip(s);
            nonl(s);
            if (curpl) {    /* reverse commands list */
                for (wl=curpl->pl_commands,
                        curpl->pl_commands=NULL; wl &&
                        wl->wl_next; wl=nwl) {
                    nwl = wl->wl_next;
                    wl->wl_next = curpl->pl_commands;
                    curpl->pl_commands = wl;
                }
            }
            curpl = alloc(plot);
            curpl->pl_next = plots;
            plots = curpl;
            curpl->pl_name = copy(s);
            curpl->pl_date = copy(date);
            curpl->pl_title = copy(title);
            flags = VF_PERMANENT;
            nvars = npoints = 0;
        } else if (ciprefix("flags:", buf)) {
            s = buf;
            skip(s);
            while (t = gettok(&s)) {
                if (cieq(t, "real"))
                    flags |= VF_REAL;
                else if (cieq(t, "complex"))
                    flags |= VF_COMPLEX;
                else
                    fprintf(cp_err, 
                        "Warning: unknown flag %s\n",
                        t);
            }
        } else if (ciprefix("no. variables:", buf)) {
            s = buf;
            skip(s);
            skip(s);
            nvars = scannum(s);
        } else if (ciprefix("no. points:", buf)) {
            s = buf;
            skip(s);
            skip(s);
            npoints = scannum(s);
        } else if (ciprefix("command:", buf)) {
            /* Note that we reverse these commands eventually... */
            s = buf;
            skip(s);
            nonl(s);
            if (curpl) {
                wl = alloc(wordlist);
                wl->wl_word = copy(s);
                wl->wl_next = curpl->pl_commands;
                if (curpl->pl_commands)
                    curpl->pl_commands->wl_prev = wl;
                curpl->pl_commands = wl;
            } else
                fprintf(cp_err, 
                    "Error: misplaced Command: line\n");
            /* Now execute the command if we can. */
            (void) cp_evloop(s);
        } else if (ciprefix("option:", buf)) {
            s = buf;
            skip(s);
            nonl(s);
            if (curpl) {
                wl = cp_lexer(s);
                for (vv = curpl->pl_env; vv && vv->va_next;
                        vv = vv->va_next)
                    ;
                if (vv)
                    vv->va_next = cp_setparse(wl);
                else
                    curpl->pl_env = cp_setparse(wl);
            } else
                fprintf(cp_err, 
                    "Error: misplaced Command: line\n");
        } else if (ciprefix("variables:", buf)) {
            /* We reverse the dvec list eventually... */
            if (!curpl) {
                fprintf(cp_err, "Error: no plot name given\n");
                plots = NULL;
                break;
            }
            s = buf;
            skip(s);
            if (!*s) {
                (void) fgets(buf, BSIZE, fp);
                s = buf;
            }
            /* Now read all the variable lines in. */
            for (i = 0; i < nvars; i++) {
                v = alloc(dvec);
                v->v_next = curpl->pl_dvecs;
                curpl->pl_dvecs = v;
                if (!curpl->pl_scale)
                    curpl->pl_scale = v;
                v->v_flags = flags;
                v->v_length = npoints;
                v->v_plot = curpl;

                if (isreal(v))
                    v->v_realdata = (double *) tmalloc(
                        npoints * sizeof (double));
                else
                    v->v_compdata = (complex *) tmalloc(
                        npoints * sizeof (complex));
                if (!i)
                    curpl->pl_scale = v;
                else {
                    (void) fgets(buf, BSIZE, fp);
                    s = buf;
                }
                (void) gettok(&s);  /* The index field. */
                if (t = gettok(&s))
                    v->v_name = t;
                else
                    fprintf(cp_err, 
                        "Error: bad var line %s\n",
                        buf);
                t = gettok(&s); /* The type name. */
                if (t)
                    v->v_type = ft_typnum(t);
                else
                    fprintf(cp_err, 
                        "Error: bad var line %s\n",
                        buf);
                
                /* Fix the name... */
                if (isdigit(*v->v_name) && (r = ft_typabbrev(v
                        ->v_type))) {
                    (void) sprintf(buf2, "%s(%s)", r,
                            v->v_name);
                    v->v_name = copy(buf2);
                }
                /* Now come the strange options... */
                while (t = gettok(&s)) {
                    if (ciprefix("min=", t)) {
                        if (sscanf(t + 4, "%lf",
                            &v->v_minsignal) != 1)
                            fprintf(cp_err,
                            "Error: bad arg %s\n",
                            t);
                        v->v_flags |= VF_MINGIVEN;
                    } else if (ciprefix("max=", t)) {
                        if (sscanf(t + 4, "%lf",
                            &v->v_maxsignal) != 1)
                            fprintf(cp_err,
                            "Error: bad arg %s\n",
                            t);
                        v->v_flags |= VF_MAXGIVEN;
                    } else if (ciprefix("color=", t)) {
                        v->v_defcolor = copy(t + 6);
                    } else if (ciprefix("scale=", t)) {
                        /* This is bad, but... */
                        v->v_scale = (struct dvec *)
                                copy(t + 6);
                    } else if (ciprefix("grid=", t)) {
                        v->v_gridtype = (GRIDTYPE)
                                scannum(t + 5);
                    } else if (ciprefix("plot=", t)) {
                        v->v_plottype = (PLOTTYPE)
                                scannum(t + 5);
                    } else if (ciprefix("dims=", t)) {
                        fixdims(v, t + 5);
                    } else {
                        fprintf(cp_err, 
                        "Warning: bad var param %s\n",
                            t);
                    }
                }
            }
        } else if (ciprefix("values:", buf) || 
                ciprefix("binary:", buf)) {
            if (!curpl) {
                fprintf(cp_err, "Error: no plot name given\n");
                plots = NULL;
                break;
            }
            /* We'd better reverse the dvec list now... */
            for (v = curpl->pl_dvecs, curpl->pl_dvecs = NULL; v;
                    v = nv) {
                nv = v->v_next;
                v->v_next = curpl->pl_dvecs;
                curpl->pl_dvecs = v;
            }
            /* And fix the scale pointers. */
            for (v = curpl->pl_dvecs; v; v = v->v_next)
                if (v->v_scale) {
                    for (nv = curpl->pl_dvecs; nv; nv =
                            nv->v_next)
                        if (cieq((char *) v->v_scale,
                                nv->v_name)) {
                            v->v_scale = nv;
                            break;
                        }
                    if (!nv) {
                        fprintf(cp_err,
                        "Error: no such vector %s\n",
                            (char *) v->v_scale);
                        v->v_scale = NULL;
                    }
                }
            for (i = 0; i < npoints; i++) {
                if ((*buf == 'v') || (*buf == 'V')) {
                (void) fscanf(fp, " %d", &j);
                    for (v = curpl->pl_dvecs; v; v = v->v_next)
                    if (flags & VF_REAL) {
                        if (fscanf(fp, " %lf", 
                        &v->v_realdata[i]) != 1)
                        fprintf(cp_err,
                        "Error: bad rawfile\n");
                    } else {
                        if (fscanf(fp, " %lf, %lf",
                        &realpart(&v->v_compdata[i]),
                        &imagpart(&v->v_compdata[i])) != 2)
                        fprintf(cp_err,
                        "Error: bad rawfile\n");
                }
                } else {
                    for (v = curpl->pl_dvecs; v; v = v->v_next)
                    if (flags & VF_REAL) {
                    if (fread((char *) &v->v_realdata[i],
                        sizeof (double), 1, fp) != 1)
                        fprintf(cp_err,
                        "Error: bad rawfile\n");
                    } else {
                    if (fread((char *) &v->v_compdata[i].
                        cx_real,
                        sizeof (double), 1, fp) != 1)
                        fprintf(cp_err,
                        "Error: bad rawfile\n");
                    if (fread((char *) &v->v_compdata[i].
                        cx_imag,
                        sizeof (double), 1, fp) != 1)
                        fprintf(cp_err,
                        "Error: bad rawfile\n");
                    }
                }
            }
        } else {
            s = buf;
            skip(s);
            if (*s) {
                fprintf(cp_err, 
            "Error: strange line in rawfile -- load aborted\n");
                return (NULL);
            }
        }
    }

    if (curpl) {    /* reverse commands list */
        for (wl=curpl->pl_commands,
                curpl->pl_commands=NULL; wl &&
                wl->wl_next; wl=nwl) {
            nwl = wl->wl_next;
            wl->wl_next = curpl->pl_commands;
            curpl->pl_commands = wl;
        }
    }

    /* Fix everything up nicely again. */
    cp_popcontrol();
    cp_curin = lastin;
    cp_curout = lastout;
    cp_curerr = lasterr;
    (void) fclose(fp);
    return (plots);
}

/* s is a string of the form d1,d2,d3... */

static void
fixdims(v, s)
    struct dvec *v;
    char *s;
{
    int i;

    v->v_numdims = 0;

    for (;;) {
        i = 0;
        while (isdigit(*s))
            i = 10 * i + *s++ - '0';
        if (i)
            v->v_dims[v->v_numdims++] = i;
        if (!*s)
            break;
        else
            s++;
    }
    return;
}

