
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:11:02 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/clip.c,v $
 * Copyright (c) 1986 Wayne A. Christopyher, U. C. Berkeley CAD Group
 *  Permission is granted to do anything with this code except sell it
 *  or remove this message.
 *
 * Some routines to do clipping of polygons, etc to boxes.  Most of this code
 * was rescued from MFB:
 *  Copyright -C- 1982 Giles Billingsley
 *  sccsid "@(#)mfbclip.c   1.2  12/21/83"
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "suffix.h"

#define POLYGONBUFSIZE 512
#define PI 3.1415926535897932384

#define CODEMINX 1
#define CODEMINY 2
#define CODEMAXX 4
#define CODEMAXY 8
#define CODE(x,y,c)  c = 0;\
                     if (x < l)\
                         c = CODEMINX;\
                     else if (x > r)\
                         c = CODEMAXX;\
                     if (y < b)\
                         c |= CODEMINY;\
                     else if (y > t)\
                         c |= CODEMAXY;

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define SWAPINT(a, b) { int xxxx = (a); (a) = (b); (b) = xxxx; }

#ifdef notdef /* We don't use these routines. */

/* Y_Intercept will return the value 'yi' where the the coordinate
 * (e,yi) is the intersection of the vertical line x = e and the line
 * determined by the coordinates (x1,y1) and (x2,y2).
 */

static void
Y_Intercept(x1,y1,x2,y2,e,yi)
    int x1,y1,x2,y2;                /* two points on line */
    int e;                          /* vertical line of intercept */
    int *yi;                        /* y coordinate of intercept */
{
    *yi = y1;
    if (x1 != x2)
        *yi = y1 + ((e - x1) * (y2 - y1))/(x2 - x1);
    return;
}

/* X_Intercept will return the value 'xi' where the the coordinate
 * (xi,e) is the intersection of the horizontal line y = e and the line
 * determined by the coordinates (x1,y1) and (x2,y2).
 */

static void
X_Intercept(x1,y1,x2,y2,e,xi)
    int x1,y1,x2,y2;                /* two points on line */
    int e;                          /* horizontal line of intercept */
    int *xi;                        /* x coordinate of intercept */
{
    *xi = x1;
    if (y1 != y2)
        *xi = x1 + ((e - y1) * (x2 - x1))/(y2 - y1);
    return;
}

#endif

/* clip_line will clip a line to a rectangular area.  The returned
 * value is 'true' if the line is out of the AOI (therefore does not
 * need to be displayed) and 'false' if the line is in the AOI.
 */

bool
clip_line(pX1,pY1,pX2,pY2,l,b,r,t)
    int *pX1, *pY1, *pX2, *pY2, l, b, r, t;
{
    int x1 = *pX1;
    int y1 = *pY1;
    int x2 = *pX2;
    int y2 = *pY2;
    int x,y,c,c1,c2;

    CODE(x1,y1,c1)
    CODE(x2,y2,c2)
    while (c1 || c2) {
        if (c1 & c2)
            return (true); /* Line is invisible. */
        if (!(c = c1))
            c = c2;
        if (c & CODEMINX) {
            y = y1+(y2-y1)*(l-x1)/(x2-x1);
            x = l;
        } else if (c & CODEMAXX) {
            y = y1+(y2-y1)*(r-x1)/(x2-x1);
            x = r;
        } else if (c & CODEMINY) {
            x = x1+(x2-x1)*(b-y1)/(y2-y1);
            y = b;
        } else if (c & CODEMAXY) {
            x = x1+(x2-x1)*(t-y1)/(y2-y1);
            y = t;
        }
        if (c == c1) {
            x1 = x;
            y1 = y;
            CODE(x,y,c1)
        } else {
            x2 = x;
            y2 = y; 
            CODE(x,y,c2)
        }
    }
    *pX1 = x1;
    *pY1 = y1;
    *pX2 = x2;
    *pY2 = y2;
    return (false); /* Line is at least partially visible.*/
}

/* This routine will clip a line to a circle, returning true if the line
 * is entirely outside the circle.  Note that we have to be careful not
 * to switch the points around, since in grid.c we need to know which is
 * the outer point for putting the label on.
 */

