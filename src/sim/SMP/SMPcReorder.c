/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* SMPcReorder(matrix) -
     *      Perform markowitz reordering of given complex matrix.
     *      (considering both fillin minimization and
     *      accuracy.  Necessary fill-in elements are
     *      created.)
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include <math.h>
#include "complex.h"
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPcReorder.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:21 $")

int
SMPcReorder(matrix,pivtol,pivrel,numswaps)
    register SMPmatrix * matrix;    /* the matrix to work on */
    double pivtol;                  /* minimum acceptable pivot */
    double pivrel;                  /* minimum fraction of best pivot 
                                     * we will accept for sparsity reasons 
                                     */
    register int *numswaps;          /* keeps track whether number of 
                                     * swaps is odd or even
                                     * used for finding determinant
                                     */
{
    register int n;
    double max;
    double relmax;
    SPcomplex ctemp;
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
     */
    if (matrix->SMPoldNonZ == 0) {
        /* first time we have done a reordering on this matrix,
         * so we begin to save statistics about it 
         */
        matrix->SMPoldNonZ = matrix->SMPnonZero;
    } else {
        /* need to recompute counts so markowitz reordering will be done
         * correctly below.  We destroyed these counts during last reorder
         */
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
            if (DC_ABS( (here->SMPvalue), (here->SMPiValue) ) < max) continue;
            max=DC_ABS( (here->SMPvalue), (here->SMPiValue) );
        }
        if (max < pivtol) {
            /*printf(" SMPcReorder: max. entry in column too small\n");*/
            /*printf(" after %d steps, max entry has abs of only %e ",n,max);*/
            /*SMPprint(matrix,stdout);*/
            /*ABORT();*/
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
            if (relmax >= DC_ABS( (diag->SMPvalue), (diag->SMPiValue) )) 
                continue;
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
                    if(DC_ABS( (here->SMPvalue), (here->SMPiValue) ) < 
                            relmax) continue;
                    ops=((*(matrix->SMProwCount+i))) *
                        ((*(matrix->SMPcolCount+j)));
                    if (ops>minOps) continue;
                    minOps = ops;
                    savei = i;
                    savej = j;
                }
            }
        }
        if (savei == 0) {
            errMsg = MALLOC(strlen(singmsg)+1);
            strcpy(errMsg,singmsg);
            return(E_SINGULAR);
        }
found: /* label to branch to if ideal pivot found */
        if (savei != n) {
            /* need to swap rows savei and n */
#ifdef DEBUG
            fflush(stdout);
#endif /*DEBUG*/
            SMProwSwap(matrix,savei,n);
            *numswaps *= -1;
        }
        if (savej != n) {
            /* need to swap columns savej and n */
#ifdef DEBUG
            fflush(stdout);
#endif /*DEBUG*/
            SMPcolSwap(matrix,savej,n);
            *numswaps *= -1;
        }
        /* now generate the appropriate fill-ins */
        diag = SMPfindElt(matrix,n,n,0);
        if(diag == (SMPelement *)NULL) { 
            /* diagonal term is missing!   but we just created it above, so
             * something is really funny!  bail out!
             */
            errMsg = MALLOC(strlen(singmsg)+1);
            strcpy(errMsg,singmsg);
            return(E_SINGULAR);
        }
        for(col=diag->SMPcolNext; col!= NULL; col = col->SMPcolNext) {
            /* divide column by diagonal */
            DC_DIVEQ( &(col->SMPvalue), &(col->SMPiValue),
                    (diag->SMPvalue), (diag->SMPiValue) );
            i = col->SMProwNumber;
            /* now move across the row */
            for (row = diag->SMProwNext;row != NULL  ;
                    row = row->SMProwNext) {
                j = row->SMPcolNumber;
                /* now correct each remaining row */
                /* this is a high-cost operation - should do better */
                here = SMPfindElt(matrix,i,j,1);
                if(here == (SMPelement *)NULL)  return(E_NOMEM);
                DC_MULT((col->SMPvalue), (col->SMPiValue),
                        (row->SMPvalue), (row->SMPiValue),
                        &(ctemp.real), &(ctemp.imag) );
                DC_MINUSEQ( &(here->SMPvalue), &(here->SMPiValue),
                        (ctemp.real), (ctemp.imag) );
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
