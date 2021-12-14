
/* RCS Info: $Revision: 1.2 $ on $Date: 92/12/23 11:29:30 $
 *           $Source: //carob/usr/local_carob/xspice-1-0/src/sim/FTE/RCS/interpolate.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Polynomial interpolation code.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"
#include "suffix.h"

/* Interpolate data from oscale to nscale. data is assumed to be olen long,
 * ndata will be nlen long. Returns false if the scales are too strange
 * to deal with.  Note that we are guaranteed that either both scales are
 * strictly increasing or both are strictly decreasing.
 */

static int putinterval();
static void lincopy();
static void printmat();

bool
ft_interpolate(data, ndata, oscale, olen, nscale, nlen, degree)
    double *data, *ndata, *oscale, *nscale;
{
    double *result, *scratch, *xdata, *ydata;
    int sign, lastone, i, l;

    if ((olen < 2) || (nlen < 2)) {
        fprintf(cp_err, "Error: lengths too small to interpolate.\n");
        return (false);
    }
    if ((degree < 1) || (degree > olen)) {
        fprintf(cp_err, "Error: degree is %d, can't interpolate...\n",
                degree);
        return (false);
    }

    if (oscale[1] < oscale[0])
        sign = -1;
    else
        sign = 1;

    scratch = (double *) tmalloc((degree + 1) * (degree + 2) * 
            sizeof (double));
    result = (double *) tmalloc((degree + 1) * sizeof (double));
    xdata = (double *) tmalloc((degree + 1) * sizeof (double));
    ydata = (double *) tmalloc((degree + 1) * sizeof (double));

    /* Deal with the first degree pieces... */
    bcopy((char *) data, (char *) ydata, (degree + 1) * sizeof (double));
    bcopy((char *) oscale, (char *) xdata, (degree + 1) * sizeof (double));

    while (!ft_polyfit(xdata, ydata, result, degree, scratch)) {
        /* If it doesn't work this time, bump the interpolation
         * degree down by one...
         */

        if (--degree == 0) {
            fprintf(cp_err, "ft_interpolate: Internal Error...\n");
            return (false);
        }


    }

    /* Add this part of the curve... What we do is evaluate the polynomial
     * at those points between the last one and the one that is greatest,
     * without being greater than the leftmost old scale point, or least
     * if the scale is decreasing at the end of the interval we are looking
     * at.
     */
    lastone = -1;
    for (i = 0; i < degree; i++) {
        lastone = putinterval(result, degree, ndata, lastone, 
                    nscale, nlen, xdata[i], sign);
    }

    /* Now plot the rest, piece by piece... l is the 
     * last element under consideration.
     */
    for (l = degree + 1; l < olen; l++) {

        /* Shift the old stuff by one and get another value. */
        for (i = 0; i < degree; i++) {
            xdata[i] = xdata[i + 1];
            ydata[i] = ydata[i + 1];
        }
        ydata[i] = data[l];
        xdata[i] = oscale[l];

        while (!ft_polyfit(xdata, ydata, result, degree, scratch)) {
            if (--degree == 0) {
                fprintf(cp_err, 
                    "interpolate: Internal Error...\n");
                return (false);
            }
        }
        lastone = putinterval(result, degree, ndata, lastone, 
                    nscale, nlen, xdata[i], sign);
    }
    tfree(scratch);
    tfree(xdata);
    tfree(ydata);
    tfree(result);
    return (true);
}

/* Takes n of doubles, and fills in result with the n coefficients 
 * of the polynomial that will fit them. It also takes a pointer to an
 * array of n ^ 2 + n doubles to use for scratch -- we want to make this
 * fast and avoid doing mallocs for each call.
 */

