
/* RCS Info: $Revision: 1.2 $ on $Date: 92/06/18 19:20:24 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/vectors.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Routines for dealing with the vector database.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"
#ifndef CMS
#include "FTEredirect.h"
#else  /* CMS */
#include "FTEredir.h"
#endif /* CMS */
#ifdef UNIX
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include "suffix.h"

static double boltz;
static double c;
static double e;
static double echarge;
static complex ii;
static double kelvin;
static double pi;
static double planck;
static int namecmp();
static int veccmp();
static struct dvec *findvec();
static struct dvec *sortvecs();

#ifdef notdef
static bool oct_file_type();
static bool binary_file_type();
#endif

/* Where 'constants' go when defined on initialization. */

static struct plot constantplot = {
    "Constant values", "Sat Aug 16 10:55:15 PDT 1986", "constants",
    "constants", NULL, NULL, NULL, NULL, NULL, NULL, true
} ;

struct plot *plot_cur = &constantplot;
struct plot *plot_list = &constantplot;
int plotl_changed;      /* true after a load */

int plot_num = 1;


/* gtri - begin - add function prototype for EVTfindvec */
struct dvec *EVTfindvec(char *node);
/* gtri - end   - add function prototype for EVTfindvec */


/* Load in a rawfile. */

void
ft_loadfile(file)
    char *file;
{
    struct plot *pl, *np, *pp;

    fprintf(cp_out, "Loading raw file '%s' ... ", file);
#ifdef notdef
    if (oct_file_type(file))
        pl = ft_octread(file);
    else
#endif
        pl = raw_read(file);
    if (pl)
        fprintf(cp_out, "done.\n\n");

    /* This is a minor annoyance -- we should reverse the plot list so
     * they get numbered in the correct order.
     */
    for (pp = pl, pl = NULL; pp; pp = np) {
        np = pp->pl_next;
        pp->pl_next = pl;
        pl = pp;
    }
    for (; pl; pl = np) {
        np = pl->pl_next;
        plot_add(pl);
        /* Don't want to get too many "plot not written" messages. */
        pl->pl_written = true;
    }
    plot_num++;
    plotl_changed = true;
    return;
}

void
plot_add(pl)
    struct plot *pl;
{
    struct dvec *v;
    struct plot *tp;
    char *s, buf[BSIZE];

    fprintf(cp_out, "Title:  %s\nName: %s\nDate: %s\n\n", pl->pl_title,
        pl->pl_name, pl->pl_date);
    
    if (plot_cur)
        plot_cur->pl_ccom = cp_kwswitch(CT_VECTOR, pl->pl_ccom);
    
    for (v = pl->pl_dvecs; v; v = v->v_next)
        cp_addkword(CT_VECTOR, v->v_name);
    cp_addkword(CT_VECTOR, "all");

    if (!(s = ft_plotabbrev(pl->pl_name)))
        s = "unknown";
    do {
        (void) sprintf(buf, "%s%d", s, plot_num);
        for (tp = plot_list; tp; tp = tp->pl_next)
            if (cieq(tp->pl_typename, buf)) {
                plot_num++;
                break;
            }
    } while (tp);

    pl->pl_typename = copy(buf);
    plot_new(pl);
    cp_addkword(CT_PLOT, buf);
    pl->pl_ccom = cp_kwswitch(CT_VECTOR, (char *) NULL);
    plot_setcur(pl->pl_typename);

    return;
}

/* Remove a vector from the database, if it is there. */

void
vec_remove(name)
    char *name;
{
    struct dvec *ov;

    for (ov = plot_cur->pl_dvecs; ov; ov = ov->v_next) {
        if (cieq(name, ov->v_name) && (ov->v_flags & VF_PERMANENT))
            break;
    }
    if (!ov)
        return;

    ov->v_flags &= ~VF_PERMANENT;

    /* Remove from the keyword list. */
    cp_remkword(CT_VECTOR, name);
    return;
}

/* Get a vector by name. This deals with v(1), etc. almost properly. Also,
 * it checks for pre-defined vectors.
 */

