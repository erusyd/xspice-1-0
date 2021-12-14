/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * SMPsolve(matrix,rhs,spare) 
     *      - solve (perform forward/back substitution on) the
     *      matrix given, using the supplied Right Hand Side.
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "suffix.h"

RCSID("SMPsolve.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:48 $")

void
SMPsolve(matrix,rhs,spare)
    register SMPmatrix *matrix;
    register double rhs[];
    register double spare[];

{
    register SMPelement * here;
    register SMPelement * diag;
    register int row;

    /*  first, permute the rhs correctly */

    for(row=1;row<=matrix->SMPsize;row++) {
        spare[SMPextToIntMapRow(row,matrix)] = rhs[row];
    }
    /*  forward substitution */

    /* step to each row */
    for(row=1;row<=matrix->SMPsize;row++) {
        here = *(matrix->SMProwHead + row);
        /* walk through the row as far as the diagonal */
        while (here != NULL && here->SMPcolNumber<row) {
            if(here->SMPcolNumber!=0) {
                /* and perform the appropriate computation on the RHS*/
                spare[row] -= 
                    ( spare[here->SMPcolNumber]
                        * (here->SMPvalue));
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
            spare[row] -= ((here->SMPvalue)) * (spare[here->SMPcolNumber]);
            /* and continue across row */
            here = here->SMProwNext;
        }
        /* using saved knowledge about the diagonal element, perform
         * the final operation for the row */
        spare[row] /= (diag->SMPvalue);
    }

    for(row=1;row<=matrix->SMPsize;row++) {
        rhs[SMPintToExtMapCol(row,matrix)] = spare[row];
    }
}