bool
clip_to_circle(x1, y1, x2, y2, cx, cy, rad)
    int *x1, *y1, *x2, *y2,  cx, cy, rad;
{
    double perplen, a, b, c;
    double tx, ty, dt;
    double dtheta;
    double theta1, theta2, tt, alpha, beta, gamma;
    bool flip = false;
    int i;

    /* Get the angles between the origin and the endpoints. */
    theta1 = atan2((double) *y1 - cy, (double) *x1 - cx);
    theta2 = atan2((double) *y2 - cy, (double) *x2 - cx);
    if (theta1 < 0.0)
        theta1 = 2 * PI + theta1;
    if (theta2 < 0.0)
        theta2 = 2 * PI + theta2;

    dtheta = theta2 - theta1;
    if (dtheta > PI)
        dtheta = dtheta - 2 * PI;
    else if (dtheta < - PI)
        dtheta = 2 * PI - dtheta;

    /* Make sure that p1 is the first point... */
    if (dtheta < 0) {
        tt = theta1;
        theta1 = theta2;
        theta2 = tt;
        i = *x1;
        *x1 = *x2;
        *x2 = i;
        i = *y1;
        *y1 = *y2;
        *y2 = i;
        flip = true;
        dtheta = -dtheta;
    }

    /* Figure out the distances between the points... */
    a = sqrt((double) ((*x1 - cx) * (*x1 - cx) + (*y1 - cy) * (*y1 - cy)));
    b = sqrt((double) ((*x2 - cx) * (*x2 - cx) + (*y2 - cy) * (*y2 - cy)));
    c = sqrt((double) ((*x1 - *x2) * (*x1 - *x2) +
            (*y1 - *y2) * (*y1 - *y2)));

    /* We have three cases now -- either the midpoint of the line is
     * closest to the origon, or point 1 or point 2 is...  Actually the
     * midpoint won't in general be the closest, but if a point besides
     * one of the endpoints is closest, the midpoint will be closer than
     * both endpoints.
     */
    tx = (*x1 + *x2) / 2;
    ty = (*y1 + *y2) / 2;
    dt = sqrt((double) ((tx - cx) * (tx - cx) + (ty - cy) * (ty - cy)));
    if ((dt < a) && (dt < b)) {
        /* This is wierd -- round-off errors I guess. */
        tt = (a * a + c * c - b * b) / (2 * a * c);
        if (tt > 1.0)
            tt = 1.0;
        else if (tt < -1.0)
            tt = -1.0;
        alpha = acos(tt);
        perplen = a * sin(alpha);
    } else if (a < b) {
        perplen = a;
    } else {
        perplen = b;
    }

    /* Now we should see if the line is outside of the circle... */
    if (perplen >= rad)
        return (true);

    /* It's at least partially inside... */
    if (a > rad) {
        tt = (a * a + c * c - b * b) / (2 * a * c);
        if (tt > 1.0)
            tt = 1.0;
        else if (tt < -1.0)
            tt = -1.0;
        alpha = acos(tt);
        gamma = asin(sin(alpha) * a / rad);
        if (gamma < PI / 2)
            gamma = PI - gamma;
        beta = PI - alpha - gamma;
        *x1 = cx + rad * cos(theta1 + beta);
        *y1 = cy + rad * sin(theta1 + beta);
    }
    if (b > rad) {
        tt = (c * c + b * b - a * a) / (2 * b * c);
        if (tt > 1.0)
            tt = 1.0;
        else if (tt < -1.0)
            tt = -1.0;
        alpha = acos(tt);
        gamma = asin(sin(alpha) * b / rad);
        if (gamma < PI / 2)
            gamma = PI - gamma;
        beta = PI - alpha - gamma;
        *x2 = cx + rad * cos(theta2 - beta);
        *y2 = cy + rad * sin(theta2 - beta);
    }
    if (flip) {
        i = *x1;
        *x1 = *x2;
        *x2 = i;
        i = *y1;
        *y1 = *y2;
        *y2 = i;
    }
    return (false);
}

#ifdef notdef

/* We don't use this routine at all. */

/* clip_polygon will take the polygon 'poly' and clip it to the
 * box described by 'maxy','miny','minx', and 'maxx'.
 * Returns true if any of the polygon was actually inside the bounding
 * box.
 */