struct dvec *
vec_fromplot(word, plot)
    char *word;
    struct plot *plot;
{
    struct dvec *d;
    char buf[BSIZE], buf2[BSIZE], cc, *s;

    d = findvec(word, plot);
    if (!d) {
        (void) strcpy(buf, word);
        for (s = buf; *s; s++)
            if (isupper(*s))
                *s = tolower(*s);
        d = findvec(buf, plot);
    }
    if (!d) {
        (void) strcpy(buf, word);
        for (s = buf; *s; s++)
            if (islower(*s))
                *s = *s - 'a' + 'A';
        d = findvec(buf, plot);
    }

    /* scanf("%c(%s)" doesn't do what it should do. ) */
    if (!d && (sscanf(word, "%c(%s", /* ) */ &cc, buf) == 2) &&
        /* ( */ ((s = rindex(buf, ')')) != NULL) &&
            (*(s + 1) == '\0')) {
        *s = '\0';
        if (prefix("i(", /* ) */ word)) {
            /* Spice dependency... */
            (void) sprintf(buf2, "%s#branch", buf);
            (void) strcpy(buf, buf2);
        }
        d = findvec(buf, plot);
    }

    return (d);
}

/* This is the main lookup routine for names. The possible types of names are:
 *  name        An ordinary vector.
 *  plot.name   A vector from a particular plot.
 *  @device[parm]   A device parameter.
 *  @model[parm]    A model parameter.
 *  @param      A circuit parameter.
 * For the @ cases, we construct a dvec with length 1 to hold the value.
 * In the other two cases, either the plot or the name can be "all", a
 * wildcard.
 * The vector name may have imbedded dots -- if the first component is a plot
 * name, it is considered the plot, otherwise the current plot is used.
 */

#define SPECCHAR '@'

struct dvec *
vec_get(word)
    char *word;
{
    struct dvec *d, *end = NULL, *newv = NULL;
    struct plot *pl;
    char buf[BSIZE], *s, *wd, *name, *param;
    int i = 0;
    struct variable *vv;

    wd = word = copy(word);     /* Gets mangled below... */

    if (index(word, '.')) {
        /* Snag the plot... */
        for (i = 0, s = word; *s != '.'; i++, s++)
            buf[i] = *s;
        buf[i] = '\0';
        if (cieq(buf, "all")) {
            word = ++s;
            pl = NULL;  /* NULL pl signifies a wildcard. */
        } else {
            for (pl = plot_list; pl && !plot_prefix(buf, 
                    pl->pl_typename); pl = pl->pl_next)
                ;
            if (pl) {
                word = ++s;
            } else {
                /* This used to be an error... */
                pl = plot_cur;
            }
        }
    } else
        pl = plot_cur;

    if (pl) {
        d = vec_fromplot(word, pl);
        if (!d)
            d = vec_fromplot(word, &constantplot);
    } else {
        for (pl = plot_list; pl; pl = pl->pl_next) {
            if (cieq(pl->pl_typename, "constants"))
                continue;
            d = vec_fromplot(word, pl);
            if (d) {
                if (end)
                    end->v_link2 = d;
                else
                    newv = d;
                for (end = d; end->v_link2; end = end->v_link2)
                    ;
            }
        }
        d = newv;
        if (!d) {
            fprintf(cp_err, 
            "Error: plot wildcard (name %s) matches nothing\n",
                    word);
            return (NULL);
        }
    }
    if (!d && (*word == SPECCHAR)) {
        /* This is a special quantity... */
        if (ft_nutmeg) {
            fprintf(cp_err,
        "Error: circuit parameters only available with spice\n");
            return (false);
        }
        name = ++word;
        for (param = name; *param && (*param != '['); param++)
            ;
        if (*param) {
            *param++ = '\0';
            for (s = param; *s && *s != ']'; s++)
                ;
            *s = '\0';
        } else
            param = NULL;
        vv = if_getparam(ft_curckt->ci_ckt, &name, param, 0);
        if (!vv) {
            /* if_getparam will already have complained... */
            return (NULL);
        }
        tfree(wd);
        d = alloc(dvec);
        d->v_name = copy(word);
        d->v_type = SV_NOTYPE;
        d->v_flags |= VF_REAL;  /* No complex values yet... */
        d->v_realdata = (double *) tmalloc(sizeof (double));
        d->v_length = 1;
        *d->v_realdata = vv->va_real;
        vec_new(d);
        return (d);
    }

    tfree(wd);
    return (sortvecs(d));
}

