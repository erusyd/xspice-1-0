/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

    /*
     * SMPdestroy(matrix)
     *      - destroy a matrix managed by the
     *      sparse matrix package.  
     */

#include "prefix.h"
#include "util.h"
#include "SMPdefs.h"
#include <stdio.h>
#include "suffix.h"

RCSID("SMPdestroy.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:28 $")

void
SMPdestroy(matrix)
    register SMPmatrix *matrix;
{
    register SMPelement *here;
    register SMPelement *prev;
    register int i;

    if(matrix->SMProwCount)     FREE(matrix->SMProwCount);
    if(matrix->SMPcolCount)     FREE(matrix->SMPcolCount);
    if(matrix->SMProwMapIn)     FREE(matrix->SMProwMapIn);
    if(matrix->SMPcolMapIn)     FREE(matrix->SMPcolMapIn);
    if(matrix->SMProwMapOut)    FREE(matrix->SMProwMapOut);
    if(matrix->SMPcolMapOut)    FREE(matrix->SMPcolMapOut);
    if(matrix->SMProwHead)      FREE(matrix->SMProwHead);

    if(matrix->SMPcolHead) {
        for(i=0; i< matrix->SMPsize;i++) {
            here = *(matrix->SMPcolHead+i);
            while(prev = here) {
                here = prev->SMPcolNext;
                FREE(prev);
            }
        }
        FREE(matrix->SMPcolHead);
    }
    FREE(matrix);
}
