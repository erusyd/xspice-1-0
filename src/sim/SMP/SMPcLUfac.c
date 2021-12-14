/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * SMPcLUfac(matrix,pivtol)
     *      -performs an in-place L-U factorization of the
     *      given complex matrix.  
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include <math.h>
#include "complex.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPcLUfac.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:18 $")

int 
SMPcLUfac(matrix,pivtol)
    register SMPmatrix *matrix;     /* matrix to work on */
    double pivtol;                  /* minimum acceptable pivot absolute value*/
{
    register SMPelement *here;      /* pointer to current element */
    register SMPelement *diag;      /* pointer to element on diagonal */
    register SMPelement *row;       /* pointer to current row element */
    register SMPelement *col;       /* pointer to current column element */
    register int j;                 /* column number we are working with */
    int n;                          /* counter of what #'d diagonal element */
                                    /* we are trying to work on now */
    SPcomplex ctemp;                /* temporary because c doesn't have */
                                    /* inline complex operations */
    static char *badmsg = "Improperly formed matrix - shouldn't happen!";
    static char *singmsg = "Matrix is nearly singular";
    

    /* walk down diagonal */
    for(n=1;n<=matrix->SMPsize;n++) {
        diag = SMPfindElt(matrix,n,n,0);
        if(diag == (SMPelement *)NULL) {
            /* diagonal element doesn't exist!  IMPOSSIBLE! */
            errMsg = MALLOC(strlen(badmsg)+1);
            strcpy(errMsg,badmsg);
            return(E_BADMATRIX);
        }
        /* make sure value not too small - */
        if( DC_ABS( (diag->SMPvalue), (diag->SMPiValue) ) <pivtol) {
#ifdef DEBUG
            /*printf("in trouble - diagonal too small! - at %d it is\n",n);*/
            /*printf("only %g + %gi\n",diag->SMPvalue,diag->SMPiValue);*/
            /*fflush(stdout);*/
            /*ABORT();*/
#endif /*DEBUG*/
            /* the diagonal is too small, so we can't continue with the
             * decomposition now.  It may be possible to decompose the
             * matrix if it is reordered again, but we can't deal with
             * that here, so we just claim that it is singular and let
             * higher level code decide whether to try to reorder again.
             */
            matrix->SMPbadi = SMPintToExtMapRow(n,matrix);
            matrix->SMPbadj = SMPintToExtMapCol(n,matrix);
            errMsg = MALLOC(strlen(singmsg)+1);
            strcpy(errMsg,singmsg);
            return(E_SINGULAR);
        }
        /* now move down the col */
        for(col = diag->SMPcolNext; col != NULL;col = col->SMPcolNext) {
            /* divide col by diagonal */
            DC_DIVEQ( &(col->SMPvalue), &(col->SMPiValue),
                    (diag->SMPvalue), (diag->SMPiValue) );
            /* now move across the row */
            for (row = diag->SMProwNext; row != NULL;
                    row = row->SMProwNext) {
                j=row->SMPcolNumber;
                /* now correct each element remaining */
                for(here = col->SMProwNext; 
                        here!= NULL &&
                        here->SMPcolNumber != j;
                        here = here->SMProwNext) {
                    ; /* do nothing useful - just walk
                        across row until we reach
                        right column */
                }
                DC_MULT( (col->SMPvalue), (col->SMPiValue),
                        (row->SMPvalue), (row->SMPiValue),
                        &(ctemp.real), &(ctemp.imag) );
                DC_MINUSEQ( &(here->SMPvalue), &(here->SMPiValue),
                        (ctemp.real), (ctemp.imag) );
            }
        }
    }
    return(OK);
}