/* Find a named vector in a plot. We are careful to copy the vector
 * if v_link2 is set, because otherwise we will get screwed up.
 */

static struct dvec *
findvec(word, pl)
    char *word;
    struct plot *pl;
{
    struct dvec *d, *newv = NULL, *end = NULL, *v;
    char buf[BSIZE];

    if (pl == NULL)
        return (NULL);

    if (cieq(word, "all")) {
        for (d = pl->pl_dvecs; d; d = d->v_next) {
            if (d->v_flags & VF_PERMANENT) {
                if (d->v_link2) {
                    v = vec_copy(d);
                    vec_new(v);
                } else
                    v = d;
                if (end)
                    end->v_link2 = v;
                else
                    newv = v;
                end = v;
            }
        }
        return (newv);
    }

    for (d = pl->pl_dvecs; d; d = d->v_next)
        if (cieq(word, d->v_name) && (d->v_flags & VF_PERMANENT))
            break;
    if (!d) {
        (void) sprintf(buf, "v(%s)", word);
        for (d = pl->pl_dvecs; d; d = d->v_next)
            if (cieq(buf, d->v_name) && (d->v_flags & VF_PERMANENT))
                break;
    }

/* gtri - begin - Add processing for getting event-driven vector */

    if(!d)
      d = EVTfindvec(word);

/* gtri - end   - Add processing for getting event-driven vector */

    if (d && d->v_link2) {
        d = vec_copy(d);
        vec_new(d);
    }

    return (d);
}

/* Execute the commands for a plot. This is done whenever a plot becomes
 * the current plot.
 */

void
plot_docoms(wl)
    wordlist *wl;
{
    bool inter;

    inter = cp_interactive;
    cp_interactive = false;
    while (wl) {
        (void) cp_evloop(wl->wl_word);
        wl = wl->wl_next;
    }
    cp_resetcontrol();
    cp_interactive = inter;
    return;
}

/* Create a copy of a vector. */

struct dvec *
vec_copy(v)
    struct dvec *v;
{
    struct dvec *nv;
    int i;
    
    if (!v)
        return (NULL);
    nv = alloc(dvec);
    nv->v_name = copy(v->v_name);
    nv->v_type = v->v_type;
    nv->v_flags = v->v_flags & ~VF_PERMANENT;
    nv->v_length = v->v_length;
    nv->v_plot = v->v_plot;
    nv->v_minsignal = v->v_minsignal;
    nv->v_maxsignal = v->v_maxsignal;
    nv->v_defcolor = v->v_defcolor;
    nv->v_gridtype = v->v_gridtype;
    nv->v_plottype = v->v_plottype;
    nv->v_scale = v->v_scale;
    nv->v_numdims = v->v_numdims;
    for (i = 0; i < v->v_numdims; i++)
        nv->v_dims[i] = v->v_dims[i];
    if (isreal(v)) {
        nv->v_realdata = (double *) tmalloc(sizeof (double) *
                v->v_length);
        bcopy((char *) v->v_realdata, (char *) nv->v_realdata, 
                sizeof (double) * v->v_length);
    } else {
        nv->v_compdata = (complex *) tmalloc(sizeof (complex) *
                v->v_length);
        bcopy((char *) v->v_compdata, (char *) nv->v_compdata, 
                sizeof (complex) * v->v_length);
    }
    return (nv);
}

/* Create a new plot structure. This just fills in the typename and sets up
 * the ccom struct.
 */

