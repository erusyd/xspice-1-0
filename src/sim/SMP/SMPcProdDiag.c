/*
 * Copyright (c) 1985 Mani B. Srivastava
 */

    /*
     * SMPcProdDiag(matrix,det,power)
     *    this routine finds the product of the diagnol elements
     *    in complex numbers
     *    this is used to find the determinant
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include <math.h>
#include "complex.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPcProdDiag.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:19 $")

int 
SMPcProdDiag(matrix,det,power)
    register SMPmatrix *matrix;     /* matrix to work on */
    SPcomplex *det;
    int *power;
{
    register SMPelement *diag;      /* pointer to element on diagonal */
    SMPelement *SMPfindElt();
    double test;
    int n;                          /* counter of what #'d diagonal element */
                                    /* we are trying to work on now */
    SPcomplex ctemp;                /* temporary because c doesn't have */
                                    /* inline complex operations */
    static char *badmsg = "Improperly formed matrix - shouldn't happen!";
    

    det->real=1.0 ; det->imag=0.0;
    *power = 0;
    /* walk down diagonal */
    for(n=1;n<=matrix->SMPsize;n++) {
        diag = SMPfindElt(matrix,n,n,0);
        if(diag == (SMPelement *)NULL) {
            /* diagonal element doesn't exist!  IMPOSSIBLE! */
            errMsg = MALLOC(strlen(badmsg)+1);
            strcpy(errMsg,badmsg);
            return(E_BADMATRIX);
        }
    /*multiply*/
    DC_MULT(det->real , det->imag , diag->SMPvalue ,
    diag->SMPiValue , &(ctemp.real) , &(ctemp.imag));
    test=(fabs(ctemp.real)>fabs(ctemp.imag)) ?
    fabs(ctemp.real) : fabs(ctemp.imag) ;
    if (test !=0.0)
        {
        while (test>1.0)
            {
            test=test/10.0;
            ctemp.real=ctemp.real/10.0;
            ctemp.imag=ctemp.imag/10.0;
            *power=(*power) + 1;
                        };
        while (test<=0.1)
            {
            test=test*10.0;
            ctemp.real=ctemp.real*10.0;
            ctemp.imag=ctemp.imag*10.0;
            *power=(*power) - 1;
                        };
        };
    det->real=ctemp.real;
    det->imag=ctemp.imag;
    }
    return(OK);
}
