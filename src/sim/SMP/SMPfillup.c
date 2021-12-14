/*
 * Copyright (c) 1985 Thomas L. Quarles
 */
    /*
     * SMPfillup(matrix)
     *      - fill up (not in) the matrix with zero entries
     *      for debugging purposes - want to create a full
     *      matrix just to be safe....
     */
    
#include "prefix.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "SPerror.h"
#include "suffix.h"

RCSID("SMPfillup.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:30 $")

int
SMPfillup(matrix)
    register SMPmatrix * matrix;
{
    register int i;
    register int j;
    register double *check;
    double * SMPmakeElt();
    for (i=0;i<=matrix->SMPsize;i++) {
        for (j=1;j<=matrix->SMPsize;j++) {
            check = SMPmakeElt(matrix,i,j);
            if(check == NULL) return(E_NOMEM);
        }
    }
    return(OK);
}
