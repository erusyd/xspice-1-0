/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * SMPluFac(matrix,pivtol)
     *      -performs an in-place L-U factorization of the
     *      given matrix.  
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include <math.h>
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPluFac.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:34 $")

int 
SMPluFac(matrix,pivtol,gmin)
register SMPmatrix *matrix;
double pivtol;
double gmin;
{
    int n;  /* counter of what #'d diagonal element we are
             * trying to work on now */
    
    register int j;
    register SMPelement *diag;
    register SMPelement *row;
    register SMPelement *col;
    register SMPelement *across;
    static char *badmsg = "Improperly formed matrix - shouldn't happen!(luFac)";
    static char *singmsg = "Matrix is nearly singular";

    /* walk down diagonal */
    for(n=1;n<=matrix->SMPsize;n++) {
        diag = SMPfindElt(matrix,n,n,0);
        if(diag == (SMPelement *)NULL) {
            errMsg = MALLOC(strlen(badmsg)+1);
            strcpy(errMsg,badmsg);
            return(E_BADMATRIX); /* impossible! */
        }
        /* make sure value not too small - pivoting to fix later... */
        diag->SMPvalue += gmin;
        if( FABS(diag->SMPvalue)<pivtol) {
#ifdef DEBUG
            /*printf("in trouble - diagonal too small! - at %d it is\n",n);*/
            /*printf("only %g\n",diag->SMPvalue);*/
            /*fflush(stdout);*/
            /*ABORT();*/
#endif /*DEBUG*/
            matrix->SMPbadi = SMPintToExtMapRow(n,matrix);
            matrix->SMPbadj = SMPintToExtMapCol(n,matrix);
            errMsg = MALLOC(strlen(singmsg)+1);
            strcpy(errMsg,singmsg);
            return(E_SINGULAR);
        }
        /* now move down the col */
        for(col = diag->SMPcolNext; col != NULL;col = col->SMPcolNext) {
            /* divide col by diagonal */
            col->SMPvalue /= 
                diag->SMPvalue;
            /* now move across the row */
            across = col;
            for (row = diag->SMProwNext; row != NULL;
                    row = row->SMProwNext) {
                j=row->SMPcolNumber;
                /* now correct each element remaining */
                for(; across!= NULL && across->SMPcolNumber != j;
                        across = across->SMProwNext) {
                    ; /* do nothing useful - just walk
                        across row until we reach
                        right column */
                }
#ifdef DEBUG
                if(across == NULL || across->SMPcolNumber != j) {
                    printf("error:  badly formed matrix");
                    return(E_SINGULAR);
                }
#endif /* DEBUG */
                across->SMPvalue -= (col->SMPvalue * row->SMPvalue);
            }
        }
    }
    return(OK);
}
