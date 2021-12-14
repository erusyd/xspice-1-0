
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:11:10 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/cmath4.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Routines to do complex mathematical functions. These routines require
 * the -lm libraries. We sacrifice a lot of space to be able
 * to avoid having to do a seperate call for every vector element,
 * but it pays off in time savings.  These routines should never
 * allow FPE's to happen.
 *
 * Complex functions are called as follows:
 *  cx_something(data, type, length, &newlength, &newtype),
 *  and return a char * that is cast to complex or double.
 *
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"
#include "FTEcmath.h"
#include "suffix.h"

char *
cx_and(data1, data2, datatype1, datatype2, length)
    char *data1, *data2;
    short datatype1, datatype2;
{
    double *dd1 = (double *) data1;
    double *dd2 = (double *) data2;
    double *d;
    complex *cc1 = (complex *) data1;
    complex *cc2 = (complex *) data2;
    complex c1, c2;
    int i;

    d = alloc_d(length);
    if ((datatype1 == VF_REAL) && (datatype2 == VF_REAL)) {
        for (i = 0; i < length; i++)
            d[i] = dd1[i] && dd2[i];
    } else {
        for (i = 0; i < length; i++) {
            if (datatype1 == VF_REAL) {
                realpart(&c1) = dd1[i];
                imagpart(&c1) = 0.0;
            } else {
                realpart(&c1) = realpart(&cc1[i]);
                imagpart(&c1) = imagpart(&cc1[i]);
            }
            if (datatype2 == VF_REAL) {
                realpart(&c2) = dd2[i];
                imagpart(&c2) = 0.0;
            } else {
                realpart(&c2) = realpart(&cc2[i]);
                imagpart(&c2) = imagpart(&cc2[i]);
            }
            d[i] = ((realpart(&c1) && realpart(&c2)) &&
                (imagpart(&c1) && imagpart(&c2)));
        }
    }
    return ((char *) d);
}

char *
cx_or(data1, data2, datatype1, datatype2, length)
    char *data1, *data2;
    short datatype1, datatype2;
{
    double *dd1 = (double *) data1;
    double *dd2 = (double *) data2;
    double *d;
    complex *cc1 = (complex *) data1;
    complex *cc2 = (complex *) data2;
    complex c1, c2;
    int i;

    d = alloc_d(length);
    if ((datatype1 == VF_REAL) && (datatype2 == VF_REAL)) {
        for (i = 0; i < length; i++)
            d[i] = dd1[i] || dd2[i];
    } else {
        for (i = 0; i < length; i++) {
            if (datatype1 == VF_REAL) {
                realpart(&c1) = dd1[i];
                imagpart(&c1) = 0.0;
            } else {
                realpart(&c1) = realpart(&cc1[i]);
                imagpart(&c1) = imagpart(&cc1[i]);
            }
            if (datatype2 == VF_REAL) {
                realpart(&c2) = dd2[i];
                imagpart(&c2) = 0.0;
            } else {
                realpart(&c2) = realpart(&cc2[i]);
                imagpart(&c2) = imagpart(&cc2[i]);
            }
            d[i] = ((realpart(&c1) || realpart(&c2)) &&
                (imagpart(&c1) || imagpart(&c2)));
        }
    }
    return ((char *) d);
}

char *
cx_not(data, type, length, newlength, newtype)
    char *data;
    short type;
    int length;
    int *newlength;
    short *newtype;
{
    double *d;
    double *dd = (double *) data;
    complex *cc = (complex *) data;
    int i;

    d = alloc_d(length);
    *newtype = VF_REAL;
    *newlength = length;
    if (type == VF_COMPLEX) {
        for (i = 0; i < length; i++) {
            /* gcc doens't like !double */
            d[i] = realpart(&cc[i]) ? 0 : 1;
            d[i] = imagpart(&cc[i]) ? 0 : 1;
        }
    } else {
        for (i = 0; i < length; i++)
            d[i] = ! dd[i];
    }
    return ((char *) d);
}

/* This is a strange function. What we do is fit a polynomial to the
 * curve, of degree $polydegree, and then evaluate it at the points
 * in the time scale.  What we do is this: for every set of points that
 * we fit a polynomial to, fill in as much of the new vector as we can
 * (i.e, between the last value of the old scale we went from to this
 * one). At the ends we just use what we have...  We have to detect
 * badness here too...
 * Note that we pass arguments differently for this one cx_ function...
 */

char *
cx_interpolate(data, type, length, newlength, newtype, pl, newpl)
    char *data;
    short type;
    int length;
    int *newlength;
    short *newtype;
    struct plot *pl, *newpl;
{
    struct dvec *ns, *os;
    double *d;
    int degree;
    register int i, oincreasing = 1, nincreasing = 1;

    /* First do some sanity checks. */
    if (!pl || !pl->pl_scale || !newpl || !newpl->pl_scale) {
        fprintf(cp_err, "Internal error: cx_interpolate: bad scale\n");
        return (NULL);
    }
    ns = newpl->pl_scale;
    os = pl->pl_scale;
    if (iscomplex(ns)) {
        fprintf(cp_err, "Error: new scale has complex data\n");
        return (NULL);
        /*
        for (i = ns->v_length - 1; i >= 0; i--)
            if (imagpart(&ns->v_compdata[i])) {
                fprintf(cp_err, 
                    "Error: new scale has complex data\n");
                return (NULL);
            }
        osbuf = alloc_d(olen);
        */
    }
    if (iscomplex(os)) {
        fprintf(cp_err, "Error: old scale has complex data\n");
        return (NULL);
        /*
        for (i = os->v_length - 1; i >= 0; i--)
            if (imagpart(&os->v_compdata[i])) {
                fprintf(cp_err, 
                    "Error: old scale has complex data\n");
                return (NULL);
            }
        nsbuf = alloc_d(nlen);
        */
    }

    if (length != os->v_length) {
        fprintf(cp_err, "Error: lengths don't match\n");
        return (NULL);
    }
    if (type != VF_REAL) {
        fprintf(cp_err, "Error: argument has complex data\n");
        return (NULL);
    }

    /* Now make sure that either both scales are strictly increasing or
     * both are strictly decreasing.
     */
    if (os->v_realdata[0] < os->v_realdata[1])
        oincreasing = true;
    else
        oincreasing = false;
    for (i = 0; i < os->v_length - 1; i++)
        if ((os->v_realdata[i] < os->v_realdata[i + 1])
                != oincreasing) {
            fprintf(cp_err, "Error: old scale not monotonic\n");
            return (NULL);
        }
    if (ns->v_realdata[0] < ns->v_realdata[1])
        nincreasing = true;
    else
        nincreasing = false;
    for (i = 0; i < ns->v_length - 1; i++)
        if ((ns->v_realdata[i] < ns->v_realdata[i + 1])
                != nincreasing) {
            fprintf(cp_err, "Error: new scale not monotonic\n");
            return (NULL);
        }

    *newtype = VF_REAL;
    *newlength = ns->v_length;
    d = alloc_d(ns->v_length);

    if (!cp_getvar("polydegree", VT_NUM, (char *) &degree))
        degree = 1;

    if (!ft_interpolate((double *) data, d, os->v_realdata, os->v_length,
            ns->v_realdata, ns->v_length, degree)) {
        return (NULL);
    }

    return ((char *) d);
}

