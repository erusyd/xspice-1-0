/*
 * Copyright (c) 1985 Thomas L. Quarles
 */

#include "prefix.h"
#include "SMPdefs.h"
#include "suffix.h"

RCSID("SMPgetError.c $Revision: 1.1 $ on $Date: 91/04/02 11:48:33 $")

/* SMPgetError(matrix,i,j)
 *
 * get the i,j coordinates of the last E_SINGULAR error to occur in the given
 * matrix (if any).  returned value undefined if an E_SINGULAR has not
 * occurred
 */
void
SMPgetError(matrix,i,j)
    int *i;
    int *j;
    SMPmatrix *matrix;

{
    *i = matrix->SMPbadi;
    *j = matrix->SMPbadj;
    return;
}
