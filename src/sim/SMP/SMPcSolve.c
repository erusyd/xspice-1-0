/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * SMPcSolve(matrix,rhs,irhs,spare,ispare) 
     *      - solve (perform forward/back substitution on) the
     *      matrix given, using the supplied Right Hand Side.
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "complex.h"
#include <math.h>
#include "suffix.h"

RCSID("SMPcSolve.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:22 $")

void
SMPcSolve(matrix,rhs,irhs,spare,ispare)
    register SMPmatrix *matrix;
    register double rhs[];
    register double irhs[];
    register double spare[];
    register double ispare[];

{
    register SMPelement * here;
    register SMPelement * diag;
    register int row;
    SPcomplex ctemp;

    /*  first, permute the rhs correctly */

    for(row=1;row<=matrix->SMPsize;row++) {
        spare[SMPextToIntMapRow(row,matrix)] = rhs[row];
        ispare[SMPextToIntMapRow(row,matrix)] = irhs[row];
    }

    /*  forward substitution */

    /* step to each row */
    for(row=1;row<=matrix->SMPsize;row++) {
        here = *(matrix->SMProwHead + row);
        /* walk through the row as far as the diagonal */
        while (here != NULL && here->SMPcolNumber<row) {
            if(here->SMPcolNumber!=0) {
                /* and perform the appropriate computation on the RHS*/
                DC_MULT( (spare[here->SMPcolNumber]),
                        (ispare[here->SMPcolNumber]),
                        (here->SMPvalue), (here->SMPiValue),
                        &(ctemp.real), &(ctemp.imag) );
                DC_MINUSEQ( &(spare[row]), &(ispare[row]),
                        (ctemp.real), (ctemp.imag) );
            }
            here = here->SMProwNext;
        }
    }

    /*  backward substitution */

    /* step to each row */
    for (row=matrix->SMPsize;row>=1;row--) {
        /* find diagonal element */
        diag = *(matrix->SMProwHead + row);
        while (diag != NULL && diag->SMPcolNumber<row) {
            diag = diag->SMProwNext;
                /* do nothing interesting to these elements
                 * below the diagonal - just walk along */
        }
        /* remembering diagonal element, go across rest of row...*/
        here = diag->SMProwNext;
        /* while still in row, */
        while ( here != NULL) {
            /* perform appropriate calculation */
            DC_MULT( (here->SMPvalue), (here->SMPiValue),
                    (spare[here->SMPcolNumber]),
                    (ispare[here->SMPcolNumber]),
                    &(ctemp.real), &(ctemp.imag) );
            DC_MINUSEQ( &(spare[row]), &(ispare[row]),
                    (ctemp.real), (ctemp.imag) );
            /* and continue across row */
            here = here->SMProwNext;
        }
        /* using saved knowledge about the diagonal element, perform
         * the final operation for the row */
        DC_DIVEQ( &(spare[row]), &(ispare[row]),
                (diag->SMPvalue), (diag->SMPiValue) );
    }

    /* finally, map the rhs back to the correct order for outside */
    for(row=1;row<=matrix->SMPsize;row++) {
        rhs[SMPintToExtMapCol(row,matrix)] = spare[row];
        irhs[SMPintToExtMapCol(row,matrix)] = ispare[row];
    }
}