struct plot *
plot_alloc(name)
    char *name;
{
    struct plot *pl = alloc(plot), *tp;
    char *s;
    char buf[BSIZE];

    if (!(s = ft_plotabbrev(name)))
        s = "unknown";
    do {
        (void) sprintf(buf, "%s%d", s, plot_num);
        for (tp = plot_list; tp; tp = tp->pl_next)
            if (cieq(tp->pl_typename, buf)) {
                plot_num++;
                break;
            }
    } while (tp);
    pl->pl_typename = copy(buf);
    cp_addkword(CT_PLOT, buf);
    s = cp_kwswitch(CT_VECTOR, (char *) NULL);
    cp_addkword(CT_VECTOR, "all");
    pl->pl_ccom = cp_kwswitch(CT_VECTOR, s);
    return (pl);
}

/* Stick a new vector in the proper place in the plot list. */

void
vec_new(d)
    struct dvec *d;
{
#ifdef FTEDEBUG
    if (ft_vecdb)
        fprintf(cp_err, "new vector %s\n", d->v_name);
#endif
    /* Note that this can't happen. */
    if (plot_cur == NULL) {
#ifdef notdef
        /* Must create a new plot now. */
        plot_cur = plot_alloc((char *) NULL);
        plot_cur->pl_title = copy("Generic plot...");
        plot_cur->pl_date = copy(datestring());
        plot_cur->pl_name = copy("Anonymous");
        plot_new(plot_cur);
        (void) cp_kwswitch(CT_VECTOR, plot_cur->pl_ccom);
#endif
        fprintf(cp_err, "vec_new: Internal Error: no cur plot\n");
    }
    if ((d->v_flags & VF_PERMANENT) && (plot_cur->pl_scale == NULL))
        plot_cur->pl_scale = d;
    if (!d->v_plot)
        d->v_plot = plot_cur;
    d->v_next = d->v_plot->pl_dvecs;
    d->v_plot->pl_dvecs = d;
    return;
}

/* Because of the way that all vectors, including temporary vectors,
 * are linked together under the current plot, they can often be
 * left lying around. This gets rid of all vectors that don't have
 * the permanent flag set. Also, for the remaining vectors, it
 * clears the v_link2 pointer.
 */

void
vec_gc()
{
    struct dvec *d, *nd;
    struct plot *pl;

    for (pl = plot_list; pl; pl = pl->pl_next)
        for (d = pl->pl_dvecs; d; d = nd) {
            nd = d->v_next;
            if (!(d->v_flags & VF_PERMANENT)) {
                if (ft_vecdb)
                    fprintf(cp_err, 
                        "vec_gc: throwing away %s.%s\n",
                        pl->pl_typename, d->v_name);
                vec_free(d);
            }
        }

    for (pl = plot_list; pl; pl = pl->pl_next)
        for (d = pl->pl_dvecs; d; d = d->v_next)
            d->v_link2 = NULL;

    return;
}

/* Free a dvector. This is sort of a pain because we also have to make sure
 * that it has been unlinked from its plot structure. If the name of the
 * vector is NULL, then we have already freed it so don't try again. (This
 * situation can happen with user-defined functions.) Note that this depends
 * on our having tfree set its argument to NULL. Note that if all the vectors
 * in a plot are gone it stays around...
 */

void
vec_free(v)
    struct dvec *v;
{
    struct plot *pl;
    struct dvec *lv;

    if ((v == NULL) || (v->v_name == NULL))
        return;
    pl = v->v_plot;

    /* Now we have to take this dvec out of the plot list. */
    if (pl == NULL)
        fprintf(cp_err, "vec_free: Internal Error: plot ptr is 0\n");
    if (pl->pl_dvecs == v)
        pl->pl_dvecs = v->v_next;
    else {
        for (lv = pl->pl_dvecs; lv->v_next; lv = lv->v_next)
            if (lv->v_next == v)
                break;
        if (lv->v_next == NULL)
            fprintf(cp_err, 
                "vec_free: Internal Error: %s not in plot\n",
                    v->v_name);
        lv->v_next = v->v_next;
    }
    if (pl->pl_scale == v) {
        if (pl->pl_dvecs)
            pl->pl_scale = pl->pl_dvecs;    /* Random one... */
        else
            pl->pl_scale = NULL;
    }
    tfree(v->v_name);
    if (isreal(v)) {
        tfree(v->v_realdata);
    } else {
        tfree(v->v_compdata);
    }
    tfree(v);
    return;
}

