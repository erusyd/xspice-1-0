/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /* SMPcClear(matrix)
     *  clear the matrix to all zeroes
     */

#include "prefix.h"
#include <stdio.h>
#include "SMPdefs.h"
#include "suffix.h"

RCSID("SMPcClear.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:17 $")

void
SMPcClear(matrix)

    register SMPmatrix *matrix;

{
    
    register SMPelement *here;

    register int i;

    for (i=0;i<=matrix->SMPsize;i++) {
        for (here = *(matrix->SMProwHead + i ); here != NULL ;
                        here = here->SMProwNext) {
            here->SMPvalue = 0;
            here->SMPiValue = 0;
        }
    }
}