bool
ft_polyfit(xdata, ydata, result, n, scratch)
    double *xdata, *ydata, *result, *scratch;
    register int n;
{
    register double *mat1 = scratch;
    register int l, k, j, i;
    register double *mat2 = scratch + n * n;
    double d;

/*
fprintf(cp_err, "n = %d, xdata = ( ", n);
   for (i = 0; i < n; i++)
    fprintf(cp_err, "%G ", xdata[i]);
   fprintf(cp_err, ")\n");
   fprintf(cp_err, "ydata = ( ");
   for (i = 0; i < n; i++)
    fprintf(cp_err, "%G ", ydata[i]);
   fprintf(cp_err, ")\n");
*/

    /* need to do this for n+1, because of ft_peval J.H. */
    bzero((char *) result, (n+1) * sizeof(double));
    bzero((char *) mat1, n * n * sizeof (double));
    bcopy((char *) ydata, (char *) mat2, n * sizeof (double));

    /* Fill in the first matrix with the proper things... */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            l = i * n + j;
            mat1[l] = 1.0;
            for (k = 0; k < j; k++)
                mat1[l] *= xdata[i];
        }
    }

    /* Do Gauss-Jordan elimination on mat1. */
    for (i = 0; i < n; i++) {
      int lindex;
      double largest;
        /* choose largest pivot */
        for (j=i, largest = mat1[i * n + i], lindex = i; j < n; j++) {
          if (fabs(mat1[j * n + i]) > largest) {
            largest = fabs(mat1[j * n + i]);
            lindex = j;
          }
        }
        if (lindex != i) {
          /* swap rows i and lindex */
          for (k = 0; k < n; k++) {
            d = mat1[i * n + k];
            mat1[i * n + k] = mat1[lindex * n + k];
            mat1[lindex * n + k] = d;
          }
          d = mat2[i];
          mat2[i] = mat2[lindex];
          mat2[lindex] = d;
        }
#ifdef notdef
        if (mat1[i * n + i] == 0.0)
            for (j = i; j < n; j++)
                if (mat1[j * n + i] != 0.0) {
                    /* Swap rows i and j. */
                    for (k = 0; k < n; k++) {
                        d = mat1[i * n + k];
                        mat1[i * n + k] =
                            mat1[j * n + k];
                        mat1[j * n + k] = d;
                    }
                    d = mat2[i];
                    mat2[i] = mat2[j];
                    mat2[j] = d;
                    break;
                }
#endif
        /* Make sure we have a non-zero pivot. */
        if (mat1[i * n + i] == 0.0) {
            /* Well, this should be rotated... */
            return (false);
        }
        for (j = i + 1; j < n; j++) {
            d = mat1[j * n + i] / mat1[i * n + i];
            for (k = 0; k < n; k++)
                mat1[j * n + k] -= d * mat1[i * n + k];
            mat2[j] -= d * mat2[i];
        }
    }

    for (i = n - 1; i > 0; i--)
        for (j = i - 1; j >= 0; j--) {
            d = mat1[j * n + i] / mat1[i * n + i];
            for (k = 0; k < n; k++)
                mat1[j * n + k] -= 
                        d * mat1[i * n + k];
            mat2[j] -= d * mat2[i];
        }
    
    /* Now write the stuff into the result vector. */
    for (i = 0; i < n; i++) {
        result[i] = mat2[i] / mat1[i * n + i];
        /* printf(cp_err, "result[%d] = %G\n", i, result[i]);*/
    }

#define ABS_TOL 0.001
#define REL_TOL 0.001

    /* Let's check and make sure the coefficients are ok.  If they aren't,
     * just return false.  This is not the best way to do it...
     */
    for (i = 0; i < n; i++) {
        d = ft_peval(xdata[i], result, n);
        if (fabs(d - ydata[i]) > ABS_TOL) {
            /*
            fprintf(cp_err,
                "Error: polyfit: x = %le, y = %le, int = %le\n",
                    xdata[i], ydata[i], d);
            printmat("mat1", mat1, n, n);
            printmat("mat2", mat2, n, 1);
            */
            return (false);
        } else if (fabs(d - ydata[i]) / (fabs(d) > ABS_TOL ? fabs(d) :
                ABS_TOL) > REL_TOL) {
            /*
            fprintf(cp_err,
                "Error: polyfit: x = %le, y = %le, int = %le\n",
                    xdata[i], ydata[i], d);
            printmat("mat1", mat1, n, n);
            printmat("mat2", mat2, n, 1);
            */
            return (false);
        }
    }

    return (true);
}

/* Returns the index of the last element that was calculated. oval is the
 * value of the old scale at the end of the interval that is being interpolated
 * from, and sign is 1 if the old scale was increasing, and -1 if it was
 * decreasing.
 */

static int
putinterval(poly, degree, nvec, last, nscale, nlen, oval, sign)
    double *poly, *nvec, *nscale, oval;
{
    int end, i;

    /* See how far we have to go... */
    for (end = last + 1; end < nlen; end++)
        if (nscale[end] * sign > oval * sign)
            break;
    end--;

    for (i = last + 1; i <= end; i++)
        nvec[i] = ft_peval(nscale[i], poly, degree);
    return (end);
}

/* gtri - begin - Eliminate routine so compiler won't complain */
/*
static void
printmat(name, mat, m, n)
    char *name;
    double *mat;
{
    int i, j;

    printf("\n\r=== Matrix: %s ===\n\r", name);
    for (i = 0; i < m; i++) {
        printf(" | ");
        for (j = 0; j < n; j++)
            printf("%G ", mat[i * n + j]);
        printf("|\n\r");
    }
    printf("===\n\r");
    return;
}
*/
/* gtri - end - Eliminate routine so compiler won't complain */

/* This should be a macro or be asm coded if possible. */

