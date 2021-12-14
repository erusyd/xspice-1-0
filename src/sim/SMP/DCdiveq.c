/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include <math.h>
#include "suffix.h"

RCSID("DCdiveq.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:14 $")

#ifdef SHORTMACRO
void
DCdiveq(a,b,c,d)
    double *a,*b,c,d;
{
    /* the Double Complex divide-equals operator.  This is done
     * as a macro on most machines, but some machines can't handle the
     * length of the macros necessary, so the following code is provided
     * for them.
     *
     * this is the complex operation (a,b) /= (c,d)
     *
     */
    double r,s,x,y;

    if(FABS(c)>FABS(d)) { 
        r=(d)/(c);
        s=(c)+r*(d);
        x=((*(a))+(*(b))*r)/s;
        y=((*(b))-(*(a))*r)/s;
    } else { 
        r=(c)/(d);
        s=(d)+r*(c);
        x=((*(a))*r+(*(b)))/s;
        y=((*(b))*r-(*(a)))/s;
    }
    (*(a)) = x;
    (*(b)) = y;
}

void
DCmult(a,b,c,d,x,y) 
    double a,b,c,d,*x,*y;
{
    *(x) = (a) * (c) - (b) * (d) ;
    *(y) = (a) * (d) + (b) * (c) ;
}

void
DCminusEq(a,b,c,d) 
    double c,d,*a,*b;
{
    *(a) -= (c) ;
    *(b) -= (d) ;
}

#endif /*SHORTMACRO*/

