/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 11:57:19 $
 *           $Source: //pepper/atesse_spice/spice3/INP/RCS/PTfunctions.c,v $
 * Copyright (c) 1987 Wayne A. Christopher, U. C. Berkeley CAD Group 
 *   faustus@cad.berkeley.edu, ucbvax!faustus
 * Permission is granted to modify and re-distribute this code in any manner
 * as long as this notice is preserved.  All standard disclaimers apply.
 *
 * All the functions used in the parse tree.  These functions return HUGE
 * if their argument is out of range.
 */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "IFsim.h"
#ifndef CMS
#include "INPparseTree.h"
#else  /* CMS */
#include "INPparse.h"
#endif /* CMS */
#include "suffix.h"

RCSID("PTfunctions.c $Revision: 1.1 $ on $Date: 91/04/02 11:57:19 $")

#define PI 3.14159265358979323846

/* These should be in math.h */

#ifdef HASATRIGH
extern double asinh(), acosh(), atanh();
#endif /* HASATRIGH */

/* Reduce the argument to be within the range 0 - 2 pi.  This is necessary
 * because the trig functions are broken.
 */

static double
reduce(arg)
    double arg;
{
    int i = arg / PI;

    return (arg - i * PI);
}

double
PTplus(arg1, arg2)
    double arg1, arg2;
{
    return (arg1 + arg2);
}

double
PTminus(arg1, arg2)
    double arg1, arg2;
{
    return (arg1 - arg2);
}

double
PTtimes(arg1, arg2)
    double arg1, arg2;
{
    return (arg1 * arg2);
}

double 
PTdivide(arg1, arg2)
    double arg1, arg2;
{
    if (arg2 == 0)
        return (HUGE);

    return (arg1 / arg2);
}

double
PTpower(arg1, arg2)
    double arg1, arg2;
{
    return (pow(arg1, arg2));
}

double
PTacos(arg)
    double arg;
{
    return (acos(arg));
}

#ifdef HASATRIGH
double
PTacosh(arg)
    double arg;
{
    return (acosh(arg));
}
#endif /* HASATRIGH */

double
PTasin(arg)
    double arg;
{
    return (asin(arg));
}

#ifdef HASATRIGH
double
PTasinh(arg)
    double arg;
{
    return (asinh(arg));
}
#endif /* HASATRIGH */

double
PTatan(arg)
    double arg;
{
    return (atan(arg));
}

#ifdef HASATRIGH
double
PTatanh(arg)
    double arg;
{
    return (atanh(arg));
}
#endif /* HASATRIGH */

double
PTcos(arg)
    double arg;
{
    return (cos(reduce(arg)));
}

double
PTcosh(arg)
    double arg;
{
    return (cosh(arg));
}

double
PTexp(arg)
    double arg;
{
    return (exp(arg));
}

double
PTln(arg)
    double arg;
{
    return (log(arg));
}

double
PTlog(arg)
    double arg;
{
    return (log10(arg));
}

double
PTsin(arg)
    double arg;
{
    return (sin(reduce(arg)));
}

double
PTsinh(arg)
    double arg;
{
    return (sinh(arg));
}

double
PTsqrt(arg)
    double arg;
{
    return (sqrt(arg));
}

double
PTtan(arg)
    double arg;
{
    return (tan(reduce(arg)));
}

double
PTtanh(arg)
    double arg;
{
    return (tanh(arg));
}

double
PTuminus(arg)
    double arg;
{
    return (- arg);
}