double
ft_peval(pt, coeffs, degree)
    double pt, *coeffs;
    register int degree;
{
    register int i, j;
    double d = 0.0, f;

    /* fprintf(cp_err, "peval "); 
    for (i = 0; i <= degree; i++) 
        fprintf(cp_err, "%G ", coeffs[i]);
    fprintf(cp_err, "at %G", pt);
    */
    for (i = 0; i <= degree; i++) {
        f = 1.0;
        for (j = 0; j < i; j++)
            f *= pt;
        d += f * coeffs[i];
    }
    /* fprintf(cp_err, " = %G\n", d);*/
    return (d);
}

/* Interpolate all the vectors in a plot to a linear time scale, which
 * we determine by looking at the transient parameters in the CKT struct.
 */

void
com_linearize(wl)
    wordlist *wl;
{
    double tstart, tstop, tstep, d;
    struct plot *new, *old;
    struct dvec *newtime, *v;
    struct dvec *oldtime;
    int len, i;
    char buf[BSIZE];

    if (!ft_curckt || !ft_curckt->ci_ckt ||
        !if_tranparams(ft_curckt, &tstart, &tstop, &tstep)) {
        fprintf(cp_err, 
            "Error: can't get transient parameters from circuit\n");
        return;
    }
    if (((tstop - tstart) * tstep <= 0.0) || ((tstop - tstart) < tstep)) {
        fprintf(cp_err, 
         "Error: bad parameters -- start = %G, stop = %G, step = %G\n",
                tstart, tstop, tstep);
        return;
    }
    if (!plot_cur || !plot_cur->pl_dvecs || !plot_cur->pl_scale) {
        fprintf(cp_err, "Error: no vectors available\n");
        return;
    }
    if (!isreal(plot_cur->pl_scale)) {
        fprintf(cp_err, "Error: non-real time scale for %s\n",
                plot_cur->pl_typename);
        return;
    }
    if (!ciprefix("tran", plot_cur->pl_typename)) {
        fprintf(cp_err, "Error: plot must be a transient analysis\n");
        return;
    }
    old = plot_cur;
    oldtime = old->pl_scale;
    new = plot_alloc("transient");
    (void) sprintf(buf, "%s (linearized)", old->pl_name);
    new->pl_name = copy(buf);
    new->pl_title = copy(old->pl_title);
    new->pl_date = copy(old->pl_date);
    new->pl_next = plot_list;
    plot_new(new);
    plot_setcur(new->pl_typename);
    plot_list = new;
    len = (tstop - tstart) / tstep + 1.5;
    newtime = alloc(dvec);
    newtime->v_name = copy(oldtime->v_name);
    newtime->v_type = oldtime->v_type;
    newtime->v_flags = oldtime->v_flags;
    newtime->v_flags |= VF_PERMANENT;
    newtime->v_length = len;
    newtime->v_plot = new;
    newtime->v_realdata = (double *) tmalloc(len * sizeof (double));
    for (i = 0, d = tstart; i < len; i++, d += tstep)
        newtime->v_realdata[i] = d;
    new->pl_scale = new->pl_dvecs = newtime;

    if (wl) {
        while (wl) {
            v = vec_fromplot(wl->wl_word, old);
            if (!v) {
                fprintf(cp_err, "Error: no such vector %s\n",
                        wl->wl_word);
                continue;
            }
            lincopy(v, newtime->v_realdata, len, oldtime);
            wl = wl->wl_next;
        }
    } else {
        for (v = old->pl_dvecs; v; v = v->v_next) {
            if (v == old->pl_scale)
                continue;
            lincopy(v, newtime->v_realdata, len, oldtime);
        }
    }
    return;

}

static void
lincopy(ov, newscale, newlen, oldscale)
    struct dvec *ov, *oldscale;
    double *newscale;
    int newlen;
{
    struct dvec *v;
    double *nd;

    if (!isreal(ov)) {
        fprintf(cp_err, "Warning: %s is not real\n", ov->v_name);
        return;
    }
    if (ov->v_length < oldscale->v_length) {
        fprintf(cp_err, "Warning: %s is too short\n", ov->v_name);
        return;
    }
    v = alloc(dvec);
    v->v_name = copy(ov->v_name);
    v->v_type = ov->v_type;
    v->v_flags = ov->v_flags;
    v->v_flags |= VF_PERMANENT;
    v->v_length = newlen;

    nd = (double *) tmalloc(newlen * sizeof (double));
    if (!ft_interpolate(ov->v_realdata, nd, oldscale->v_realdata,
            oldscale->v_length, newscale, newlen, 1)) {
        fprintf(cp_err, "Error: can't interpolate %s\n", ov->v_name);
        return;
    }
    v->v_realdata = nd;
    vec_new(v);
    return;
}

