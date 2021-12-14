/*
 * Copyright (c) 1985 Mani B. Srivasatva
 */

    /*
     * NIpzSolve(ckt,s,listptr,f,type,power)
     *
     *This subroutine calculates the value 
     *             det[SMP(s)] / ((s-r1)*(s-r2)* ...  *(s-rn))
     *where SMP(s) is the matrix at complex frequency s with some
     *modifications depending on the value of type
     *and
     *r1,r2, ... ,rn are the roots already found and are in a list
     *pointed to by listptr
     */

#include "prefix.h"
#include <stdio.h>
#include <math.h>
#include "TRANdefs.h"
#include "complex.h"
#include "CKTdefs.h"
#include "SMPdefs.h"
#include "PZdefs.h"
#include "util.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("NIpzSolve.c $Revision: 1.1 $ on $Date: 91/04/02 11:47:32 $ ")

int
NIpzSolve(ckt,s,listptr,f,type,power)
    register CKTcircuit *ckt;
    SPcomplex *s;
    root *listptr;
    SPcomplex *f;
    int type;
    int *power;
{
    int error;
    double tempreal,tempimag;
    int wastype;
    double test;
    int power1,power2;

    wastype=0;
    /* printf("NIpzSolve:entering \n"); */
    /* printf("s=(%5.6e,%5.6e) \n",s->real,s->imag); */

retry:
    ckt->CKTnoncon=0;

    s->real=(s->real) * 1.0e6;
    s->imag=(s->imag) * 1.0e6;
    /* printf("NIpzSolve:calling PZload with type=%5d \n",type); */
    error=CKTpzLoad(ckt,s,type);
    s->real=(s->real) * 1.0e-6;
    s->imag=(s->imag) * 1.0e-6;
    if (error) return(error);
    
    if(ckt->CKTniState & NIPZSHOULDREORDER) {
        /* printf("NIpzSolve:calling SMPcReorder \n"); */
        error = SMPcReorder(ckt->CKTmatrix,ckt->CKTpivotAbsTol,
                ckt->CKTpivotRelTol,&(((PZAN*)ckt->CKTcurJob)->PZnumswaps));
        /* printf("NIpzSolve:came out of SMPcReorder \n"); */
        ckt->CKTniState &= ~NIPZSHOULDREORDER;
        if (error==E_SINGULAR) {
            power1=0;
            f->real=0.0;
            f->imag=0.0;
            goto detzero;
        };
        if(error != 0) {
            /* no memory,
             * let caller handle problem
             */
            return(error);
        }
    } else {
        /* printf("NIpzSolve:calling SMPcLUfac \n"); */
        error = SMPcLUfac(ckt->CKTmatrix,ckt->CKTpivotAbsTol);
        /* printf("NIpzSolve:came out of SMPcLUfac \n"); */
        if(error != 0) {
            if(error == E_SINGULAR) {
                /* the problem is that the matrix can't be solved with the
                 * current LU factorization.  Maybe if we reload and
                 * try to reorder again it will help...
                 */
                ckt->CKTniState |= NIPZSHOULDREORDER;
                goto retry;
            }
            return(error); /* can't handle E_BADMATRIX, so let caller */
        }
    } 
    
    /*calculate determinant by the product of diagnol*/
    /* printf("entering SMPcProdDiag\n"); */
    error=SMPcProdDiag(ckt->CKTmatrix,f,&power1);
    /* printf("came out of SMPcProdDiag\n"); */
    if (error) return(error);
    f->real *= ((PZAN*)ckt->CKTcurJob)->PZnumswaps;
    f->imag *= ((PZAN*)ckt->CKTcurJob)->PZnumswaps;
    detzero:
    /* printf("determinant=(%5.6e,%5.6e) \n",f->real,f->imag); */
    /* printf("power=%5.d\n",power1); */
    if ((type==2) || (type==4) || (type==5) ||
            (type==13) || (type==14) || (type==15) || (type==16)) {
        type=6;
        tempreal=f->real;
        tempimag=f->imag;
        power2=power1;
        wastype=1;
        goto retry;
    };
    if (wastype==1) {
        if (power1==power2) {
            f->real=tempreal-f->real;
            f->imag=tempimag-f->imag; 
        } else if (power2>power1) {
            f->real=tempreal-f->real/(pow(10.0,(double)(power2-power1)));
            f->imag=tempimag-f->imag/(pow(10.0,(double)(power2-power1)));
            power1=power2;
        } else /* (power2<power1) */ {
            f->real=tempreal/(pow(10.0,(double)(power1-power2)))-f->real;
            f->imag=tempimag/(pow(10.0,(double)(power1-power2)))-f->imag;
        };
    };
    *power = power1;
    /*divide by terms of the form (s-rn) */
    /* printf("Final determinant=(%5.6e,%5.6e) \n",f->real,f->imag); */
    /* printf("power=%5.d\n",*power); */
    /* printf("dividing out the roots\n"); */
    while (listptr != (root *)(NULL)) {
        tempreal=(s->real)-(listptr->real);
        tempimag=(s->imag)-(listptr->imag);
        /* printf("denom=(%5.6e,%5.6e)\n",tempreal,tempimag); */
        DC_DIVEQ(&(f->real) , &(f->imag) , tempreal , tempimag);
        /* printf("answr=(%5.6e,%5.6e)\n",f->real,f->imag); */
        /* printf("power=%5.d\n",*power); */
        test=(fabs(f->real)>fabs(f->imag)) ?
        fabs(f->real) : fabs(f->imag) ;
        /* printf("test=%5.6e\n",test); */
        if (test !=0.0) {
            while (test>1.0) {
                test=test/10.0;
                f->real=f->real/10.0;
                f->imag=f->imag/10.0;
                *power=(*power) + 1;
            };
            while (test<=0.1) {
                test=test*10.0;
                f->real=f->real*10.0;
                f->imag=f->imag*10.0;
                *power=(*power) - 1;
            };
        };
        listptr=listptr->next;
        /* printf("normalized answr=(%5.6e,%5.6e)\n",f->real,f->imag); */
        /* printf("power=%5.d\n",*power); */
    }

    /* printf("function=(%5.6e,%5.6e) \n",f->real,f->imag); */
    /* printf("power=%5.d\n",*power); */
    return(OK);
}