/* This is something we do in a few places...  Since vectors get copied a lot,
 * we can't just compare pointers to tell if two vectors are 'really' the same.
 */

bool
vec_eq(v1, v2)
    struct dvec *v1, *v2;
{
    char *s1, *s2;

    if (v1->v_plot != v2->v_plot)
        return (false);
    
    s1 = vec_basename(v1);
    s2 = vec_basename(v2);

    if (cieq(s1, s2))
        return (true);
    else
        return (false);
}

/* Return the name of the vector with the plot prefix stripped off.  This
 * is no longer trivial since '.' doesn't always mean 'plot prefix'.
 */

char *
vec_basename(v)
    struct dvec *v;
{
    char buf[BSIZE], *t, *s;
    int i;

    if (index(v->v_name, '.')) {
        for (t = v->v_name, i = 0; *t; t++)
            buf[i++] = *t;
        buf[i] = '\0';
        if (cieq(v->v_plot->pl_typename, buf))
            (void) strcpy(buf, t + 1);
        else
            (void) strcpy(buf, v->v_name);
    } else
        (void) strcpy(buf, v->v_name);
    
    for (t = buf; *t; t++)
        if (isupper(*t))
            *t = tolower(*t);
    for (t = buf; isspace(*t); t++)
        ;
    s = t;
    for (t = s; *t; t++)
        ;
    while ((t > s) && isspace(t[-1]))
        *--t = '\0';
    return (copy(s));
}

/* Sort all the vectors in d, first by plot name and then by vector name.
 * Do the right thing with numbers.
 */

static struct dvec *
sortvecs(d)
    struct dvec *d;
{
    struct dvec **array, *t;
    int i, j;

    for (t = d, i = 0; t; t = t->v_link2)
        i++;
    if (i < 2)
        return (d);
    array = (struct dvec **) tmalloc(i * sizeof (struct dvec *));
    for (t = d, i = 0; t; t = t->v_link2)
        array[i++] = t;
    
    qsort((char *) array, i, sizeof (struct dvec *), veccmp);

    /* Now string everything back together... */
    for (j = 0; j < i - 1; j++)
        array[j]->v_link2 = array[j + 1];
    array[j]->v_link2 = NULL;
    d = array[0];
    tfree(array);
    return (d);
}

static int
veccmp(d1, d2)
    struct dvec **d1, **d2;
{
    int i;

    if ((i = namecmp((*d1)->v_plot->pl_typename,
            (*d2)->v_plot->pl_typename)) != 0)
        return (i);
    return (namecmp((*d1)->v_name, (*d2)->v_name));
}

/* If there are imbedded numeric strings, compare them numerically, not
 * alphabetically.
 */

static int
namecmp(s, t)
    char *s, *t;
{
    int i, j;

    for (;;) {
        while ((*s == *t) && !isdigit(*s) && *s)
            s++, t++;
        if (!*s)
            return (0);
        if ((*s != *t) && (!isdigit(*s) || !isdigit(*t)))
            return (*s - *t);
        
        /* The beginning of a number... Grab the two numbers 
         * and then compare them...
         */
        for (i = 0; isdigit(*s); s++)
            i = i * 10 + *s - '0';
        for (j = 0; isdigit(*t); t++)
            j = j * 10 + *t - '0';
        
        if (i != j)
            return (i - j);
    }
}

/* Check the type of a file. (Either oct, ascii, or binary)  This returns
 * true if oct - can be fooled if you give it another directory name
 */

#ifdef notdef

static bool
oct_file_type(file)
    char *file;
{
    struct stat buf;
    (void) stat(file,&buf);
    if( (buf.st_mode & S_IFMT) == S_IFDIR) {
        return(true);
    } else {
        return(false);
    }
}

/* Check if file is in (the new) binary format.  */

bool
binary_file_type(file)
    char *file;
{
    FILE *fp;

    if (!(fp = fopen(file, "r"))) {
        perror(file);
        return (false);
    }

    if (getc(fp) == BMAGIC) {
        (void) fclose(fp);
        return (true);
    } else {
        (void) fclose(fp);
        return (false);
    }

}