bool
clip_polygon(verts, num, newverts, newnum, minx, miny, maxx, maxy)
    int *verts, num;
    int **newverts, *newnum;
    int maxy,miny,minx,maxx;      /* bounding box */
{
    static int polybuf1[POLYGONBUFSIZE];
    static int polybuf2[POLYGONBUFSIZE];
    int *pbuf1 = polybuf1, *pbuf2 = polybuf2;
    int i,k,n,*swap;
    int lowx, lowy, hix, hiy;

    if (maxy < miny)
        SWAPINT(maxy,miny);
    if (maxx < minx)
        SWAPINT(minx,maxx);

    n = num * 2;
    if (n < 4)
        return (true);
    n = MIN(POLYGONBUFSIZE,n);
    lowx = hix = verts[0];
    lowy = hiy = verts[1];
    for (i=0; i<n; i++) {
        pbuf2[i] = verts[i];
        hix = MAX(pbuf2[i],hix);
        lowx = MIN(pbuf2[i],lowx);
        i++;
        pbuf2[i] = verts[i];
        hiy = MAX(pbuf2[i],hiy);
        lowy = MIN(pbuf2[i],lowy);
    }

    /* Are we entirely inside the box? */
    if ((hix <= maxx) && (lowx >= minx) && (hiy <= maxy) &&
            (lowy >= miny)) {
        *newverts = verts;
        *newnum = num;
        return (true);
    }

    /* Are we entirely outside of the box? */
    if ((hix <= minx) || (lowx >= maxx) || (hiy <= miny) ||
            (lowy >= maxy)) {
        *newnum = 0;
        *newverts = NULL;
        return (false);
    }

    /* start with the maxx side first */
    k = i = 0;
    while (pbuf2[i] > maxx && i < n)
        i += 2;
    if (i == 0) {
        pbuf1[k++] = pbuf2[i++];
        pbuf1[k++] = pbuf2[i++];
    } else {
        pbuf1[k++] = maxx;
        Y_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],pbuf2[i+1],
                maxx,&pbuf1[k++]);
    }
    while (i < n) {
        if (pbuf2[i] <= maxx) {
            pbuf1[k++] = pbuf2[i++];
            pbuf1[k++] = pbuf2[i++];
        } else {
            if (pbuf2[i-2] != maxx) {
                pbuf1[k++] = maxx;
                Y_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],
                        pbuf2[i+1], maxx,&pbuf1[k++]);
            }
            while (i < n && pbuf2[i] > maxx)
                i += 2;
            if (i < n && pbuf2[i] <= maxx) {
                pbuf1[k++] = maxx;
                Y_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],
                        pbuf2[i+1], maxx,&pbuf1[k++]);
            }
        }
    }
    if ((pbuf2[0] > maxx && pbuf2[n-2] <= maxx) ||
            (pbuf2[0] <= maxx && pbuf2[n-2] > maxx)) {
        pbuf1[k++] = maxx;
        Y_Intercept(pbuf2[0],pbuf2[1],pbuf2[n-2],pbuf2[n-1],maxx,
                &pbuf1[k++]);
    }
    n = MIN(POLYGONBUFSIZE,k);
    swap = pbuf1;
    pbuf1 = pbuf2;
    pbuf2 = swap;

    /* next the minx side */
    k = i = 0;
    while (pbuf2[i] < minx && i < n)
        i += 2;
    if (i == 0) {
        pbuf1[k++] = pbuf2[i++];
        pbuf1[k++] = pbuf2[i++];
    } else {
        pbuf1[k++] = minx;
        Y_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],pbuf2[i+1],minx,
                &pbuf1[k++]);
    }
    while (i < n) {
        if (pbuf2[i] >= minx) {
            pbuf1[k++] = pbuf2[i++];
            pbuf1[k++] = pbuf2[i++];
        } else {
            if (pbuf2[i-2] != minx) {
                pbuf1[k++] = minx; 
                Y_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],
                        pbuf2[i+1], minx,&pbuf1[k++]);
            }
            while (i < n && pbuf2[i] < minx)
                i += 2;
            if (i < n && pbuf2[i] >= minx) {
                pbuf1[k++] = minx;
                Y_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],
                        pbuf2[i+1], minx,&pbuf1[k++]);
            }
        }
    }
    if ((pbuf2[0] < minx && pbuf2[n-2] >= minx) ||
            (pbuf2[0] >= minx && pbuf2[n-2] < minx)) {
        pbuf1[k++] = minx;
        Y_Intercept(pbuf2[0],pbuf2[1],pbuf2[n-2],pbuf2[n-1],minx,
            &pbuf1[k++]);
    }
    n = MIN(POLYGONBUFSIZE,k);
    swap = pbuf1;
    pbuf1 = pbuf2;
    pbuf2 = swap;

    /* next the maxy side */
    k = i = 0;
    while (pbuf2[i+1] > maxy && i < n)
        i += 2;
    if (i == 0) {
        pbuf1[k++] = pbuf2[i++];
        pbuf1[k++] = pbuf2[i++];
    } else {
        X_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],pbuf2[i+1],maxy,
                &pbuf1[k++]);
        pbuf1[k++] = maxy;
    }
    while (i < n) {
        if (pbuf2[i+1] <= maxy) {
            pbuf1[k++] = pbuf2[i++];
            pbuf1[k++] = pbuf2[i++];
        } else {
            if (pbuf2[i-1] != maxy) {
                X_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],
                        pbuf2[i+1], maxy,&pbuf1[k++]);
                pbuf1[k++] = maxy;
            }
            while (i < n && pbuf2[i+1] > maxy)
                i += 2;
            if (i < n && pbuf2[i+1] <= maxy) {
                X_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],
                        pbuf2[i+1], maxy,&pbuf1[k++]);
                pbuf1[k++] = maxy;
            }
        }
    }
    if ((pbuf2[1] > maxy && pbuf2[n-1] <= maxy) ||
            (pbuf2[1] <= maxy && pbuf2[n-1] > maxy)) {
        X_Intercept(pbuf2[0],pbuf2[1],pbuf2[n-2],pbuf2[n-1],maxy,
                &pbuf1[k++]);
        pbuf1[k++] = maxy;
    }
    n = MIN(POLYGONBUFSIZE,k);
    swap = pbuf1;
    pbuf1 = pbuf2;
    pbuf2 = swap;

    /* finally the miny side */
    i = k = 0;
    while (pbuf2[i+1] < miny && i < n)
        i += 2;
    if (i == 0) {
        pbuf1[k++] = pbuf2[i++];
        pbuf1[k++] = pbuf2[i++];
    } else {
        X_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],pbuf2[i+1],miny,
                &pbuf1[k++]);
        pbuf1[k++] = miny;
    }
    while (i < n) {
        if (pbuf2[i+1] >= miny) {
            pbuf1[k++] = pbuf2[i++];
            pbuf1[k++] = pbuf2[i++];
        } else {
            if (pbuf2[i-1] != miny) { 
                X_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],
                        pbuf2[i+1], miny,&pbuf1[k++]);
                pbuf1[k++] = miny;
            }
            while (i < n && pbuf2[i+1] < miny)
                i += 2;
            if (i < n && pbuf2[i+1] >= miny) {
                X_Intercept(pbuf2[i-2],pbuf2[i-1],pbuf2[i],
                        pbuf2[i+1], miny,&pbuf1[k++]);
                pbuf1[k++] = miny;
            }
        }
    }
    if ((pbuf2[1] < miny && pbuf2[n-1] >= miny) ||
            (pbuf2[1] >= miny && pbuf2[n-1] < miny)) {
        X_Intercept(pbuf2[0],pbuf2[1],pbuf2[n-2],pbuf2[n-1],miny,
                &pbuf1[k++]);
        pbuf1[k++] = miny;
    }
    k = MIN(POLYGONBUFSIZE,k);

    /* Transfer coordinates to poly buffer and one last clipping */
    *newverts = (int *) tmalloc(k * sizeof (int));
    *newnum = k / 2;
    for (i=0; i<k; i += 2) {
        (*newverts)[i] = MAX(MIN(pbuf1[i],maxx),minx);
        (*newverts)[i+1] = MAX(MIN(pbuf1[i+1],maxy),miny);
    }
    return (true);
}

#endif

