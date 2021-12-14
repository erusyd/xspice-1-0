/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
#ifndef CMPLX
#define CMPLX "complex.h $Revision: 1.1 $  on $Date: 91/04/02 11:26:53 $ "

/*  header file containing definitions for complex functions
 *
 *  Each expects two arguments for each complex number - a real and an
 *  imaginary part.
 */
typedef struct {
    double real;
    double imag;
} SPcomplex;


#define DC_ABS(a,b) (FABS(a) + FABS(b))

#ifdef notdef
#define DC_DIV(a,b,c,d,x,y) { \
    double r,s;\
    if(FABS(c)>FABS(d)) { \
        r=(d)/(c);\
        s=(c)+r*(d);\
        x=((a)+(b)*r)/s;\
        y=((b)-(a)*r)/s;\
    } else { \
        r=(c)/(d);\
        s=(d)+r*(c);\
        x=((a)*r+(b))/s;\
        y=((b)*r-(a))/s;\
    }\
}
#endif /*notdef */

#ifndef SHORTMACRO
#define DC_DIVEQ(a,b,c,d) { \
    double r,s,x,y;\
    if(FABS(c)>FABS(d)) { \
        r=(d)/(c);\
        s=(c)+r*(d);\
        x=((*(a))+(*(b))*r)/s;\
        y=((*(b))-(*(a))*r)/s;\
    } else { \
        r=(c)/(d);\
        s=(d)+r*(c);\
        x=((*(a))*r+(*(b)))/s;\
        y=((*(b))*r-(*(a)))/s;\
    }\
    (*(a)) = x; \
    (*(b)) = y; \
}
#else /*SHORTMACRO*/
#define DC_DIVEQ DCdiveq
#ifdef __STDC__
extern void DCdiveq(double*,double*,double,double);
#else /* stdc */
extern void DCdiveq();
#endif /* stdc */
#endif /*SHORTMACRO*/

#ifndef SHORTMACRO
#define DC_MULT(a,b,c,d,x,y) { \
    *(x) = (a) * (c) - (b) * (d) ;\
    *(y) = (a) * (d) + (b) * (c) ;\
}
#else /*SHORTMACRO*/
#define DC_MULT DCmult
#ifdef __STDC__
extern void DCmult(double,double,double,double,double*,double*);
#else /* stdc */
extern void DCmult();
#endif /* stdc */
#endif /*SHORTMACRO*/

#ifdef notdef 
#define DC_MINUS(a,b,c,d,x,y) { \
    (x) = (a) - (c) ;\
    (y) = (b) - (d) ;\
}
#endif /*notdef*/

#ifndef SHORTMACRO
#define DC_MINUSEQ(a,b,c,d) { \
    *(a) -= (c) ;\
    *(b) -= (d) ;\
}
#else /*SHORTMACRO*/
#define DC_MINUSEQ DCminusEq
#ifdef __STDC__
extern void DCminusEq(double*,double*,double,double);
#else /* stdc */
extern void DCminusEq();
#endif /* stdc */
#endif /*SHORTMACRO*/

#endif /*CMPLX*/