#endif

/* Make a plot the current one.  This gets called by cp_usrset() when one
 * does a 'set curplot = name'.
 */

void
plot_setcur(name)
    char *name;
{
    struct plot *pl;

    if (cieq(name, "new")) {
        pl = plot_alloc("unknown");
        pl->pl_title = copy("Anonymous");
        pl->pl_name = copy("unknown");
        pl->pl_date = datestring();
        plot_new(pl);
        plot_cur = pl;
        return;
    }
    for (pl = plot_list; pl; pl = pl->pl_next)
        if (plot_prefix(name, pl->pl_typename))
            break;
    if (!pl) {
        fprintf(cp_err, "Error: no such plot named %s\n", name);
        return;
    }
    if (plot_cur)
        plot_cur->pl_ccom = cp_kwswitch(CT_VECTOR, pl->pl_ccom);
    plot_cur = pl;
    return;
}

/* Add a plot to the plot list. This is different from plot_add() in that
 * all this does is update the list and the variable $plots.
 */

void
plot_new(pl)
    struct plot *pl;
{
    pl->pl_next = plot_list;
    plot_list = pl;

    return;
}

/* This routine takes a multi-dimensional vector and turns it into a family
 * of 1-d vectors, linked together with v_link2.  It is here so that plot
 * can do intelligent things.
 */

struct dvec *
vec_mkfamily(v)
    struct dvec *v;
{
    int size, numvecs, i, j, count[MAXDIMS];
    struct dvec *vecs, *d;
    char buf[BSIZE];

    if (v->v_numdims < 2)
        return (v);

    size = v->v_dims[v->v_numdims - 1];
    for (i = 0, numvecs = 1; i < v->v_numdims - 1; i++)
        numvecs *= v->v_dims[0];
    for (i = 0, vecs = d = NULL; i < numvecs; i++) {
        if (vecs)
            d = d->v_link2 = alloc(dvec);
        else
            d = vecs = alloc(dvec);
    }
    for (i = 0; i < MAXDIMS; i++)
        count[i] = 0;
    for (d = vecs, j = 0; ; j++, d = d->v_link2) {
        (void) strcpy(buf, v->v_name);
        for (i = 0; i < v->v_numdims - 1; i++)
            (void) sprintf(buf + strlen(buf), "[%d]", count[i]);
        d->v_name = copy(buf);
        d->v_type = v->v_type;
        d->v_flags = v->v_flags;
        d->v_minsignal = v->v_minsignal;
        d->v_maxsignal = v->v_maxsignal;
        d->v_gridtype = v->v_gridtype;
        d->v_plottype = v->v_plottype;
        d->v_scale = v->v_scale;
        /* Don't copy the default color, since there will be many
         * of these things...
         */
        d->v_numdims = 1;
        d->v_length = size;

        if (isreal(v)) {
            d->v_realdata = (double *) tmalloc(sizeof (double) *
                    size);
            bcopy((char *) v->v_realdata + size * j,
                    (char *) d->v_realdata, 
                    size * sizeof (double));
        } else {
            d->v_compdata = (complex *) tmalloc(sizeof (complex) *
                    size);
            bcopy((char *) v->v_compdata + size * j,
                    (char *) d->v_compdata,
                    size * sizeof (complex));
        }

        for (i = v->v_numdims - 2; i >= 0; i--) {
            if (count[i]++ < v->v_dims[i])
                break;
            else
                count[i] = 0;
        }
        if (i < 0)
            break;
    }
    for (d = vecs; d; d = d->v_link2)
        vec_new(d);

    return (vecs);
}

/* This function will match "op" with "op1", but not "op1" with "op12". */

bool
plot_prefix(pre, str)
    char *pre, *str;
{
    if (!*pre)
        return (true);
    while (*pre && *str) {
        if (*pre != *str)
            break;
        pre++; str++;
    }
    if (*pre || (*str && isdigit(pre[-1])))
        return (false);
    else
        return (true);
}

