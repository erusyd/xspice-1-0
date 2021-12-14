/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* SMPreorder(matrix) -
     *      Perform markowitz reordering of given matrix.
     *      (considering both fillin minimization and
     *      accuracy.  Necessary fill-in elements are
     *      created.
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include <math.h>
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPreorder.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:45 $")

int
SMPreorder(matrix,pivtol,pivrel,gmin)
    register SMPmatrix * matrix;
    double pivtol;
    double pivrel;
    double gmin;    /* minimal value to be added to diagonal */
{
    register int n;
    double max;
    double relmax;
    register SMPelement * here;
    register SMPelement * diag;
    SMPelement * col;
    SMPelement * row;
    register long ops;
    long minOps;
    int savei;
    int savej;
    register int i;
    int j;
    static char *singmsg = "Matrix is nearly singular";

    /* first, do some bookkeeping for statistics purposes - 
     * these initial calculations are NOT used for further numeric
     * operations withing the sparse matrix package
     */
    if (matrix->SMPoldNonZ == 0) {
        /* first time we have done a reordering on this matrix,
         * so we begin to save statistics about it 
         */
        matrix->SMPoldNonZ = matrix->SMPnonZero;
    } else {
        for(i=1;i<=matrix->SMPsize;i++) { /* clear out the count arrays */
            (*((matrix->SMProwCount) +i)) = 0 ;
            (*((matrix->SMPcolCount) +i)) = 0 ;
        }
        for(i=1;i<=matrix->SMPsize;i++) {
            here = *(matrix->SMProwHead+i);
            while (here != NULL) {
                if(here->SMProwNumber != here->SMPcolNumber) {
                    (*((matrix->SMProwCount)+(here->SMProwNumber))) ++;
                    (*((matrix->SMPcolCount)+(here->SMPcolNumber))) ++;
                }
                here = here->SMProwNext;
            }
        }
    } /* end of bookkeeping operations - back to real calculations */


    for ( n = 1 ; n<=matrix->SMPsize ; n = n + 1 ) {
        max = 0;
        /* run down the column to find the maximum value 
         * below the diag. */
        for ( here = *(matrix->SMPcolHead + n) ;
                here != NULL ;
                here = here->SMPcolNext) {
            if (here->SMProwNumber <n) continue;
            if (FABS(here->SMPvalue)
                < max) continue;
            max=FABS(here->SMPvalue);
        }
        if (max < pivtol) {
#ifdef DEBUG
             /*printf(" SMPreorder: max. entry in column too small\n");*/
             /*printf(" after %d steps, max entry is only %g \n",n,max); */
             /*SMPprint(matrix, stderr); */
            /*ABORT();*/
#endif /*DEBUG*/
            matrix->SMPbadi = SMPintToExtMapRow(n,matrix);
            matrix->SMPbadj = SMPintToExtMapCol(n,matrix);
            errMsg = MALLOC(strlen(singmsg)+1);
            strcpy(errMsg,singmsg);
            return(E_SINGULAR);
        }
        relmax = (pivtol > (pivrel * max) ) ? pivtol : (pivrel * max);
        /* now try to pivot on the diagonal (if good enough value
         * is available - i.e. withing .1% of best )
         */
        /* next for lint because MAXPOSINT is machine dependent */
#ifdef LINT
        minOps = 0;
#else /* LINT */
        minOps = MAXPOSINT; /* a very large number - i.e. infinity */
#endif /*LINT*/
        savei = 0;
        savej = 0;
        for (i=n;i<=matrix->SMPsize;i=i+1) {
            /* note the '-1' is missing from the next two lines - this
             * is because the -1 is included in the 'count' fields already
             * to save subtractions here
             */
            ops = (*(matrix->SMProwCount + i) ) *
                (*(matrix->SMPcolCount + i));
            if (ops >= minOps) continue;
                /* ground row or missing element - 
                 * - not in consideration */
            if ((diag = SMPfindElt(matrix,i,i,0)) == NULL) continue;
                /* big enough to consider? */
            if (relmax >= FABS(diag->SMPvalue)) continue;
            savei = i;
            savej = i;
            minOps=ops;
            if(minOps <= 0) goto found; /* found a great one - why
                        * look any longer?  just
                        * jump all the way out */
        }
        if (savei == 0) {
            /* didn't find a pivot on the diagonal, so have
             * to look all over the matrix to find one */
             for (i=n;i<=matrix->SMPsize;i++) {
                for (here = *(matrix->SMProwHead + i);
                    here != NULL;
                    here = here->SMProwNext) {
                    j = here->SMPcolNumber;
                    if(j<n) continue;
                    if(FABS(here->SMPvalue) < relmax)continue;
                    ops=((*(matrix->SMProwCount+i))) *
                        ((*(matrix->SMPcolCount+j)));
                    if (ops>minOps) continue;
                    minOps = ops;
                    savei = i;
                    savej = j;
                }
            }
        }
found: /* label to branch to if ideal pivot found */
        if (savei != n) {
            /* need to swap rows savei and n */
#ifdef DEBUG
            fflush(stdout);
#endif /*DEBUG*/
            SMProwSwap(matrix,savei,n);
        }
        if (savej != n) {
            /* need to swap columns savej and n */
#ifdef DEBUG
            fflush(stdout);
#endif /*DEBUG*/
            SMPcolSwap(matrix,savej,n);
        }
        /* now generate the appropriate fill-ins */
        diag = SMPfindElt(matrix,n,n,0);
        diag->SMPvalue += gmin;
        for(col=diag->SMPcolNext; col!= NULL; col = col->SMPcolNext) {
            /* divide column by diagonal */
            col->SMPvalue /= diag->SMPvalue;
            i = col->SMProwNumber;
            /* now move across the row */
            for (row = diag->SMProwNext;row != NULL  ;
                    row = row->SMProwNext) {
                j = row->SMPcolNumber;
                /* now correct each remaining row */
                /* this is a high-cost operation - should do better */
                here = SMPfindElt(matrix,i,j,1);
                if(here == (SMPelement *)NULL) return(E_NOMEM); /* no memory! */
                here->SMPvalue -= (col->SMPvalue * row->SMPvalue);
            }
            /* now correct counts for non-zero terms */
            (*(matrix->SMProwCount + i)) --;
        }
        for (row = diag->SMProwNext;row != NULL  ;
                row = row->SMProwNext) {
            j = row->SMPcolNumber;
            (*(matrix->SMPcolCount + j)) --;
        }
    }
    return(OK);
}
